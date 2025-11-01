////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/lua/LuaVM.hpp>
#include <yq/tachyon/api/Tachyon.hpp>

namespace yq::lua {
    class LuaTVM : public tachyon::Tachyon {
        YQ_TACHYON_DECLARE(LuaTVM, tachyon::Tachyon)
    public:
        LuaTVM();
        ~LuaTVM();
        
        static void init_meta();
        
    private:
        LuaVM   m_lua;
    };
}
