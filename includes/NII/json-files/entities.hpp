#pragma once
#pragma GCC diagnostic ignored "-Woverloaded-virtual"

#include <iostream>

#include <string>
#include <map>
#include <vector>
#include <utility>

#include "deserializator.hpp"

//#define NII_DEBUG

namespace nii
{
namespace json
{
    class json;
namespace entities
{
    class entity;
    class Object;
    class wrapper;
    class ObjectWrapper;
    class ArrayWrapper;
    class Array;

    class entity_handler
    {
        friend Object;
        friend Array;
        friend wrapper;
        friend ObjectWrapper;
        friend ArrayWrapper;


        
        
        public:     explicit entity_handler(entity*);
                    entity_handler();
                    entity_handler(entity_handler&&);
                    ~entity_handler();

        entity_handler(const entity_handler&) = delete;



    private:
        void setValue(int);
        void setValue(double);
        void setValue(const std::string&);
        void setValue(std::string&&);
        void setValue(bool);
        void setValue(entity *);
        void setValue();

        void addToArray(int);

        wrapper get(const std::string&);
        wrapper get(int);
        
        entity* extract();
        
    private:
        void destroyEntity();

        entity * _entity;
    };

    class entityInterface;

    struct wrapper
    {
        inline wrapper(entity_handler& handle) : handle(handle) {}
        inline ~wrapper() {}


        inline entityInterface* operator->();


        template<class _T>
        inline wrapper& operator=(_T&&elem) {
            handle.setValue(std::forward<_T>(elem));
            return *this;
        }

        template<class _T>
        inline wrapper& operator=(std::initializer_list<_T>&& list) {
            for(auto& elem : list) {
                handle.addToArray(elem);
            }
            return *this;
        }

        inline wrapper& operator=(const char* str) {
            handle.setValue(std::string(str));
            return *this;
        }

        inline wrapper& operator=(decltype(nullptr)) {
            handle.setValue();
            return *this;
        }

        inline wrapper operator[](const char* str) {
            return this->operator[](std::string(str));
        }

        inline wrapper operator[](const std::string& str) {
            return wrapper(handle.get(str));
        }

        inline wrapper operator[](int i) {
            return wrapper(handle.get(i));
        }

        template<class _T>
        inline operator _T() {
            if(handle._entity) {
                return static_cast<_T>(*handle._entity);
            } else {
                throw "J_WRAPPER: Can't cast from null";
            }
        }

        inline std::string serialize(); 

        inline bool valid()
        {
            return handle._entity;
        }

    protected:
        entity_handler& handle;
    };

    struct ObjectWrapper
    {
        inline ObjectWrapper(Object*obj)
            :obj(obj)
        {}

        inline auto begin();

        inline auto end();
        
        Object* obj;
    };

    struct ArrayWrapper
    {
        inline ArrayWrapper(Array*arr)
            :arr(arr)
        {}

        inline auto begin();

        inline auto end();
        
        Array* arr;
    };


    struct entityInterface
    {
        virtual std::string   string();
        virtual double        number();
        virtual bool          boolean();

        virtual ObjectWrapper object();
        virtual ArrayWrapper  array();

        virtual bool          isArray();
        virtual bool          isObject();

        virtual bool          isString();
        virtual bool          isNumber();
        virtual bool          isBoolean();
        virtual bool          isNull();


        virtual operator int();
        // virtual operator long();
        // virtual operator long long();
        // virtual operator unsigned();
        // virtual operator unsigned long();
        // virtual operator unsigned long long();
        
        virtual operator double();
        virtual operator bool();
        virtual operator std::string();

    protected:
        entityInterface() = default;
        virtual ~entityInterface() = default;
    };

    class entity : public entityInterface
    {
        friend wrapper;
        friend entity_handler;

    public:
        entity();
        virtual ~entity();

    protected:
        virtual void set(int);
        virtual void set(double);
        virtual void set(const std::string&);
        virtual void set(std::string&&);
        virtual void set(bool);

        virtual wrapper get(const std::string&);
        virtual wrapper get(int);


        public: virtual std::string serialize()=0;
    };

    entityInterface* wrapper::operator->() {
        if(handle._entity) {
            return handle._entity;
        } else {
            throw "J_WRAPPER: Entity is nullptr CAN'T operator ->";
        }
    }

    std::string wrapper::serialize() {
        if(handle._entity) {
            return handle._entity->serialize();
        } else {
            return "null";
        }
    }

