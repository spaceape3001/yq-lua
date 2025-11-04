////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/luavk/command/LuaExecStringEvent.hpp>
#include <yq/tachyon/api/EventMetaWriter.hpp>

YQ_OBJECT_IMPLEMENT(yq::lua::LuaExecStringEvent)

namespace yq::lua {

    LuaExecStringEvent::LuaExecStringEvent(const Header&h, std::string_view v) : LuaEvent(h), m_text(v)
    {
    }

    LuaExecStringEvent::LuaExecStringEvent(const LuaExecStringEvent& cp, const Header& h) : 
        LuaEvent(cp, h), m_text(cp.m_text)
    {
    }
    
    LuaExecStringEvent::~LuaExecStringEvent()
    {
    }

    tachyon::PostCPtr    LuaExecStringEvent::clone(rebind_k, const Header&h) const 
    {
        return new LuaExecStringEvent(*this, h);
    }
    
    ////////////////////////////////////////////////////////////////////////////

    void LuaExecStringEvent::init_meta()
    {
        auto w = writer<LuaExecStringEvent>();
        w.description("LuaExecString Event");
        w.property("text", &LuaExecStringEvent::m_text);
    }
}
