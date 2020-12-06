#include <NII/json-files/entities.hpp>

namespace nii {
namespace json {
namespace entities
{

    //================================ ENTITY ================================//
    entity::entity()
    {}

    entity::~entity()
    {}

    wrapper entity::get(const std::string&)
    {
        throw "Can't get string from j_entity";
    }

    wrapper entity::get(int)
    {
        throw "Can't get int from j_entity";
    }

    void entity::set(int)
    {
        throw "Cant't set int from j_entity";
    }

    void entity::set(double)
    {
        throw "Cant't set double from j_entity";
    }

    void entity::set(bool)
    {
        throw "Cant't set bool from j_entity";
    }

    void entity::set(const std::string&)
    {
        throw "Cant't set string from j_entity";
    }
    void entity::set(std::string&&)
    {
        throw "Cant't set string from j_entity";
    }


    //============================== ENTITY_INTERFACE ==============================//
    std::string entityInterface::string() 
    {
        throw "Can't convert EntityInterface to std::string";
    }

    double entityInterface::number()
    {
        throw "Can't convert EntityInterface to Number";
    }

    bool entityInterface::boolean()
    {
        throw "Can't convert EntityInterface to bool";
    }

    ObjectWrapper entityInterface::object()
    {
        throw "Can't convert EntityInterface to object";
    }

    ArrayWrapper entityInterface::array()
    {
        throw "Can't convert EntityInterface to array";
    }

    bool entityInterface::isArray()
    {
        return false;
    }

    bool entityInterface::isObject()
    {
        return false;
    }

    bool entityInterface::isString()
    {
        return false;
    }

    bool entityInterface::isNumber()
    {
        return false;
    }

    bool entityInterface::isBoolean()
    {
        return false;
    }

    bool entityInterface::isNull()
    {
        return false;
    }

    
    entityInterface::operator std::string()
    {
        throw "Can't convert j_entity to std::string";
    }

    entityInterface::operator int()
    {
        throw "Can't convert j_entity to int";
    }

    entityInterface::operator double()
    {
        throw "Can't convert j_entity to double";
    }

    entityInterface::operator bool()
    {
        throw "Can't convert j_entity to bool";
    }


    //============================== ENTITY_HANDLER ==============================//
    entity_handler::entity_handler(entity* _entity)
        : _entity(_entity)
    {/* std::cout << "JEH *" << std::endl; */}
    entity_handler::entity_handler()
        : _entity(nullptr)
    {/* std::cout << "JEH" << std::endl; */}

    entity_handler::entity_handler(entity_handler&&other)
        : _entity(other.extract())
    {/* std::cout << "JEH&&" << std::endl; */}

    entity_handler::~entity_handler()
    {
        destroyEntity();
        {/* std::cout << "~JEH" << std::endl; */}
    }

    void entity_handler::setValue(int i)
    {
        setValue(static_cast<double>(i));
    }

    void entity_handler::setValue(bool b)
    {
        Boolean* obj = dynamic_cast<Boolean*>(_entity);
        if(!obj) {
            destroyEntity();
            _entity = obj = new Boolean(b);
        } else {
            obj->value = b;
        }
    }

    void entity_handler::setValue(double i)
    {
        Number* obj = dynamic_cast<Number*>(_entity);
        if(!obj) {
            destroyEntity();
            _entity = obj = new Number(i);
        } else {
            obj->value = i;
        }
    }

    void entity_handler::setValue(const std::string& str)
    {
        String* obj = dynamic_cast<String*>(_entity);
        if(!obj) {
            destroyEntity();
            _entity = obj = new String(str);
        } else {
            obj->value = str;
        }
    }

    void entity_handler::setValue(std::string&& str)
    {
        String* obj = dynamic_cast<String*>(_entity);
        if(!obj) {
            destroyEntity();
            _entity = obj = new String(std::move(str));
        } else {
            obj->value = std::move(str);
        }
    }

    void entity_handler::setValue(entity * _entity)
    {
        destroyEntity();
        this->_entity = _entity; 
    }

    void entity_handler::setValue()
    {
        destroyEntity();
    }

    void entity_handler::addToArray(int i) {
        Array* arr = dynamic_cast<Array*>(_entity);
        if(!arr) {
            destroyEntity();
            _entity = arr = new Array();
        }
        arr->push(new Number(i));
    }

    wrapper entity_handler::get(const std::string& str)
    {
        Object* obj = dynamic_cast<Object*>(_entity);
        if(!obj) {
            destroyEntity();
            _entity = obj = new Object();
        }
        return obj->get(str);
    }

    wrapper entity_handler::get(int i)
    {
        Array* arr = dynamic_cast<Array*>(_entity);
        if(!arr) {
            // throw "J_ENTITY_HANDLER: Can't get offset on not array";
            destroyEntity();
            _entity = arr = new Array();
        }
        arr->initTo(i+1);
        return arr->get(i);
    }

