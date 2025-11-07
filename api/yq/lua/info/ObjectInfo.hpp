////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/lua/info/Info.hpp>

namespace yq::lua {
    class ObjectInfo : public Info {
    public:
    
    protected:
        ObjectInfo(const ObjectMeta&);
        virtual ~ObjectInfo();
    
    private:
        const ObjectMeta&   m_meta;
    };

}
