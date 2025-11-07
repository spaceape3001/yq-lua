////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/lua/extract.hpp>
#include <yq/lua/push.hpp>
#include <yq/lua/set.hpp>

#if 0
namespace yq::lua {


    //////////////////////////////////////////////////////////////////////////////
    //  Pushing (onto the lua stack), or setting

    

    //////////////////////////////////////////////////////////////////////////////
    //  HELP

    size_t                  count(global_k);
    size_t                  count(global_k, const char*, arguments_k);
    size_t                  count(global_k, const char*, results_k);
    size_t                  count(module_k);
    
    const char*             description(global_k, const char*);
    const char*             description(global_k, const char*, argument_k, size_t);
    const char*             description(global_k, const char*, result_k, size_t);
    const char*             description(module_k, const char*);

    const char*             help(global_k, const char*);
    const char*             help(global_k, const char*, argument_k, size_t);
    const char*             help(global_k, const char*, result_k, size_t);
    const char*             help(module_k, const char*);

    const char*             key(global_k, const char*, argument_k, size_t);
    const char*             key(global_k, const char*, result_k, size_t);

    const cstr_xset_t&      keys(global_k, all_k);
    const cstr_xset_t&      keys(module_k, all_k);

    //////////////////////////////////////////////////////////////////////////////
    //  REGISTRATION


    class HelpWriter {
    public:
    
        HelpWriter(Help*);
        
        HelpWriter  description(const char*);
        HelpWriter  text(const char*);
        
        HelpWriter   argument(const char*, const ObjectMeta&, const char* desc=nullptr, const char* text=nullptr);
        HelpWriter   argument(const char*, const TypeMeta&, const char* desc=nullptr, const char* text=nullptr);
        HelpWriter   argument(const char*, LuaType, const char* desc=nullptr, const char* text=nullptr);
        HelpWriter   argument(const char*, const char* desc=nullptr, const char* text=nullptr);

        HelpWriter   result(std::string_view, const ObjectMeta&, const char* desc=nullptr, const char* text=nullptr);
        HelpWriter   result(std::string_view, const TypeMeta&, const char* desc=nullptr, const char* text=nullptr);
        HelpWriter   result(std::string_view, LuaType, const char* desc=nullptr, const char* text=nullptr);
        HelpWriter   result(std::string_view, const char* desc=nullptr, const char* text=nullptr);

    private:
        Help*       m_help;
    };


    //! Registers the global w/o the contents... it's expected to be filled in by
    //! the specific VM (ie, a window or context pointer, an upvalue, etc)

    // Set all KNOWN valid globals into the given lua state machine
    void                    inject(lua_State*, all_k);

    //////////////////////////////////////////////////////////////////////////////
    //  GENERAL/OTHER/MISC (May be spun off...later)

    bool                    has_global(lua_State*, const char*);

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
#endif
