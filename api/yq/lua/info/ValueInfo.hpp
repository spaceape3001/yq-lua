////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/lua/info/Info.hpp>

namespace yq::lua {
    class ValueInfo : public Info {
    public:
    
        const type_t&   type() const { return m_type; }
        const value_t&  value() const { return m_value; }
        
    protected:
        
        ValueInfo();
        virtual ~ValueInfo();
        
    private:
        friend class Repo;
        
        type_t      m_type;
        value_t     m_value;
    };
}
