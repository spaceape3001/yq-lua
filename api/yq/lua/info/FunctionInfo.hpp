////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/lua/info/Info.hpp>

namespace yq::lua {
    class FunctionInfo : public Info {
    public:
    
        const argument_info_vector_t& arguments() const { return m_arguments; }
        const argument_info_vector_t& results() const { return m_results; }
        const value_info_vector_t& upvalues() const { return m_upvalues; }

    protected:
        FunctionInfo(const char*);
        virtual ~FunctionInfo();
    
    private:
        argument_info_vector_t  m_arguments;
        argument_info_vector_t  m_results;
        value_info_vector_t     m_upvalues;
    };

}
