////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/lua/types.hpp>
#include <yq/meta/Meta.hpp>

namespace yq::lua {
    class Repo : public Meta {
    public:
    
        static const Repo&    instance();
        
        struct Helper;

    private:
        Repo();
        ~Repo();
    };
}
