////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/luavk/command/LuaEvent.hpp>

namespace yq::lua {
    class LuaExecStringEvent : public LuaEvent {
        YQ_OBJECT_DECLARE(LuaExecStringEvent, LuaEvent)
    public:
        LuaExecStringEvent(const Header&, std::string_view);
        virtual tachyon::PostCPtr    clone(rebind_k, const Header&) const override;

        static void init_meta();
        
        const std::string&  text() const { return m_text; }

    protected:
        LuaExecStringEvent(const LuaExecStringEvent&, const Header&);
        virtual ~LuaExecStringEvent();
        
    private:

        std::string         m_text;

        LuaExecStringEvent(const LuaExecStringEvent&) = delete;
        LuaExecStringEvent(LuaExecStringEvent&&) = delete;
        LuaExecStringEvent& operator=(const LuaExecStringEvent&) = delete;
        LuaExecStringEvent& operator=(LuaExecStringEvent&&) = delete;
    };
}
