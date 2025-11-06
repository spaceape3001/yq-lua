////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "lualua.hpp"
#include "errors.hpp"
#include <yq/errors.hpp>
#include <yq/core/Any.hpp>
#include <yq/core/Flags.hpp>
#include <yq/core/Object.hpp>
#include <yq/core/Ref.hpp>
#include <yq/meta/ObjectMeta.hpp>
#include <lua.hpp>

namespace yq::lua {

    struct Reg;

    namespace {
        using value_t = std::variant<
            std::monostate,
            std::string,
            bool,
            double,
            int,
            Object*,
            const Object*,
            void*,
            FNLuaCallback
        >;
        
        struct Value {
            std::string key;
            std::string help;
            std::string description;
            value_t     value;
        };

        bool _push(lua_State* l, const value_t& val, int n=-1)
        {
            if(std::get_if<std::monostate>(&val)){
                lua_pushnil(l);
                return true;
            } else if(auto p = std::get_if<std::string>(&val)){
                push(l, *p);
                return true;
            } else if(auto p = std::get_if<bool>(&val)){
                push(l, *p);
                return true;
            } else if(auto p = std::get_if<double>(&val)){
                push(l, *p);
                return true;
            } else if(auto p = std::get_if<int>(&val)){
                lua_pushinteger(l, *p);
                return true;
            } else if(auto p = std::get_if<Object*>(&val)){
                push(l, *p);
                return true;
            } else if(auto p = std::get_if<const Object*>(&val)){
                push(l, *p);
                return true;
            } else if(auto p = std::get_if<void*>(&val)){
                push(l, *p);
                return true;
            } else if(auto p = std::get_if<FNLuaCallback>(&val)){
                if(n<0)
                    return false;
                push(l, *p, n);
                return true;
            } else  
                return false;
        }
    }
    
    struct Reg {
        std::string             key;
        std::string             help, description;
        value_t                 value;
        std::vector<Value>      values; // arguments/etc
        
        //bool pushy_push(lua_State* l)
        //{
            
        
            //if(!_push(l, value, (int) values.size()))
                //return false;
        //}
        
    };
    
    namespace {
        struct Repo {
            std::map<std::string,Reg*,IgCase>   globals;
        };
        
        Repo&    repo() 
        {
            static Repo s_repo;
            return s_repo;
        }
    }

    /////////////////////////////////////////////////////////////////////
    //  IMPLEMENTATION "micro" HELPERS

    std::error_code     _error(int n)
    {
        switch(n){
        case LUA_OK:
            return std::error_code();
        case LUA_ERRRUN:
            return errors::lua_runtime();
        case LUA_ERRMEM:
            return errors::lua_memory();
        case LUA_ERRERR:
            return errors::lua_message();
        case LUA_ERRSYNTAX:
            return errors::lua_syntax();
        case LUA_YIELD:
            return errors::lua_yield_code();
        case LUA_ERRFILE:
            return errors::lua_file();
        default:
            return errors::lua_unexpected();
        }
    }

    XFlags      _flags(lua_State *l, int n)
    {
        lua_pushstring(l, keyFlags);
        lua_gettable(l, n);
        XFlags  flags               = (XFlags::value_type) lua_tointeger(l, -1);
        _pop(l);
        return flags;
    }

    bool    _gc(lua_State*l, int n, object_k)
    {
        if(!l)
            return false;
        if(!lua_istable(l, n))
            return false;

        Object*obj                  = _object(l, n);
        XFlags flags                = _flags(l, n);
        const ObjectMeta*   meta    = _meta(l, n, OBJECT);

        if(!obj || !meta)               // neither meta nor obj
            return false;
        if(&obj->metaInfo() != meta)    // mismatch
            return false;

        if(flags(X::Ref)){
            if(Refable* ref = dynamic_cast<Refable*>(obj)){
                ref -> decRef();
            } else if(flags(X::Delete))
                delete obj;
        } else if(flags(X::Delete)){
            delete obj;
        }

        return true;
    }

    const Meta*         _meta(lua_State*l, int n)
    {
        lua_pushstring(l, keyMeta);
        lua_gettable(l, n);
        const Meta* obj = (const Meta*) lua_touserdata(l, -1);
        _pop(l);
        return obj;
    }
    
