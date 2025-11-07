////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/lua/info/Info.hpp>

namespace yq::lua {
    class TypeInfo : public Info {
    public:
    
    protected:
        TypeInfo(const TypeMeta&);
        virtual ~TypeInfo();
    
    private:
    
        const TypeMeta& m_meta;
    };

}
