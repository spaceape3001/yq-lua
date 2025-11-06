////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq::lua {
    struct table_k {};
    struct upvalue_k {};
    struct upvalues_k {};
    
    static const constexpr table_k      TABLE;
    static const constexpr upvalue_k    UPVALUE;
    static const constexpr upvalues_k   UPVALUES;
}
