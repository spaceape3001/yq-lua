////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/lua/lualua.hpp>
#include <yq/core/Any.hpp>
#include <yq/core/Object.hpp>
#include <yq/core/Ref.hpp>
#include <lua.hpp>

namespace yq::lua {
    std::error_code     fn_register(lua_State*, global_k, const char*, FNLuaCallback);
}
