////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "extract.ipp"
#include "handler.ipp"
#include "impl.ipp"
#include "push.ipp"
#include "set.ipp"
#include "info/ArgumentInfo.ipp"
#include "info/FunctionInfo.ipp"
#include "info/Info.ipp"
#include "info/ModuleInfo.ipp"
#include "info/ObjectInfo.ipp"
#include "info/TypeInfo.ipp"
#include "info/ValueInfo.ipp"

#if 0
#include "repo.ipp"

#include "lualua.hpp"
#include "errors.hpp"
#include <yq/errors.hpp>
#include <yq/core/Any.hpp>
#include <yq/core/Flags.hpp>
#include <yq/core/Object.hpp>
#include <yq/core/Ref.hpp>
#include <yq/meta/ObjectMeta.hpp>
#include <lua.hpp>

    /*
        CONVENTIONS
        
        Want something like ... (could be a global routine)
        geo = geodetic( 50, 75, 10 )
        geo = geodetic( "W45 N36 7000ft" )
        
        
    */



namespace yq::lua {




    


    //////////////////////////////////////////////////////////////////////////////
    //  EXTRACTION
    
    //////////////////////////////////////////////////////////////////////////////
    //  PUSH/SET



    //////////////////////////////////////////////////////////////////////////////
    //  HELP

    size_t                  count(global_k)
    {
        return repo().globals.size();
    }
    
    size_t                  count(global_k, const std::string& k, arguments_k)
    {
        const Function* v   = 
    }
    
    size_t                  count(global_k, std::string_view, results_k);
    size_t                  count(module_k);
    
    std::string_view        description(global_k, std::string_view);
    std::string_view        description(global_k, std::string_view, argument_k, size_t);
    std::string_view        description(global_k, std::string_view, result_k, size_t);
    std::string_view        description(module_k, std::string_view);

    std::string_view        help(global_k, std::string_view);
    std::string_view        help(global_k, std::string_view, argument_k, size_t);
    std::string_view        help(global_k, std::string_view, result_k, size_t);
    std::string_view        help(module_k, std::string_view);

    std::string_view        key(global_k, std::string_view, argument_k, size_t);
    std::string_view        key(global_k, std::string_view, result_k, size_t);

    const string_xset_t&    keys(global_k, all_k);
    const string_xset_t&    keys(module_k, all_k);

    
    //////////////////////////////////////////////////////////////////////////////
    //  REGISTRATION

    namespace {
        Reg*        _reg(const char* key)
        {
            auto& _repo = repo();
            auto [i,f]  = _repo.globals.insert({key, nullptr});
            if(!i->second){
                i->second           = new Reg;
                i->second -> key    = key; 
            }
            return i->second;
        }
    }

    RegWriter           reg(global_k, const char* key)
    {
        Reg*    r  = _reg(key);
        r->value    = {};
        return {r};
    }

    RegWriter           reg(global_k, const char* key, bool v)
    {
        Reg*    r  = _reg(key);
        r->value    = v;
        return {r};
    }
    
    RegWriter           reg(global_k, const char* key, int v)
    {
        Reg*    r  = _reg(key);
        r->value    = v;
        return {r};
    }
    
    RegWriter           reg(global_k, const char* key, double v)
    {
        Reg*    r  = _reg(key);
        r->value    = v;
        return {r};
    }
    
    RegWriter           reg(global_k, const char* key, void* v)
    {
        if(!v)
            return {};
            
        Reg*    r  = _reg(key);
        r->value    = v;
        return {r};
    }
    
    RegWriter           reg(global_k, const char*key, Object*v)
    {
        if(!v)
            return {};

        Reg*    r  = _reg(key);
        r->value    = v;
        return {r};
    }
    
    RegWriter           reg(global_k, const char* key, std::string_view v)
    {
        Reg*    r  = _reg(key);
        r->value    = std::string(v);
        return {r};
    }
    
    RegWriter           reg(global_k, const char* key, const Object* v)
    {
        if(!v)
            return {};

        Reg*    r  = _reg(key);
        r->value    = v;
        return {r};
    }
    
    RegWriter           reg(global_k, const char* key, FNLuaCallback v)
    {
        if(!v)
            return {};

        Reg*    r  = _reg(key);
        r->value    = v;
        return {r};
    }


    bool                has_global(lua_State* l, const char* key);
    {
        if(!l)
            return false;
        if(!key)
            return false;
        if(!*key)
            return false;
        if(lua_getglobal(l, key) != LUA_OK)
            return false;
        _pop(l);
        return true;
    }
    

    // meant for a help system...

    size_t                  reg_count(global_k)
    {
        return repo().globals.size();
    }
    
    size_t                  reg_count(global_k, std::string_view k, args_k)
    {
        const Reg* r    = _reg(k);
        if(!r)
            return 0;
        return r->values.size();
    }
    
    std::string_view        reg_description(global_k, std::string_view );
    std::string_view        reg_description(global_k, std::string_view, arg_k, size_t );
    std::string_view        reg_description(global_k, std::string_view, arg_k, std::string_view );
    std::string_view        reg_help(global_k, std::string_view );
    std::string_view        reg_help(global_k, std::string_view, arg_k, std::string_view );
    std::string_view        reg_help(global_k, std::string_view, arg_k, size_t );
    std::string_view        reg_key(global_k, std::string_view, arg_k, size_t );
    string_view_vector_t    reg_keys(global_k, std::string_view, args_k);

    //////////////////////////////////////////////////////////////////////////////
    //  HELP

    size_t                  count(global_k);
    size_t                  count(global_k, const char*, arguments_k);
    size_t                  count(global_k, const char*, results_k);
    size_t                  count(module_k);
    
    const char*             description(global_k, const char*);
    const char*             description(global_k, const char*, argument_k, size_t);
    const char*             description(global_k, const char*, result_k, size_t);
    const char*             description(module_k, const char*);

    const char*             help(global_k, const char*);
    const char*             help(global_k, const char*, argument_k, size_t);
    const char*             help(global_k, const char*, result_k, size_t);
    const char*             help(module_k, const char*);

    const char*             key(global_k, const char*, argument_k, size_t);
    const char*             key(global_k, const char*, result_k, size_t);

    const cstr_xset_t&      keys(global_k, all_k);
    const cstr_xset_t&      keys(module_k, all_k);


    //////////////////////////////////////////////////////////////////////////////
    //  HELP WRITER


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
        if(!reg)
            return *this;
        
        Value vv;
        vv.key          = std::string(k);
        vv.description  = description;
        vv.help         = help;
        reg -> values.push_back(vv);
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////////
    //  GENERAL/OTHER/MISC (May be spun off...later)


    LuaVM* vm(lua_State* l)
    {
        void_ptr_x  x = voidptr(l, GLOBAL, keyVM);
        if(!x)
            return nullptr;
        return (LuaVM*) *x;
    }


}
#endif
