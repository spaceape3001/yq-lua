////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <system_error>
#include <yq/keywords.hpp>
#include <yq/core/Flags.hpp>
#include <yq/lua/keywords.hpp>
#include <yq/typedef/expected.hpp>
#include <yq/typedef/string_vectors.hpp>
#include <variant>
#include <lua.hpp>

struct lua_State;

namespace yq {
    class LuaVM;
    class Any;
    class Refable;
    class Object;
    class ObjectMeta;
    class Meta;
    class TypeMeta;
    class Stream;
}

namespace yq::lua {

    //  standard keys for internals
    static constexpr const char* keyVM                  = "_vm";
    static constexpr const char* keyName                = "_name";
    static constexpr const char* keyStem                = "_stem";
    static constexpr const char* keyDescription         = "_desc";
    static constexpr const char* keyMeta                = "_meta";
    static constexpr const char* keyPointer             = "_ptr";
    static constexpr const char* keyFlags               = "_flags";
    static constexpr const char* keyGarbageCollection   = "__gc"; // LUA's
    

    static constexpr const int       MAX_UPVALUES    = 255;  // hardcoded in Lua

    enum class X : uint8_t {
        Const,
        Delete,
        Ref
    };
    using XFlags    = Flags<X, int32_t>;

    typedef int (*FNLuaCallback)(lua_State*);
    
    template <typename ... Args> struct LuaCallbackUpvalued {
        typedef int (*FN)(lua_State*, Args...);
    };
    
    template <typename ... Args>
    using FNLuaUpvalueHandler = LuaCallbackUpvalued<Args...>::FN;


    //////////////////////////////////////////////////////////////////////////////
    //  Micro helpers, these *ASSUME* things (like valid pointers, in range, etc)
    //  Technically more "impl" than external, simply going to be reused so exposed.

    //! Converts lua error code into a std::error_code
    std::error_code     _error(int);
    

    XFlags              _flags(lua_State *l, int n);

    //

    bool                _gc(lua_State*, int, object_k);
    
    //! Assumes the pointer of the meta field on the given table is a valid meta thing
    
    /*! Assumes the given stack value is a table representing a complex thing (ie, object, any), 
        and extracts the meta pointer from it.  
    */
    const Meta*         _meta(lua_State*l, int n);
    
    /*! Assumes the given stack item is a table representing a complex thing (ie, object), 
        and extracts the meta pointer from it.  
    */
    const ObjectMeta*   _meta(lua_State*l, int n, object_k);

    /*! Assumes the given stack item is a table representing a complex thing (ie, any), 
        and extracts the meta pointer from it.  
    */
    const TypeMeta*     _meta(lua_State*l, int n, type_k);

    // Creates a meta table
    void                _metamake(lua_State*, const ObjectMeta&);
    
    Object*             _object(lua_State *l, int n);
    
    inline void         _pop(lua_State* l)
    {
        lua_pop(l, 1);
    }

    std::error_code     _push(lua_State* l, Object* obj, XFlags flags);

    //! Generic type string to a lua type ID (don't use with the meta)
    std::string_view    _type(int);
    

    //////////////////////////////////////////////////////////////////////////////
    //  Extracting
    //
    //  Usage
    //      (type/item) (luaState*, int)  
    //
    //          Gets the item from the stack with negatives used to get the last (n) 
    //          item pushed.   (ie, -1 to get top of stack).
    //
    //      (type/item) (luaState*, GLOBAL, const char*)
    //
    //          Gets the specified global using the specified key
    //
    //      (type/item) (luaState*, UPVALUE, int)
    //
    //          Gets the specified upvalue (ie, bound at registration), where 1 <= n <= 255
    
 
    boolean_x               boolean(lua_State*, int);
    boolean_x               boolean(lua_State*, global_k, const char*);
    boolean_x               boolean(lua_State*, upvalue_k, int);
    
    double_x                double_(lua_State*, int);
    double_x                double_(lua_State*, global_k, const char*);
    double_x                double_(lua_State*, upvalue_k, int);
    
    integer_x               integer(lua_State*, int);
    integer_x               integer(lua_State*, global_k, const char*);
    integer_x               integer(lua_State*, upvalue_k, int);
    
    object_ptr_x            object(lua_State*, int);
    object_cptr_x           object(lua_State*, int, const_k);
    object_ptr_x            object(lua_State*, int, const ObjectMeta&);
    object_cptr_x           object(lua_State*, int, const ObjectMeta&, const_k);

    object_ptr_x            object(lua_State*, global_k, const char*);
    object_cptr_x           object(lua_State*, global_k, const char*, const_k);
    object_ptr_x            object(lua_State*, global_k, const char*, const ObjectMeta&);
    object_cptr_x           object(lua_State*, global_k, const char*, const ObjectMeta&, const_k);

    object_ptr_x            object(lua_State*, upvalue_k, int);
    object_cptr_x           object(lua_State*, upvalue_k, int, const_k);
    object_ptr_x            object(lua_State*, upvalue_k, int, const ObjectMeta&);
    object_cptr_x           object(lua_State*, upvalue_k, int, const ObjectMeta&, const_k);
    
    // defined in lualua.hxx
    template <class Obj>
    Expect<Obj*>            object_as(lua_State*, int);

    // defined in lualua.hxx
    template <class Obj>
    Expect<const Obj*>      object_as(lua_State*, int, const_k);

    // defined in lualua.hxx
    template <class Obj>
    Expect<Obj*>            object_as(lua_State*, global_k, const char*);

    // defined in lualua.hxx
    template <class Obj>
    Expect<const Obj*>      object_as(lua_State*, global_k, const char*, const_k);
    
    // defined in lualua.hxx
    template <class Obj>
    Expect<Obj*>            object_as(lua_State*, upvalue_k, int);

