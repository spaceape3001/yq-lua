////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <system_error>
#include <yq/keywords.hpp>
#include <yq/typedef/expected.hpp>
#include <variant>

struct lua_State;

namespace yq {
    class LuaVM;
    class Any;
    class Object;
    class ObjectMeta;
}

namespace yq::lua {

    static constexpr const char*        VM          = "_vm";
    static constexpr const char*        NAME        = "_name";
    static constexpr const char*        STEM        = "_stem";
    static constexpr const char*        DESCRIPTION = "_description";

    static constexpr const size_t       MAX_UPVALUES    = 255;  // hardcoded in Lua

    typedef int (*FNLuaCallback)(lua_State*);
    
    template <typename ... Args> struct LuaCallbackUpvalued {
        typedef int (*FN)(lua_State*, Args...);
    };
    
    template <typename ... Args>
    using FNLuaUpvalueHandler = LuaCallbackUpvalued<Args...>::FN;
    
    /*
        LUA helpers in this file
        
        
        Stack... note, number is as pushed on the stack, so use negatives to get 
        the last item pushed (ie -1)
    */

    boolean_x           boolean(lua_State*, global_k, const char*);
    boolean_x           boolean(lua_State*, int);
    
    double_x            double_(lua_State*, global_k, const char*);
    double_x            double_(lua_State*, int);

    //! Converts lua error code into a std::error_code
    std::error_code     errored(int);
    
    
    integer_x           integer(lua_State*, global_k, const char*);
    integer_x           integer(lua_State*, int);
    
    Object*             object(lua_State*, int);
    const Object*       object(lua_State*, int, const_k);

    Object*             object(lua_State*, int, const ObjectMeta&);
    const Object*       object(lua_State*, int, const ObjectMeta&, const_k);
    
    // defined in lualua.hxx
    template <class Obj>
    Obj*                object_as(lua_State*, int);

    // defined in lualua.hxx
    template <class Obj>
    const Obj*          object_as(lua_State*, int, const_k);

    //! Returns user data as a pointer (may or may not be lightweight)
    void_ptr_x          pointer(lua_State*, global_k, const char*);

    //! Returns user data as a pointer (may or may not be lightweight)
    void_ptr_x          pointer(lua_State*, int);
    
    std::error_code     push(lua_State*, const Any&);
    std::error_code     push(lua_State*, bool);
    std::error_code     push(lua_State*, double);
    std::error_code     push(lua_State*, int64_t);
    std::error_code     push(lua_State*, const_k, const Object*);
    std::error_code     push(lua_State*, Object*);
    std::error_code     push(lua_State*, std::nullptr_t);
    std::error_code     push(lua_State*, std::string_view);
    std::error_code     push(lua_State*, void*);
    
    std::error_code     push(lua_State*, FNLuaCallback);
    std::error_code     push(lua_State*, FNLuaCallback, size_t);

    std::error_code     set(lua_State*, global_k, const char*, bool);
    std::error_code     set(lua_State*, global_k, const char*, double);
    std::error_code     set(lua_State*, global_k, const char*, int64_t);
    std::error_code     set(lua_State*, global_k, const char*, std::string_view);
    std::error_code     set(lua_State*, global_k, const char*, void*);

    //! Global as a string... 
    string_x            string(lua_State*, global_k, const char*);

    //! Stack value as a string... 
    //! \note This will convert said value to a STRING
    string_view_x       string(lua_State*,  int);

    //! Generic type string to a lua type ID (don't use with the meta)
    std::string_view    typestring(int);

    any_x               value(lua_State*, int);

    LuaVM*              vm(lua_State*);

    //! Returns LIGHTWEIGHT user data (only)
    void_ptr_x          voidptr(lua_State*, int);

    //! Returns LIGHTWEIGHT user data (only)
    void_ptr_x          voidptr(lua_State*, global_k, const char*);
}