    const ObjectMeta*   _meta(lua_State*l, int n, object_k)
    {
        const Meta* m   = _meta(l, n);
        if(!m->is_object())
            return nullptr;
        return static_cast<const ObjectMeta*>(m);
    }
    
    const TypeMeta*     _meta(lua_State*l, int n, type_k)
    {
        const Meta* m   = _meta(l, n);
        if(!m->is_type())
            return nullptr;
        return static_cast<const TypeMeta*>(m);
    }

    void       _metamake(lua_State* l, const ObjectMeta& om)
    {
        std::string name(om.name());
        luaL_newmetatable(l, name.c_str());
        set(l, -1, TABLE, keyGarbageCollection, lh_gc_object);
    }

    Object*     _object(lua_State *l, int n)
    {
        lua_pushstring(l, keyPointer);
        lua_gettable(l, n);
        Object* obj = (Object*) lua_touserdata(l, -1);
        _pop(l);
        return obj;
    }

    std::error_code     _push(lua_State* l, Object* obj, XFlags flags)
    {
        if(!l)
            return errors::lua_null();

        if(!obj){
            lua_pushnil(l);
            return {};
        }
        
        lua_newtable(l);
        set(l, -1, TABLE, keyPointer, (void*) obj);
        set(l, -1, TABLE, keyMeta, (void*) &(obj->metaInfo()));
        set(l, -1, TABLE, keyFlags, flags.value());
        
        Refable* ref    = dynamic_cast<Refable*>(obj);
        if(ref && flags(X::Ref))
            ref -> incRef();
        
        std::string name    = std::string(obj->metaInfo().name());
        luaL_getmetatable(l, name.c_str());
        
        if(lua_isnil(l, -1)){
            _pop(l);
            _metamake(l, obj->metaInfo());
        }
        
        lua_setmetatable(l, -1);    // think this works....
        return {};
    }
    

    std::string_view    _type(int n)
    {
        switch(n){
        case LUA_TNIL:
            return "nil";
        case LUA_TNUMBER:
            return "number";
        case LUA_TBOOLEAN:
            return "boolean";
        case LUA_TSTRING:
            return "string";
        case LUA_TTABLE:
            return "table";
        case LUA_TFUNCTION:
            return "function";
        case LUA_TUSERDATA:
            return "userdata";
        case LUA_TTHREAD:
            return "thread";
        case LUA_TLIGHTUSERDATA:
            return "lightuserdata";
        default:
            return "unknown";
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //  EXTRACTION
    
    boolean_x           boolean(lua_State*l, int n)
    {
        if(!l)
            return errors::lua_null();
        return static_cast<bool>(lua_toboolean(l, n));
    }
    

    boolean_x           boolean(lua_State*l, global_k, const char* key)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();

        if(lua_getglobal(l, key) != LUA_OK)
            return errors::lua_bad_global();
        auto ret = boolean(l, -1);
        lua_pop(l, -1);
        return ret;
    }

    boolean_x           boolean(lua_State* l, upvalue_k, int n)
    {
        if((n<0) || (n>MAX_UPVALUES))
            return errors::bad_argument();
        return boolean(l, lua_upvalueindex(n));
    }
    

    double_x            double_(lua_State*l, int n)
    {
        if(!l)
            return errors::lua_null();
        int success    = 0;
        double  v   = lua_tonumberx(l, n, &success);
        if(!success)
            return errors::lua_not_number();
        return v;
    }
    
    double_x            double_(lua_State* l, global_k, const char* key)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();

        if(lua_getglobal(l, key) != LUA_OK)
            return errors::lua_bad_global();
        auto ret = double_(l, -1);
        lua_pop(l, -1);
        return ret;
    }

    double_x            double_(lua_State*l, upvalue_k, int n)
    {
        if((n<0) || (n>MAX_UPVALUES))
            return errors::bad_argument();
        return double_(l, lua_upvalueindex(n));
    }


    integer_x             integer(lua_State* l, int n)
    {
        if(!l)
            return errors::lua_null();
        int success    = 0;
        int  v   = lua_tointegerx(l, n, &success);
        if(!success)
            return errors::lua_not_number();
        return v;
    }

