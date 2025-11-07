////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/lua/types.hpp>

namespace yq::lua {
    Info*           reg(global_k, const char*, bool);
    Info*           reg(global_k, const char*, int);
    Info*           reg(global_k, const char*, double);
    Info*           reg(global_k, const char*, void*);
    Info*           reg(global_k, const char*, Object*);
    Info*           reg(global_k, const char*, std::string_view);
    Info*           reg(global_k, const char*, const Object*);
    
    //! Register a function (doesn't need upvalues)
    FunctionInfo*   reg(global_k, const char*, FNLuaCallback);

    //! Abstract function registration (will likely need upvalues, up to the VM implementer to define)
    FunctionInfo*   reg(global_k, const char*, function_k);

    //! Register a function to a meta (doesn't need upvalues)
    FunctionInfo*   reg(const ObjectMeta&, const char*, FNLuaCallback);

    //! Register a function to a meta (doesn't need upvalues)
    FunctionInfo*   reg(const TypeMeta&, const char*, FNLuaCallback);

    ModuleInfo*     reg(module_k, const char*);
}
