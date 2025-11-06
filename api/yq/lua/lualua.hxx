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
#include <yq/lua/keywords.hpp>
#include <yq/lua/lualua.hpp>
#include <yq/meta/MetaBinder.hpp>
#include <lua.hpp>

namespace yq { class Object; }

namespace yq::lua {
    //////////////////////////////////////////////////////////////////////////////
    //  Extracting

    #if 0
        // TBD....
    template <typename T>
    auto  extract(lua_State* l, int n)
    {
        return Extractor<T>::get(l, n);
    }
    #endif

    template <class Obj>
    Expect<Obj*>        object_as(lua_State*l, int n)
    {
        auto x    = object(l, n, meta<Obj>());
        if(!x)
            return unexpected(x.error());
        return static_cast<const Obj*>(*x);
    }

    // defined in lualua.hxx
    template <class Obj>
    Expect<const Obj*>  object_as(lua_State*l, int n, const_k)
    {
        auto x    = object(l, n, meta<Obj>(), CONST);
        if(!x)
            return unexpected(x.error());
        return static_cast<const Obj*>(*x);
    }

    template <class Obj>
    Expect<Obj*>        object_as(lua_State*l, global_k, const char* key)
    {
        auto x    = object(l, GLOBAL, key, meta<Obj>());
        if(!x)
            return unexpected(x.error());
        return static_cast<const Obj*>(*x);
    }

    // defined in lualua.hxx
    template <class Obj>
    Expect<const Obj*>  object_as(lua_State*l, global_k, const char* key, const_k)
    {
        auto x    = object(l, GLOBAL, key, meta<Obj>(), CONST);
        if(!x)
            return unexpected(x.error());
        return static_cast<const Obj*>(*x);
    }

    template <class Obj>
    Expect<Obj*>        object_as(lua_State*l, upvalue_k, int n)
    {
        auto x    = object(l, UPVALUE, n, meta<Obj>());
        if(!x)
            return unexpected(x.error());
        return static_cast<const Obj*>(*x);
    }

    // defined in lualua.hxx
    template <class Obj>
    Expect<const Obj*>  object_as(lua_State*l, upvalue_k, int n, const_k)
    {
        auto x    = object(l, UPVALUE, n, meta<Obj>(), CONST);
        if(!x)
            return unexpected(x.error());
        return static_cast<const Obj*>(*x);
    }

    //////////////////////////////////////////////////////////////////////////////
    //  Pushing/Setting
    
    template <class Obj>
    requires (std::is_base_of_v<Refable, Obj> && std::is_base_of_v<Object, Obj>)
    std::error_code     push(lua_State*l, Ref<Obj> ptr)
    {
        return _push(l, ptr.ptr(), { X::Ref });
    }

    template <class Obj>
    requires (std::is_base_of_v<Refable, Obj> && std::is_base_of_v<Object, Obj>)
    std::error_code     push(lua_State*l, Ref<const Obj> ptr)
    {
        return _push(l, const_cast<Object*>(ptr.ptr()), { X::Const, X::Ref });
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
        static_assert(sizeof...(args) <= (size_t) MAX_UPVALUES, "Lua cannot take more upvalues");

        if(!l)
            return errors::lua_null();
        if(!fn)
            return errors::null_pointer();
        
        push(l, ALL, args...);
        lua_pushcclosure(l, fn, (size_t) sizeof...(args));
        return {};
    }
    
    
    template <typename ... Args>
    std::error_code     set(lua_State* l, global_k, const char*key, FNLuaCallback fn, upvalues_k, Args... args)
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

    template <typename ... Args>
    std::error_code     set(lua_State* l, int n, table_k, const char*key, FNLuaCallback fn, upvalues_k, Args... args)
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
