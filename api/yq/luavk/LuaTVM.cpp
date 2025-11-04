////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "LuaTVM.hpp"

#include <yq/lua/lualua.hpp>
#include <yq/lua/lualua.hxx>
#include <yq/tachyon/api/TachyonMetaWriter.hpp>
#include <yq/luavk/command/ExecuteFileCommand.hpp>
#include <yq/luavk/command/ExecuteStringCommand.hpp>
#include <yq/luavk/event/ExecuteFileEvent.hpp>
#include <yq/luavk/event/ExecuteStringEvent.hpp>

YQ_TACHYON_IMPLEMENT(yq::lua::LuaTVM)

namespace yq::lua {
    LuaTVM::LuaTVM()
    {
    }
    
    LuaTVM::~LuaTVM()
    {
    }

    void    LuaTVM::on_exec_file(const ExecuteFileCommand& cmd)
    {
        auto ec = m_lua.execfile(cmd.file());
        send(new ExecuteFileEvent({.cause=&cmd}, cmd.file(), ec));
        send_output();
    }

    void    LuaTVM::on_exec_string(const ExecuteStringCommand& cmd)
    {
        auto ec = m_lua.execute(cmd.text());
        send(new ExecuteStringEvent({.cause=&cmd}, cmd.text(), ec));
        send_output();
    }

    void    LuaTVM::send_output()
    {
    }

    tachyon::Execution   LuaTVM::setup(const tachyon::Context&ctx) 
    {
        if(m_init)
            return tachyon::Tachyon::setup(ctx);
        
        //  TODO
        
        return tachyon::Tachyon::setup(ctx); 
    }
    
    tachyon::Execution   LuaTVM::tick(const tachyon::Context&ctx) 
    {
        return tachyon::Tachyon::tick(ctx);
    }
    
    tachyon::Execution   LuaTVM::teardown(const tachyon::Context&ctx) 
    {
        if(!m_init)
            return tachyon::Tachyon::teardown(ctx);
    
        // TODO
        
        return tachyon::Tachyon::teardown(ctx);
    }
        
    void LuaTVM::init_meta()
    {
        auto w = writer<LuaTVM>();
        w.description("Lua Tachyon-Based Virtual Machine");
        w.slot(&LuaTVM::on_exec_file);
        w.slot(&LuaTVM::on_exec_string);
    }
}
