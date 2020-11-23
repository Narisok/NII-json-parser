#pragma once
#pragma GCC diagnostic ignored "-Woverloaded-virtual"

#include <iostream>

#include <string>
#include <map>
#include <vector>
#include <utility>

#include "deserializator.hpp"

namespace nii::json
{
    class json;
namespace entities
{
    class entity;
    class object;
    class wrapper;
    class array;

    class entity_handler
    {
        friend object;
        friend array;
        friend wrapper;


        
        
        public:     explicit entity_handler(entity*);
                    entity_handler();
                    entity_handler(entity_handler&&);
        public:     ~entity_handler();

        entity_handler(const entity_handler&) = delete;



    private:
        void setValue(int);
        void setValue(double);
        void setValue(const std::string&);
        void setValue(std::string&&);
        void setValue(bool);
        void setValue();

        void addToArray(int);

        wrapper get(const std::string&);
        wrapper get(int);
        
        entity* extract();
        
    private:
        void destroyEntity();

        entity * _entity;
    };

    struct wrapper
    {
        inline wrapper(entity_handler& handle) : handle(handle) {}
        inline ~wrapper() {}

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

        inline wrapper& operator=(entity* _entity) {
            handle._entity = _entity;
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

    private:
        entity_handler& handle;
    };

    class entity
    {
        friend wrapper;
        friend entity_handler;

    protected:
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

        virtual operator int();
        // virtual operator long();
        // virtual operator long long();
        // virtual operator unsigned();
        // virtual operator unsigned long();
        // virtual operator unsigned long long();
        
        virtual operator double();
        virtual operator bool();
        virtual operator std::string();

        public: virtual std::string serialize()=0;
    };

    std::string wrapper::serialize() {
        if(handle._entity) {
            return handle._entity->serialize();
        } else {
            return "null";
        }
    }

    class number : public entity
    {
        friend entity_handler;
        friend void deserializator::parse(const std::string&);

        number(int);
        ~number();

        number(const number&) = delete;
        number(number&&) = delete;
        
        void set(int) override;
        void set(double) override;
        void set(bool) override;
        

        operator int() override;
        operator double() override;
        operator bool() override;

        std::string serialize() override;

        double value;

        public: inline static int GetCount() { return count; }
        inline static int count = 0;
    };
    
    class boolean : public entity
    {
        friend entity_handler;
        friend void deserializator::parse(const std::string&);

        boolean(bool);
        ~boolean();

        boolean(const boolean&) = delete;
        boolean(boolean&&) = delete;
        
        void set(int) override;
        void set(bool) override;
        

        operator int() override;
        operator bool() override;

        std::string serialize() override;

        bool value;

        public: inline static int GetCount() { return count; }
        inline static int count = 0;
    };

    class string : public entity
    {
        friend entity_handler;
        friend void deserializator::parse(const std::string&);

    public: 
        string(const std::string&);
        string(std::string&&);
        ~string();

        string(const string&) = delete;
        string(string&&) = delete;
        
    private:
        void set(const std::string&) override;
        void set(std::string&&) override;
        
        operator std::string() override;

        std::string serialize() override;

        std::string value;

        public: inline static int GetCount() { return count; }
        inline static int count = 0;
    };

    class object : public entity
    {
        friend entity_handler;
        friend nii::json::json;
        friend void deserializator::parse(const std::string&);

        object();
        ~object();

        object(const object&) = delete;
        object(object&&) = delete;
        

        wrapper get(const std::string& str) override;


        std::string serialize() override;

        std::map<std::string, entity_handler> value;

        public: inline static int GetCount() { return count; }
        inline static int count = 0;
    };

    class array : public entity
    {
        friend entity_handler;
        friend void deserializator::parse(const std::string&);
        
        array();
        ~array();

        array(const array&) = delete;
        array(array&&) = delete;


        wrapper get(int) override;

        std::string serialize() override;


        void push(entity*);

        void initTo(size_t);


        std::vector<entity_handler> value;

        public: inline static int GetCount() { return count; }
        inline static int count = 0;
    };
}}
