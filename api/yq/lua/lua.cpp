////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

    /*
        CONVENTIONS
        
        Want something like ... (could be a global routine)
        geo = geodetic( 50, 75, 10 )
        geo = geodetic( "W45 N36 7000ft" )
        
        
    */

#include "extract.ipp"
#include "handler.ipp"
#include "impl.ipp"
#include "info.ipp"
#include "push.ipp"
#include "register.ipp"
#include "repo.ipp"
#include "set.ipp"

#include "info/ArgumentInfo.ipp"
#include "info/FunctionInfo.ipp"
#include "info/Info.ipp"
#include "info/ModuleInfo.ipp"
#include "info/ObjectInfo.ipp"
#include "info/TypeInfo.ipp"
#include "info/ValueInfo.ipp"

namespace yq::lua {
    static void cfg_globals(lua_State*l)
    {
        const ModuleInfo*   x   = info(GLOBAL);
        if(!x)
            return ;
        
        InstallInfoAPI  api{ .lvm=l };
        for(const auto& i : x->components())
            i.second->install(api);
    }

    void    configure(lua_State*l)
    {
        if(!l)
            return;
        cfg_globals(l);
    }


    bool    has(lua_State* l, global_k, const char* key)
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

    LuaVM* vm(lua_State* l)
    {
        void_ptr_x  x = voidptr(l, GLOBAL, keyVM);
        if(!x)
            return nullptr;
        return (LuaVM*) *x;
    }
}

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
#endif
