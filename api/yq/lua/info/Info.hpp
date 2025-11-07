////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/lua/types.hpp>

namespace yq::lua {
    class Info {
    public:
    
        const char*     brief() const { return m_brief; }
        const char*     key() const { return m_key; }
        const char*     help() const { return m_help; }
    
        //! Sets the brief
        //! \note Provided pointer must *LIVE* throughout the program lifetime (ie, it's not copied)
        void            brief(const char*);
        
        //! Sets the text
        //! \note Provided pointer must *LIVE* throughout the program lifetime (ie, it's not copied)
        void            help(const char*);
    
    protected:
        Info(const char*z=nullptr);
        virtual ~Info();
    
    private:
        friend class Repo;
        
        const char* m_key   = nullptr;
        const char* m_brief = nullptr;
        const char* m_help  = nullptr;
    };
}
