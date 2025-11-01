////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/tachyon/api/Widget.hpp>

using namespace yq;
//using namespace yq::lua;
using namespace yq::tachyon;

class LuaWin : public Widget {
    YQ_TACHYON_DECLARE(LuaWin, Widget)
public:

    enum class FileMode {
        None
    };
    
    FileMode                    m_fileMode = FileMode::None;
    yq::tachyon::TachyonID      m_lua;
    
    LuaWin(TachyonID luavm);
    
    ~LuaWin();

    void    imgui(ViContext&u) ;
    
    static void init_meta();
};