    class Number : public entity
    {
        friend entity_handler;
    public:
        Number(double);
        ~Number();

        Number(const Number&) = delete;
        Number(Number&&) = delete;

    private:
        void set(int) override;
        void set(double) override;
        void set(bool) override;
        


        // Entity interface functions:
        double          number() override;
        std::string     string() override;

        bool            isNumber() override;

        operator int() override;
        operator double() override;
        operator bool() override;


        std::string serialize() override;

        double value;

        #ifdef NII_DEBUG
            public: inline static int GetCount() { return count; }
            inline static int count = 0;
        #endif
    };
    
    class Boolean : public entity
    {
        friend entity_handler;
    public:
        Boolean(bool);
        ~Boolean();

        Boolean(const Boolean&) = delete;
        Boolean(Boolean&&) = delete;
    private:
        void set(int) override;
        void set(bool) override;
        
        // Entity interface functions:
        bool            boolean() override;
        std::string     string() override;
        
        bool            isBoolean() override;

        operator int() override;
        operator bool() override;

        std::string serialize() override;

        bool value;

        #ifdef NII_DEBUG
            public: inline static int GetCount() { return count; }
            inline static int count = 0;
        #endif
    };

    class String : public entity
    {
        friend entity_handler;

    public: 
        String(const std::string&);
        String(std::string&&);
        ~String();

        String(const String&) = delete;
        String(String&&) = delete;
        
    private:
        void set(const std::string&) override;
        void set(std::string&&) override;


        // Entity interface functions:
        std::string     string() override;
        
        bool            isString() override;
        
        operator std::string() override;

        std::string serialize() override;

        std::string value;

        #ifdef NII_DEBUG
            public: inline static int GetCount() { return count; }
            inline static int count = 0;
        #endif
    };

    class Object : public entity
    {
        friend entity_handler;
        friend nii::json::json;

    public:
        Object();
        ~Object();
        Object(Object&&);

        Object(const Object&) = delete;

        Object & operator=(Object&&);

        Object & operator=(const Object&) = delete;

        struct iterator
        {
            inline iterator(std::map<std::string, entity_handler>::iterator iter)
                :iter(iter)
            {}

            inline std::pair<std::string, wrapper> operator*() {
                return std::make_pair(iter->first, wrapper(iter->second));
            }

            inline iterator& operator++() {
                ++iter;
                return *this;
            }

            inline bool operator!=(const iterator& other) {
                return this->iter != other.iter;
            }

            std::map<std::string, entity_handler>::iterator iter;
        };

        inline iterator begin() {
            return iterator(value.begin());
        }

        inline iterator end() {
            return iterator(value.end());
        }
        
    // private:
        wrapper get(const std::string& str) override;


        std::string serialize() override;

        ObjectWrapper object() override;
        bool isObject() override;


        std::map<std::string, entity_handler> value;

        #ifdef NII_DEBUG
            public: inline static int GetCount() { return count; }
            inline static int count = 0;
        #endif
    };

    auto ObjectWrapper::begin() {
        return obj->begin();
    }

    auto ObjectWrapper::end()  {
        return obj->end();
    }

    class Array : public entity
    {
        friend entity_handler;
    public:
        Array();
        ~Array();

        Array(const Array&) = delete;
        Array(Array&&) = delete;


        struct iterator
        {
            inline iterator(std::vector<entity_handler>::iterator iter)
                :iter(iter)
            {}

            inline wrapper operator*() {
                return wrapper(*iter);
            }

            inline iterator& operator++() {
                ++iter;
                return *this;
            }

            inline bool operator!=(const iterator& other) {
                return this->iter != other.iter;
            }

            std::vector<entity_handler>::iterator iter;
        };

        inline iterator begin() {
            return iterator(value.begin());
        }

        inline iterator end() {
            return iterator(value.end());
        }
        

    // private:
        wrapper get(int) override;

        std::string serialize() override;

        bool isArray() override;

        void push(entity*);

        void initTo(size_t);

        ArrayWrapper array() override;


        std::vector<entity_handler> value;

        #ifdef NII_DEBUG
            public: inline static int GetCount() { return count; }
            inline static int count = 0;
        #endif
    };

    auto ArrayWrapper::begin() {
        return arr->begin();
    }

    auto ArrayWrapper::end() {
        return arr->end();
    }
}
}
}