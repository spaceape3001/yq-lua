////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ObjectInfo.hpp"
#include <yq/meta/ObjectMeta.hpp>

namespace yq::lua {
    ObjectInfo::ObjectInfo(const ObjectMeta& om) : Info(om.name().data()), m_meta(om)
    {
    }
    
    ObjectInfo::~ObjectInfo() = default;
}
