#include <NII/Json/entities.hpp>

namespace nii::json::entities
{

    //================================ ENTITY ================================//
    entity::entity()
    {}

    entity::~entity()
    {}

    entity::operator std::string()
    {
        throw "Can't convert j_entity to std::string";
    }

    entity::operator int()
    {
        throw "Can't convert j_entity to int";
    }

    entity::operator double()
    {
        throw "Can't convert j_entity to double";
    }

    entity::operator bool()
    {
        throw "Can't convert j_entity to bool";
    }

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
        boolean* obj = dynamic_cast<boolean*>(_entity);
        if(!obj) {
            destroyEntity();
            _entity = obj = new boolean(b);
        } else {
            obj->value = b;
        }
    }

    void entity_handler::setValue(double i)
    {
        number* obj = dynamic_cast<number*>(_entity);
        if(!obj) {
            destroyEntity();
            _entity = obj = new number(i);
        } else {
            obj->value = i;
        }
    }

    void entity_handler::setValue(const std::string& str)
    {
        string* obj = dynamic_cast<string*>(_entity);
        if(!obj) {
            destroyEntity();
            _entity = obj = new string(str);
        } else {
            obj->value = str;
        }
    }

    void entity_handler::setValue(std::string&& str)
    {
        string* obj = dynamic_cast<string*>(_entity);
        if(!obj) {
            destroyEntity();
            _entity = obj = new string(std::move(str));
        } else {
            obj->value = std::move(str);
        }
    }

    void entity_handler::setValue()
    {
        destroyEntity();
    }

    void entity_handler::addToArray(int i) {
        array* arr = dynamic_cast<array*>(_entity);
        if(!arr) {
            destroyEntity();
            _entity = arr = new array();
        }
        arr->push(new number(i));
    }

    wrapper entity_handler::get(const std::string& str)
    {
        object* obj = dynamic_cast<object*>(_entity);
        if(!obj) {
            destroyEntity();
            _entity = obj = new object();
        }
        return obj->get(str);
    }

    wrapper entity_handler::get(int i)
    {
        array* arr = dynamic_cast<array*>(_entity);
        if(!arr) {
            // throw "J_ENTITY_HANDLER: Can't get offset on not array";
            destroyEntity();
            _entity = arr = new array();
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

    number::number(int i) 
        : entity()
        , value(i)
    {count++;}

    number::~number()
    {count--;}

    void number::set(int i)
    {
        value = i;
    }

    void number::set(double d)
    {
        value = d;
    }

    void number::set(bool b)
    {
        value = b;
    }

    number::operator int()
    {
        return value;
    }

    number::operator double()
    {
        return value;
    }

    number::operator bool()
    {
        return value;//TODO
    }

    std::string number::serialize()
    {
        return std::to_string(value);
    }



     //================================ NUMBER ================================//

    boolean::boolean(bool b) 
        : entity()
        , value(b)
    {count++;}

    boolean::~boolean()
    {count--;}

    void boolean::set(int i)
    {
        value = i;
    }

    void boolean::set(bool b)
    {
        value = b;
    }

    boolean::operator int()
    {
        return value;
    }

    boolean::operator bool()
    {
        return value;
    }

    std::string boolean::serialize()
    {
        return value ? "true" : "false";
    }



    //================================ STRING ================================//

    string::string(const std::string& str) 
        : entity()
        , value(str)
    {count++;}

    string::string(std::string&& str) 
        : entity()
        , value(std::move(str))
    {count++;}

    string::~string()
    {count--;}

    void string::set(const std::string& str)
    {
        value = str;
    }
    void string::set(std::string&& str)
    {
        value = std::move(str);
    }

    string::operator std::string()
    {
        return value;
    }

    std::string string::serialize()
    {
        
        return '"'+value+'"';
    }


    //================================ OJBECT ================================//

    object::object() 
        : entity()
        , value()
    {count++;}

    object::~object()
    {count--;}

    wrapper object::get(const std::string& str)
    {
        return wrapper(value[str]);
    }
    std::string object::serialize()
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
    
    array::array()
        : entity()
        , value()
    {count++;}

    array::~array()
    {count--;}

    wrapper array::get(int i)
    {
        return wrapper(value.at(i));
    }

    std::string array::serialize()
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

    void array::push(entity* _entity)
    {
        value.emplace_back(_entity);
    }

    void array::initTo(size_t i)
    {
        if(i > value.size()) value.resize(i);
    }

}