    void entity_handler::destroyEntity()
    {
        if(_entity) delete _entity;
        _entity = nullptr;
    }

    entity* entity_handler::extract()
    {
        entity* ptr = _entity;
        _entity = nullptr;
        return ptr;
    }


    
    //================================ NUMBER ================================//

    Number::Number(double i) 
        : entity()
        , value(i)
    {
        #ifdef NII_DEBUG
            count++;
        #endif
    }

    Number::~Number()
    {
        #ifdef NII_DEBUG
            count--;
        #endif
    }

    void Number::set(int i)
    {
        value = i;
    }

    void Number::set(double d)
    {
        value = d;
    }

    void Number::set(bool b)
    {
        value = b;
    }

    Number::operator int()
    {
        return value;
    }

    Number::operator double()
    {
        return value;
    }

    Number::operator bool()
    {
        return value;//TODO
    }

    double Number::number()
    {
        return value;
    }

    std::string Number::string()
    {
        return std::to_string(value);
    }

    bool Number::isNumber()
    {
        return true;
    }

    std::string Number::serialize()
    {
        return std::to_string(value);
    }



     //================================ BOOLEAN ================================//

    Boolean::Boolean(bool b) 
        : entity()
        , value(b)
    {
        #ifdef NII_DEBUG
            count++;
        #endif
    }

    Boolean::~Boolean()
    {
        #ifdef NII_DEBUG
            count--;
        #endif
    }

    void Boolean::set(int i)
    {
        value = i;
    }

    void Boolean::set(bool b)
    {
        value = b;
    }

    bool Boolean::boolean()
    {
        return value;
    }

    std::string Boolean::string()
    {
        return value ? "true" : "false";
    }

    bool Boolean::isBoolean()
    {
        return true;
    }

    Boolean::operator int()
    {
        return value;
    }

    Boolean::operator bool()
    {
        return value;
    }

    std::string Boolean::serialize()
    {
        return value ? "true" : "false";
    }



    //================================ STRING ================================//

    String::String(const std::string& str) 
        : entity()
        , value(str)
    {
        #ifdef NII_DEBUG
            count++;
        #endif
    }

    String::String(std::string&& str) 
        : entity()
        , value(std::move(str))
    {
        #ifdef NII_DEBUG
            count++;
        #endif
    }

    String::~String()
    {
        #ifdef NII_DEBUG
            count--;
        #endif
    }

    void String::set(const std::string& str)
    {
        value = str;
    }
    void String::set(std::string&& str)
    {
        value = std::move(str);
    }

    std::string String::string()
    {
        return value;
    }

    bool String::isString()
    {
        return true;
    }

    String::operator std::string()
    {
        return value;
    }

    std::string String::serialize()
    {
        
        return '"'+value+'"';
    }


    //================================ OJBECT ================================//

    Object::Object() 
        : entity()
        , value()
    {
        #ifdef NII_DEBUG
            count++;
        #endif
    }

    Object::~Object()
    {
        #ifdef NII_DEBUG
            count--;
        #endif
    }

    Object::Object(Object && other)
        : entity()
        , value(std::move(other.value))
    {
        #ifdef NII_DEBUG
            count++;
        #endif
    }

    Object& Object::operator=(Object && other)
    {
        value = std::move(other.value);
        return *this;
    }

    wrapper Object::get(const std::string& str)
    {
        return wrapper(value[str]);
    }

    // ObjectWrapper Object::object()
    // {
    //     return ObjectWrapper(*this);
    // }

    bool Object::isObject()
    {
        return true;
    }

    std::string Object::serialize()
    {
        std::string result = "{ ";
        for(auto& a : value) {
            result += '"' + a.first + "\": " + (a.second._entity ? a.second._entity->serialize() : std::string("null"));
            if(a.first != value.rbegin()->first) {
                result += ", ";
            }
        }
        return result += " }";
    }



    //================================ ARRAY ================================//
    
    Array::Array()
        : entity()
        , value()
    {
        #ifdef NII_DEBUG
            count++;
        #endif
    }

    Array::~Array()
    {
        #ifdef NII_DEBUG
            count--;
        #endif
    }

    wrapper Array::get(int i)
    {
        return wrapper(value.at(i));
    }

    bool Array::isArray()
    {
        return true;
    }

    std::string Array::serialize()
    {
        std::string result = "[ ";
        for(auto iter = value.begin(); iter != value.end(); ++iter) {
            result += iter->_entity ? iter->_entity->serialize() : std::string("null");
            if(iter != --value.end()) {
                result += ", ";
            }
        }
        return result += " ]";
    }

    void Array::push(entity* _entity)
    {
        value.emplace_back(_entity);
    }

    void Array::initTo(size_t i)
    {
        if(i > value.size()) value.resize(i);
    }

} }}