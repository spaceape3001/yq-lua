////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/luavk/event/LuaEvent.hpp>

namespace yq::lua {
    class ExecuteStringEvent : public LuaEvent {
        YQ_OBJECT_DECLARE(ExecuteStringEvent, LuaEvent)
    public:
        ExecuteStringEvent(const Header&, std::string_view, const std::error_code&);
        virtual tachyon::PostCPtr    clone(rebind_k, const Header&) const override;

        static void init_meta();
        
        const std::string&      text() const { return m_text; }
        const std::error_code&  error() const { return m_error; }

    protected:
        ExecuteStringEvent(const ExecuteStringEvent&, const Header&);
        virtual ~ExecuteStringEvent();
        
    private:

        std::string         m_text;
        std::error_code     m_error;

        ExecuteStringEvent(const ExecuteStringEvent&) = delete;
        ExecuteStringEvent(ExecuteStringEvent&&) = delete;
        ExecuteStringEvent& operator=(const ExecuteStringEvent&) = delete;
        ExecuteStringEvent& operator=(ExecuteStringEvent&&) = delete;
    };
}
