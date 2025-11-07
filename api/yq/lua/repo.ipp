////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "types.hpp"
#include "lualua.hpp"
#include <yq/meta/Meta.hpp>

namespace yq::lua {
    
    struct Help {
        const char*     key     = nullptr;
        std::string     full;
        std::string     desc;
        
        virtual ~Help(){}
        
        //virtual void push() ...
    };

    struct Argument : public Help {
        type_t          type;
        int             idx     = 0;
        
        virtual ~Argument(){}
    };

    struct Value : public Help {
        LuaType      type;
        value_t      value;
        virtual     ~Value(){}
    };

    struct Function : public Help {
        std::vector<Argument*>   arguments;
        std::vector<Argument*>   results;
        std::vector<Value*>      upvalues;
        virtual ~Function(){}
    };
    
    struct Class : public Help {
        virtual ~Class(){}
    };
    
    struct Module : public Help {
        virtual ~Module(){}
    };
    
    bool _push(lua_State* l, const value_t& val, int n=-1);
    
    
    class Repo : public Meta {
    public:
        std::unordered_map<id_t, Class*>            classes;
        std::map<const char*,const Help*,XCase>     globals;
        std::map<const char*,const Module*,XCase>   modules;
        
        Repo() : Meta("yq::lua::Repo") {}
        
        const Help*     global(const std::string& k) const
        {
            auto x = globals.find(k);
            if(x != globals.end())
                return x->second;
            return nullptr;
        }
        
        Value*   value(global_k, const char* k, bool create=false) 
        {
            auto x = globals.find(k);
            if(x != globals.end())
                return const_cast<Value*>(dynamic_cast<const Value*>(x->second));
            if(!Meta::thread_safe_write())  
                return nullptr;
            if(!create)
                return nullptr;
                
            Value*  v   = new Value;
            v->key  = k;
            globals[k]  = v;
            globalKeys.insert(k);
            return v;
        }

        Function*   function(global_k, const char* k, bool create=false)
        {
            auto x = globals.find(k);
            if(x != globals.end())
                return dynamic_cast<Function*>(x->second);
            if(!create)
                return nullptr;
            Function* v = new Function;
            v->key  = k;
            globals[k]  = v;
            globalKeys.insert(k);
            return v;
        }
        
        Module*     module_(global_k, const std::string& k, bool create=false)
        {
            auto x = modules.find(k);
            if(x != modules.end())
                return x->second;
            if(!create)
                return nullptr;
            Module* v   = new Module;
            v->key  = k;
            modules[k]  = v;
            moduleKeys.insert(k);
            return v;
        }
    };
    
    Repo&    _repo() 
    {
        static Repo s_repo;
        return s_repo;
    }
    
    const Repo& repo()
    {
        return _repo();
    }
}
