////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/luavk/event/ExecuteFileEvent.hpp>
#include <yq/tachyon/api/EventMetaWriter.hpp>

YQ_OBJECT_IMPLEMENT(yq::lua::ExecuteFileEvent)

namespace yq::lua {

    ExecuteFileEvent::ExecuteFileEvent(const Header&h, const std::filesystem::path& v, const std::error_code& ec) : 
        LuaEvent(h), m_file(v), m_error(ec)
    {
    }

    ExecuteFileEvent::ExecuteFileEvent(const ExecuteFileEvent& cp, const Header& h) : 
        LuaEvent(cp, h), m_file(cp.m_file), m_error(cp.m_error)
    {
    }
    
    ExecuteFileEvent::~ExecuteFileEvent()
    {
    }

    tachyon::PostCPtr    ExecuteFileEvent::clone(rebind_k, const Header&h) const 
    {
        return new ExecuteFileEvent(*this, h);
    }
    
    ////////////////////////////////////////////////////////////////////////////

    void ExecuteFileEvent::init_meta()
    {
        auto w = writer<ExecuteFileEvent>();
        w.description("Lua Execute File Event");
        //w.property("file", &ExecuteFileEvent::m_file);  // TODO (file as an any)
    }
}
