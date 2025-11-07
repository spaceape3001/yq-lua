////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/lua/handler.hpp>
#include <yq/lua/impl.hpp>

namespace yq::lua {
    int lh_gc_object(lua_State* l)
    {
        if(!_gc(l, 1, OBJECT))
            return 0;
        _pop(l);
        return -1;
    }

    int lh_write_cerr(lua_State*)
    {
        // TODO
        return 0;
    }
    
    int lh_write_cout(lua_State*)
    {
        // TODO
        return 0;
    }
    
    int lh_write_stream(lua_State*)
    {
        // TODO
        return 0;
    }
    
}
