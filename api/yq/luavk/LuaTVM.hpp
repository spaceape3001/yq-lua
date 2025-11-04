////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/lua/LuaVM.hpp>
#include <yq/tachyon/api/Tachyon.hpp>

namespace yq::lua {
    class ExecuteStringCommand;
    class ExecuteFileCommand;
    
    class LuaTVM : public tachyon::Tachyon {
        YQ_TACHYON_DECLARE(LuaTVM, tachyon::Tachyon)
    public:
        LuaTVM();
        ~LuaTVM();
        
        static void init_meta();
        
    protected:
        virtual tachyon::Execution   setup(const tachyon::Context&) override;
        virtual tachyon::Execution   tick(const tachyon::Context&) override;
        virtual tachyon::Execution   teardown(const tachyon::Context&) override;

    private:
        LuaVM      m_lua;
        bool       m_init       = false;
        
        void    on_exec_file(const ExecuteFileCommand&);
        void    on_exec_string(const ExecuteStringCommand&);
        
        void    send_output();
    };
}
