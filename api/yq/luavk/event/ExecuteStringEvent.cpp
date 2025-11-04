////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/luavk/event/ExecuteStringEvent.hpp>
#include <yq/tachyon/api/EventMetaWriter.hpp>

YQ_OBJECT_IMPLEMENT(yq::lua::ExecuteStringEvent)

namespace yq::lua {

    ExecuteStringEvent::ExecuteStringEvent(const Header&h, std::string_view v, const std::error_code&ec) : 
        LuaEvent(h), m_text(v), m_error(ec)
    {
    }

    ExecuteStringEvent::ExecuteStringEvent(const ExecuteStringEvent& cp, const Header& h) : 
        LuaEvent(cp, h), m_text(cp.m_text), m_error(cp.m_error)
    {
    }
    
    ExecuteStringEvent::~ExecuteStringEvent()
    {
    }

    tachyon::PostCPtr    ExecuteStringEvent::clone(rebind_k, const Header&h) const 
    {
        return new ExecuteStringEvent(*this, h);
    }
    
    ////////////////////////////////////////////////////////////////////////////

    void ExecuteStringEvent::init_meta()
    {
        auto w = writer<ExecuteStringEvent>();
        w.description("Lua Execute String Event");
        w.property("text", &ExecuteStringEvent::m_text);
    }
}
