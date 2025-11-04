////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/luavk/event/LuaEvent.hpp>
#include <filesystem>

namespace yq::lua {
    class ExecuteFileEvent : public LuaEvent {
        YQ_OBJECT_DECLARE(ExecuteFileEvent, LuaEvent)
    public:
        ExecuteFileEvent(const Header&, const std::filesystem::path&, const std::error_code&);
        virtual tachyon::PostCPtr    clone(rebind_k, const Header&) const override;

        static void init_meta();
        
        const std::filesystem::path&    file() const { return m_file; }
        const std::error_code&          error() const { return m_error; }

    protected:
        ExecuteFileEvent(const ExecuteFileEvent&, const Header&);
        virtual ~ExecuteFileEvent();
        
    private:

        std::filesystem::path   m_file;
        std::error_code         m_error;

        ExecuteFileEvent(const ExecuteFileEvent&) = delete;
        ExecuteFileEvent(ExecuteFileEvent&&) = delete;
        ExecuteFileEvent& operator=(const ExecuteFileEvent&) = delete;
        ExecuteFileEvent& operator=(ExecuteFileEvent&&) = delete;
    };
}