    integer_x             integer(lua_State* l, global_k, const char* key)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();

        if(lua_getglobal(l, key) != LUA_OK)
            return errors::lua_bad_global();
        auto ret = integer(l, -1);
        lua_pop(l, -1);
        return ret;
    }
    
    integer_x             integer(lua_State* l, upvalue_k, int n)
    {
        if((n<0) || (n>MAX_UPVALUES))
            return errors::bad_argument();
        return integer(l, lua_upvalueindex(n));
    }
    


    object_ptr_x        object(lua_State* l, int n)
    {
        if(!l)
            return errors::lua_null();
            
        if(!lua_istable(l, n))
            return errors::lua_not_table();
            
        XFlags  flags               = _flags(l, n);
        if(flags(X::Const))
            return errors::lua_const_object();

        const ObjectMeta*   meta    = _meta(l, n, OBJECT);
        if(!meta)
            return errors::lua_no_meta();

        Object* obj                 = _object(l, n);
        if(!obj)
            return errors::lua_null_pointer();
        if(meta != &obj->metaInfo())    // sanity check
            return errors::lua_meta_mismatch();
        
        return obj;
    }

    object_cptr_x       object(lua_State* l, int n, const_k)
    {
        if(!l)
            return errors::lua_null();
            
        if(!lua_istable(l, n))
            return errors::lua_not_table();
            
        const ObjectMeta*   meta    = _meta(l, n, OBJECT);
        if(!meta)
            return errors::lua_no_meta();

        const Object*       obj     = _object(l, n);
        if(!obj)
            return errors::lua_null_pointer();
        if(meta != &obj->metaInfo())    // sanity check
            return errors::lua_meta_mismatch();

        return obj;
    }

    object_ptr_x        object(lua_State* l, int n, const ObjectMeta& om)
    {
        if(!l)
            return errors::lua_null();
            
        if(!lua_istable(l, n))
            return errors::lua_not_table();

        XFlags  flags               = _flags(l, n);
        if(flags(X::Const))
            return errors::lua_const_object();
            
        const ObjectMeta*   meta    = _meta(l, n, OBJECT);
        if(!meta)
            return errors::lua_no_meta();
            
        if(!meta->is_base(om))
            return errors::lua_wrong_object_type();

        Object* obj                 = _object(l, n);
        if(!obj)
            return errors::lua_null_pointer();
        if(meta != &obj->metaInfo())    // sanity check
            return errors::lua_meta_mismatch();
        
            
        return obj;
    }

    object_cptr_x       object(lua_State* l, int n, const ObjectMeta& om, const_k)
    {
        if(!l)
            return errors::lua_null();
            
        if(!lua_istable(l, n))
            return errors::lua_not_table();
            
        const ObjectMeta*   meta    = _meta(l, n, OBJECT);
        if(!meta)
            return errors::lua_no_meta();

        if(!meta->is_base(om))
            return errors::lua_wrong_object_type();

        const Object*       obj     = _object(l, n);
        if(!obj)
            return errors::lua_null_pointer();
        if(meta != &obj->metaInfo())    // sanity check
            return errors::lua_meta_mismatch();
        return obj;
    }
    
    object_ptr_x        object(lua_State*l, global_k, const char*key)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();
        if(lua_getglobal(l, key) != LUA_OK)
            return errors::lua_bad_global();
            
        auto ret            = object(l, -1);
        lua_pop(l, -1);
        return ret;
    }
    
    object_cptr_x       object(lua_State*l, global_k, const char*key, const_k)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();
        if(lua_getglobal(l, key) != LUA_OK)
            return errors::lua_bad_global();
            
        auto ret            = object(l, -1, CONST);
        lua_pop(l, -1);
        return ret;
    }
    
    object_ptr_x        object(lua_State*l, global_k, const char*key, const ObjectMeta&om)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();
        if(lua_getglobal(l, key) != LUA_OK)
            return errors::lua_bad_global();
            
        auto ret            = object(l, -1, om);
        lua_pop(l, -1);
        return ret;
    }
    
    object_cptr_x       object(lua_State*l, global_k, const char*key, const ObjectMeta&om, const_k)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();
        if(lua_getglobal(l, key) != LUA_OK)
            return errors::lua_bad_global();
            
        auto ret            = object(l, -1, om, CONST);
        lua_pop(l, -1);
        return ret;
    }

    object_ptr_x        object(lua_State* l, upvalue_k, int n)
    {
        if((n<0) || (n>MAX_UPVALUES))
            return errors::bad_argument();
        return object(l, lua_upvalueindex(n));
    }

    object_cptr_x       object(lua_State* l, upvalue_k, int n, const_k)
    {
        if((n<0) || (n>MAX_UPVALUES))
            return errors::bad_argument();
        return object(l, lua_upvalueindex(n), CONST);
    }

    object_ptr_x        object(lua_State* l, upvalue_k, int n, const ObjectMeta& om)
    {
        if((n<0) || (n>MAX_UPVALUES))
            return errors::bad_argument();
        return object(l, lua_upvalueindex(n), om);
    }
    
    object_cptr_x       object(lua_State* l, upvalue_k, int n, const ObjectMeta& om, const_k)
    {
        if((n<0) || (n>MAX_UPVALUES))
            return errors::bad_argument();
        return object(l, lua_upvalueindex(n), om, CONST);
    }
    
    void_ptr_x          pointer(lua_State* l, int n)
    {
        if(!l)
            return errors::lua_null();
        return lua_touserdata(l, -1);
    }

    void_ptr_x          pointer(lua_State* l, global_k, const char*key)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();
        
        if(lua_getglobal(l, key) != LUA_OK)
            return errors::lua_bad_global();
        auto ret = pointer(l, -1);
        lua_pop(l, -1);
        return ret;
    }

    
    string_view_x       string(lua_State*l, int n)
    {
        if(!l)
            return errors::lua_null();
        size_t  sz  = 0;
        const char* v   = lua_tolstring(l, n, &sz);
        if(!v)
            return errors::lua_bad_type();
        return std::string_view(v, sz);
    }

    string_x            string(lua_State*l, global_k, const char*key)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();
        
        if(lua_getglobal(l, key) != LUA_OK)
            return errors::lua_bad_global();
            
        auto x              = string(l, -1);
        string_x    ret;
        if(x){
            ret     = std::string(*x);
        } else {
            ret     = unexpected(x.error());
        }
        
        lua_pop(l, -1);
        return ret;
    }

    string_view_x       string(lua_State*l, upvalue_k, int n)
    {
        if((n<0) || (n>MAX_UPVALUES))
            return errors::bad_argument();
        return string(l, lua_upvalueindex(n));
    }

    any_x  value(lua_State* l, int n)
    {
        if(!l)
            return errors::lua_null();

        // TODO
        return errors::todo();
    }

    any_x  value(lua_State* l, upvalue_k, int n)
    {
        if((n<0) || (n>MAX_UPVALUES))
            return errors::bad_argument();
        return value(l, lua_upvalueindex(n));
    }

    void_ptr_x          voidptr(lua_State* l, int n)
    {
        if(!l)
            return errors::lua_null();
        if(!lua_islightuserdata(l, n))
            return errors::lua_bad_type();
        return lua_touserdata(l,n);
    }

    void_ptr_x          voidptr(lua_State*l, global_k, const char*key)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();
        
        if(lua_getglobal(l, key) != LUA_OK)
            return errors::lua_bad_global();
            
        auto ret            = voidptr(l, -1);
        lua_pop(l, -1);
        return ret;
    }

    void_ptr_x          voidptr(lua_State* l, upvalue_k, int n)
    {
        if((n<0) || (n>MAX_UPVALUES))
            return errors::bad_argument();
        return voidptr(l, lua_upvalueindex(n));
    }
    
    //////////////////////////////////////////////////////////////////////////////
    //  PUSH/SET

    std::error_code     push(lua_State* l, const Any& v)
    {
        if(!l)
            return errors::lua_null();
        
        if(!v.valid()){
            lua_pushnil(l);
            return {};
        }    
        
        const TypeMeta& tm  = v.type();
        
        
        
        //LuaVM*  lvm = vm(l);
        //if(!lvm)
            //return errors::lua_badvm();
            
        // TODO
        return errors::todo();
    }

    std::error_code     push(lua_State* l, bool v)
    {
        if(!l)
            return errors::lua_null();
        lua_pushboolean(l, v);
        return {};
    }
    
    std::error_code     push(lua_State* l, double v)
    {
        if(!l)
            return errors::lua_null();
        lua_pushnumber(l, v);
        return {};
    }
    
    std::error_code     push(lua_State* l, int v)
    {
        if(!l)
            return errors::lua_null();
        lua_pushinteger(l,v);
        return {};
    }
    

    std::error_code     push(lua_State* l, std::nullptr_t)
    {
        if(!l)
            return errors::lua_null();
        lua_pushnil(l);
        return {};
    }
    
    std::error_code     push(lua_State* l, std::string_view v)
    {
        if(!l)
            return errors::lua_null();
        lua_pushlstring(l, v.data(), v.size());
        return {};
    }
    
    std::error_code     push(lua_State* l, void* v)
    {
        if(!l)
            return errors::lua_null();
        lua_pushlightuserdata(l, v);
        return {};
    }

    std::error_code     push(lua_State*l, FNLuaCallback fn)
    {
        if(!l)
            return errors::lua_null();
        if(!fn)
            return errors::null_pointer();
        lua_pushcfunction(l, fn);
        return {};
    }
    
    std::error_code     push(lua_State*l, FNLuaCallback fn, size_t n)
    {
        if(!l)
            return errors::lua_null();
        if(!fn)
            return errors::null_pointer();
        if(n>MAX_UPVALUES)
            return errors::lua_too_many_upvalues();
        if(lua_gettop(l) < (int) n)
            return errors::lua_insufficent_upvalue_arguments();
        lua_pushcclosure(l, fn, (int) n);
        return {};
    }

    std::error_code     push(lua_State*l, const_k, const Object* obj)
    {
        return _push(l, (Object*) obj, X::Const);
    }
    
    std::error_code     push(lua_State*l, Object* obj)
    {
        return _push(l, obj, XFlags{});
    }

    std::error_code      set(lua_State*l, int n, table_k, const char* key, bool v)
    {   
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();

        int tm  = lua_absindex(l, n);
        lua_pushboolean(l, v);
        lua_pushstring(l, key);
        lua_settable(l, tm);
        return {};
    }
    
    std::error_code         set(lua_State*l, int n, table_k, const char* key, double v)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();

        int tm  = lua_absindex(l, n);
        lua_pushnumber(l, v);
        lua_pushstring(l, key);
        lua_settable(l, tm);
        return {};
    }
    
    std::error_code         set(lua_State*l, int n, table_k, const char* key, int v)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();

        int tm  = lua_absindex(l, n);
        lua_pushinteger(l, v);
        lua_pushstring(l, key);
        lua_settable(l, tm);
        return {};
    }
    
    std::error_code         set(lua_State*l, int n, table_k, const char* key, std::string_view v)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();

        int tm  = lua_absindex(l, n);
        lua_pushlstring(l, v.data(), v.size());
        lua_pushstring(l, key);
        lua_settable(l, tm);
        return {};
    }
    
    std::error_code         set(lua_State*l, int n, table_k, const char* key, const_k, const Object* v)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();
        if(!v)
            return errors::null_pointer();

        int tm  = lua_absindex(l, n);
        _push(l, const_cast<Object*>(v), X::Const );
        lua_pushstring(l, key);
        lua_settable(l, tm);
        return {};
    }
    
    std::error_code         set(lua_State*l, int n, table_k, const char*key, Object*v)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();
        if(!v)
            return errors::null_pointer();

        int tm  = lua_absindex(l, n);
        _push(l, v, XFlags{} );
        lua_pushstring(l, key);
        lua_settable(l, tm);
        return {};
    }
    
    std::error_code         set(lua_State*l, int n, table_k, const char* key, void* v)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();
        if(!v)
            return errors::null_pointer();

        int tm  = lua_absindex(l, n);
        _push(l, v );
        lua_pushstring(l, key);
        lua_settable(l, tm);
        return {};
    }
    
    std::error_code         set(lua_State*l, int n, table_k, const char* key, FNLuaCallback v, size_t cnt)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();
        if(!v)
            return errors::null_pointer();
        if((int) cnt > MAX_UPVALUES)
            return errors::lua_too_many_upvalues();
        if((int) cnt > lua_gettop(l))
            return errors::lua_insufficent_upvalue_arguments();

        int tm  = lua_absindex(l, n);
        lua_pushcclosure(l, v, (int) cnt );
        lua_pushstring(l, key);
        lua_settable(l, tm);
        return {};
    }

    std::error_code     set(lua_State*l, global_k, const char* key, bool v)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();
            
        push(l, v);
        lua_setglobal(l, key);
        return {};
    }
    
    std::error_code     set(lua_State*l, global_k, const char* key, double v)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();

        push(l, v);
        lua_setglobal(l, key);
        return {};
    }
    
    std::error_code     set(lua_State*l, global_k, const char* key, int v)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();

        push(l, v);
        lua_setglobal(l, key);
        return {};
    }
    
    std::error_code     set(lua_State*l, global_k, const char* key, std::string_view v)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();

        push(l, v);
        lua_setglobal(l, key);
        return {};
    }
    
    std::error_code     set(lua_State*l, global_k, const char* key, void* v)
    {
        if(!l)
            return errors::lua_null();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();

        push(l, v);
        lua_setglobal(l, key);
        return {};
    }
    
    
    //////////////////////////////////////////////////////////////////////////////
    //  Lua Handlers (can use these....)

    int lh_gc_object(lua_State* l)
    {
        if(!_gc(l, 1, OBJECT))
            return 0;
        _pop(l);
        return -1;
    }

    
    //////////////////////////////////////////////////////////////////////////////

    RegWriter           reg(global_k, const char*);

    RegWriter           reg(global_k, const char*, bool);
    RegWriter           reg(global_k, const char*, int);
    RegWriter           reg(global_k, const char*, double);
    RegWriter           reg(global_k, const char*, void*);
    RegWriter           reg(global_k, const char*, Object*);
    RegWriter           reg(global_k, const char*, std::string_view);
    RegWriter           reg(global_k, const char*, const Object*);
    RegWriter           reg(global_k, const char*, FNLuaCallback);

    RegWriter&  RegWriter::description(std::string_view v)
    {
        if(reg)
            reg -> description  = std::string(v);
        return *this;
    }
    
    RegWriter&  RegWriter::help(std::string_view v)
    {
        if(reg)
            reg -> help     = std::string(v);
        return *this;
    }
    
    RegWriter&  RegWriter::arg(std::string_view k, std::string_view description, std::string_view help)
    {
        // TODO
        return *this;
    }

    /////////////////////////////////////////////////////////////////////
    /*
        CONVENTIONS
        
        Want something like ... (could be a global routine)
        geo = geodetic( 50, 75, 10 )
        geo = geodetic( "W45 N36 7000ft" )
        
        
    */

#if 0
    std::error_code     fn_register(lua_State* l, global_k, const char* key, FNLuaCallback fn)
    {
        if(!l)
            return errors::null_pointer();
        if(!fn)
            return errors::null_pointer();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();
        lua_pushcfunction(l, fn);
        lua_setglobal(l, key);
        return std::error_code();
    }

    std::error_code     fn_register(lua_State*l, global_k, const char* key, FNLuaCallback fn, size_t n)
    {
        if(!l)
            return errors::null_pointer();
        if(!fn)
            return errors::null_pointer();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();
        if(n > MAX_UPVALUES)
            return errors::lua_too_many_upvalues();
        if(lua_gettop(l) < (int) n)
            return errors::lua_insufficent_upvalue_arguments();
        lua_pushcclosure(l, fn, (int) n);
        lua_setglobal(l, key);
        return std::error_code();
    }
#endif

    //////////////////////////////////////////////////////////////////////////////


    LuaVM* vm(lua_State* l)
    {
        void_ptr_x  x = voidptr(l, GLOBAL, keyVM);
        if(!x)
            return nullptr;
        return (LuaVM*) *x;
    }


}
