////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/lua/types.hpp>

namespace yq::lua {
    //const Help*     help(const char*);
    const Info*     info(global_k, const char*);
    const Info*     info(module_k, const char*);
    const Info*     info(const ObjectMeta&);
    const Info*     info(const TypeMeta&);
}