    // defined in lualua.hxx
    template <class Obj>
    Expect<const Obj*>      object_as(lua_State*, upvalue_k, int, const_k);

    //! Returns user data as a pointer (may or may not be lightweight)
    void_ptr_x              pointer(lua_State*, int);

    //! Returns user data as a pointer (may or may not be lightweight)
    void_ptr_x              pointer(lua_State*, global_k, const char*);

    //! Returns user data as a pointer (may or may not be lightweight)
    void_ptr_x              pointer(lua_State*, upvalue_k, int);
    


    //! Stack value as a string... 
    //! \note This will convert said value to a STRING
    string_view_x           string(lua_State*,  int);

    //! Global as a string... 
    string_x                string(lua_State*, global_k, const char*);

    //! Stack value as a string... 
    //! \note This will convert said value to a STRING
    string_view_x           string(lua_State*,  upvalue_k, int);

    any_x                   value(lua_State*, int);
    any_x                   value(lua_State*, upvalue_k, int);


    //! Returns LIGHTWEIGHT user data (only)
    void_ptr_x              voidptr(lua_State*, int);

    //! Returns LIGHTWEIGHT user data (only)
    void_ptr_x              voidptr(lua_State*, global_k, const char*);

    //! Returns LIGHTWEIGHT user data (only)
    void_ptr_x              voidptr(lua_State*, upvalue_k, int);

    //////////////////////////////////////////////////////////////////////////////
    //  Pushing (onto the lua stack), or setting

    std::error_code         push(lua_State*, const Any&);
    std::error_code         push(lua_State*, bool);
    std::error_code         push(lua_State*, double);
    std::error_code         push(lua_State*, int);
    std::error_code         push(lua_State*, const_k, const Object*);
    std::error_code         push(lua_State*, Object*);
    std::error_code         push(lua_State*, std::nullptr_t);
    std::error_code         push(lua_State*, std::string_view);
    std::error_code         push(lua_State*, void*);
    
    std::error_code         push(lua_State*, FNLuaCallback);
    std::error_code         push(lua_State*, FNLuaCallback, size_t);

    std::error_code         set(lua_State*, int, table_k, const char*, bool);
    std::error_code         set(lua_State*, int, table_k, const char*, double);
    std::error_code         set(lua_State*, int, table_k, const char*, int);
    std::error_code         set(lua_State*, int, table_k, const char*, std::string_view);
    std::error_code         set(lua_State*, int, table_k, const char*, const_k, const Object*);
    std::error_code         set(lua_State*, int, table_k, const char*, Object*);
    std::error_code         set(lua_State*, int, table_k, const char*, void*);
    std::error_code         set(lua_State*, int, table_k, const char*, FNLuaCallback, size_t n=0);

    std::error_code         set(lua_State*, global_k, const char*, bool);
    std::error_code         set(lua_State*, global_k, const char*, double);
    std::error_code         set(lua_State*, global_k, const char*, int);
    std::error_code         set(lua_State*, global_k, const char*, std::string_view);
    std::error_code         set(lua_State*, global_k, const char*, void*);
    std::error_code         set(lua_State*, global_k, const char*, FNLuaCallback, size_t n=0);

    
    //////////////////////////////////////////////////////////////////////////////
    //  Lua Handlers (can use these....)
    //
    //  NOTE: Upvalue parameters (ie pointers) are expected to remain VALID throughout
    //  the usage of the specific Lua VM.  
    
    //! Deletes the items in the stack
    int lh_gc_object(lua_State*);
    
    //! Writes to the specified stream
    //! \tparam stream Pointer to stream
    //! \param vaargs things to write
    int lh_write_stream(lua_State*);  
    
    int lh_write_cerr(lua_State*);
    int lh_write_cout(lua_State*);

    //////////////////////////////////////////////////////////////////////////////
    //  Registration

    struct Reg;

    struct RegWriter {
        Reg*        reg = nullptr;

        RegWriter&  description(std::string_view);
        RegWriter&  help(std::string_view);
        
        RegWriter&  arg(std::string_view, std::string_view description={}, std::string_view help={});
    };

    //! Registers the global w/o the contents... it's expected to be filled in by
    //! the specific VM (ie, a window or context pointer, an upvalue, etc)
    RegWriter           reg(global_k, const char*);

    RegWriter           reg(global_k, const char*, bool);
    RegWriter           reg(global_k, const char*, int);
    RegWriter           reg(global_k, const char*, double);
    RegWriter           reg(global_k, const char*, void*);
    RegWriter           reg(global_k, const char*, Object*);
    RegWriter           reg(global_k, const char*, std::string_view);
    RegWriter           reg(global_k, const char*, const Object*);
    RegWriter           reg(global_k, const char*, FNLuaCallback);

    // Set all KNOWN valid globals into the given lua state machine
    void                inject(lua_State*, global_k, all_k);

    bool                has_global(lua_State*, const char*);

    // meant for a help system...

    string_view_vector_t    global_keys();
    int                     global_argcount(std::string_view);
    std::string_view        global_argkey(std::string_view, int);
    std::string_view        global_argdesc(std::string_view, int);
    std::string_view        global_arghelp(std::string_view, int);
    std::string_view        global_description(std::string_view);
    std::string_view        global_help(std::string_view);

    //////////////////////////////////////////////////////////////////////////////
    //  GENERAL/OTHER/MISC (May be spun off...later)

    // LuaVM from the lua state (could be null if unavailble)
    LuaVM*                  vm(lua_State*);
    
    struct WriteOptions {
        Stream*     stream  = nullptr;
        
        //! Writes out the full table (if there)
        bool        table   = false;
    };
    
    //! Writes the specified value to the stream
    void                write(lua_State*, int, const WriteOptions& options={});
}
