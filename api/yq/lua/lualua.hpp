////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/lua/extract.hpp>
#include <yq/lua/info.hpp>
#include <yq/lua/push.hpp>
#include <yq/lua/register.hpp>
#include <yq/lua/set.hpp>

namespace yq::lua {
    // Set all KNOWN valid globals into the given lua state machine
    void    configure(lua_State*);

    bool    has(lua_State* l, global_k, const char* key);

    // LuaVM from the lua state (could be null if unavailble)
    LuaVM*  vm(lua_State*);
    

#if 0

    //! Registers the global w/o the contents... it's expected to be filled in by
    //! the specific VM (ie, a window or context pointer, an upvalue, etc)


    struct WriteOptions {
        Stream*     stream  = nullptr;
        
        //! Writes out the full table (if there)
        bool        table   = false;
    };
    
    //! Writes the specified value to the stream
    void                write(lua_State*, int, const WriteOptions& options={});
#endif
}
