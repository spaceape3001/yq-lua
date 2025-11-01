////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "LuaTVM.hpp"
#include <yq/tachyon/api/TachyonMetaWriter.hpp>

YQ_TACHYON_IMPLEMENT(yq::lua::LuaTVM)

namespace yq::lua {
    LuaTVM::LuaTVM()
    {
    }
    
    LuaTVM::~LuaTVM()
    {
    }
        
    void LuaTVM::init_meta()
    {
        auto w = writer<LuaTVM>();
        w.description("Lua Tachyon-Based Virtual Machine");
    }
}
