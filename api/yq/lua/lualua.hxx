////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/errors.hpp>
#include <yq/keywords.hpp>
#include <yq/core/Any.hpp>
#include <yq/core/Object.hpp>
#include <yq/core/Ref.hpp>
#include <yq/lua/errors.hpp>
#include <yq/lua/Extractor.hpp>
#include <yq/lua/keywords.hpp>
#include <yq/lua/lualua.hpp>
#include <lua.hpp>

namespace yq::lua {
    template <typename T>
    auto  extract(lua_State* l, int n)
    {
        return Extractor<T>::get(l, n);
    }

    template <class Obj>
    Obj*                object_as(lua_State*l, int n)
    {
        return static_cast<const Obj*>(object(l, n, meta<Obj>()));
    }

    // defined in lualua.hxx
    template <class Obj>
    const Obj*          object_as(lua_State*l, int n, const_k)
    {
        return static_cast<const Obj*>(object(l, n, meta<Obj>(), CONST));
    }


    template <typename A, typename ... Args>
    std::error_code     push(lua_State*l, all_k, A arg, Args... args)
    {
        if(!l)
            return errors::lua_null();
        
        push(l, arg);
        if constexpr (sizeof...(args) > 0){
            push(l, ALL, args...);
        }
        return {};
    }
    
    template <typename ... Args>
    std::error_code     push(lua_State*l, FNLuaCallback fn, upvalues_k, Args... args)
    {
        static_assert(sizeof...(args) <= MAX_UPVALUES, "Lua cannot take more upvalues");

        if(!l)
            return errors::lua_null();
        if(!fn)
            return errors::null_pointer();
        
        push(l, ALL, args...);
        lua_pushcclosure(l, fn, (size_t) sizeof...(args));
        return {};
    }
    
    
    

    std::error_code     fn_register(lua_State*, global_k, const char*, FNLuaCallback);
    std::error_code     fn_register(lua_State*, global_k, const char*, FNLuaCallback, size_t);
    
    template <typename ... Args>
    std::error_code     fn_register(lua_State* l, global_k, const char*key, FNLuaCallback fn, upvalues_k, Args... args)
    {
        if(!l)
            return errors::lua_null();
        if(!fn)
            return errors::null_pointer();
        if(!key)
            return errors::null_pointer();
        if(!*key)
            return errors::bad_argument();

        std::error_code ec = push(l, fn, UPVALUES, args...);
        if(ec != std::error_code())
            return ec;
        lua_setglobal(l, key);
        return {};
    }


}
