#include <NII/json-files/deserializator.hpp>
#include <NII/json-files/entities.hpp>

#include <tuple>

#include <cctype>
#include <memory>

namespace nii {
namespace json {
namespace deserializator 
{
    namespace entities = nii::json::entities;
    using j_object = entities::object;
    using j_array = entities::array;
    using j_number = entities::number;
    using j_string = entities::string;
    using j_boolean = entities::boolean;

    using iterator = std::string::const_iterator;
    
    inline static std::string parse_string(iterator& current, iterator end);
    inline static void skip_spaces(iterator& current, iterator end);

    static entities::entity* parse_entity(iterator& current, iterator end);

    static entities::entity* parse_jstring(iterator& current, iterator end);
    static entities::entity* parse_jnumber(iterator& current, iterator end);
    static entities::entity* parse_jboolean(iterator& current, iterator end);
    static entities::entity* parse_jnull(iterator& current, iterator end);
    static entities::entity* parse_jobject(iterator& current, iterator end);
    static entities::entity* parse_jarray(iterator& current, iterator end);

    j_object* parse(const std::string& str)
    {
        auto begin = str.begin();
        auto end = str.end();
        auto obj = parse_jobject(begin, end);
        return dynamic_cast<j_object*>(obj);
    }





    

    static entities::entity* parse_entity(iterator& current, iterator end)
    {
        skip_spaces(current, end);

            switch (*current)
            {
            case '"':
                return parse_jstring(current, end);
            break;

            case '{':
                return parse_jobject(current, end);
            break;

            case '[':
                return parse_jarray(current, end);
            break;

            case 't':
            case 'f':
                return parse_jboolean(current, end);
            break;

            case 'n':
                return parse_jnull(current, end);
            break;

            default:
                if(*current != '0' && (isdigit(*current) || *current == '-') ) {
                    return parse_jnumber(current, end);
                }
            }

        throw "Unexpected character in json string PARSE_ENTITY 1";
    }


    inline static void skip_spaces(iterator& current, iterator end)
    {
        //bad remove this
        if(current == end) throw "Unexpected end of json string SKIP_SPACES 1";
        
        while(isspace(*current)) {
            ++current;
            if(current == end) throw "Unexpected end of json string SKIP_SPACES 2";
        };
    }

    inline static std::string parse_string(iterator& current, iterator end)
    {
        skip_spaces(current, end);
        if(*current != '"') throw "Unexpected character when parsing string PARSE_STRING";
        iterator start_key(current);

        do {
            ++current;
            if(current == end) throw "Unexpected end of json string PARSE_STRING 1";
        }while(*current != '"');

        ++start_key;
        iterator end_key(current);
        ++current;
        if(current == end) throw "Unexpected end of json string PARSE_STRING 2";

        return std::string(start_key, end_key);
    }



    static entities::entity* parse_jobject(iterator& current, iterator end)
    {
        //REMOVE THIS BLOCK ==================>
            skip_spaces(current, end);//remove this after
            if(*current != '{') throw "Unexpected character when parsing j_object 1";//remove this after
            ++current;//remove this after
            if(current == end) throw "Unexpected end of json object PASRE_JOBJECT 1";//remove this after
        //REMOVE THIS BLOCK END ==============>

        
        std::unique_ptr<j_object> obj(new j_object());
        skip_spaces(current, end);
        bool expectingComma = false;
        bool afterComma = false;
        while( afterComma || *current != '}'  ) {
            if(expectingComma) {
            //expects comma ','
                if(*current != ',') throw "Unexpected character, expecting character ',' PARSE JOBJECT";
                expectingComma = false;
                afterComma = true;
                ++current;
                //expects key
                if(current == end) throw "Unexpected end of json string PASRE_JOBJECT 3";
            } else {
            //expects key
                if(*current == '"') {
                    auto key = parse_string(current, end);
                    skip_spaces(current, end);

                    //expects character :
                    if(*current != ':') throw "Expected character ':' parsing j_object 001";
                    ++current;

                    //expects string, number, null, boolean, object, array
                    if(current == end) throw "Unexpected end of json string PASRE_JOBJECT 2";
                    
                    obj->get(key) = parse_entity(current, end);
                    expectingComma = true;
                    afterComma = false;
                } else {
                    //expects key
                    throw "Unexpected end of json string, expecting key PARSE_JOBJECT 4";
                }
            }
            skip_spaces(current, end);
        }

        ++current;
        return obj.release();

    }

    static entities::entity* parse_jstring(iterator& current, iterator end)
    {
        auto str = parse_string(current, end);
        return new j_string(str);
    }

    static entities::entity* parse_jnumber(iterator& current, iterator end)
    {
        iterator start(current);
        while(isdigit(*current) || *current == '.') {
            ++current;
            if(current == end) throw "Unexpected end on json string PARSE NUMBER";
        }
        auto str = std::string(start, current);

        auto number = std::stod(str);
        return new j_number(number);
        //expects number
        throw "Unexpected number value PARSE NUMBER";
    }

    static entities::entity* parse_jboolean(iterator& current, iterator end)
    {
        iterator start(current);
        while(isalpha(*current)) {
            ++current;
            if(current == end) throw "Unexpected end on json string PARSE_BOOLEAN";
        }
        auto str = std::string(start, current);
        if(str == "true") {
            return new j_boolean(true);
        } else if(str == "false") {
            return new j_boolean(false);
        }

        //expects true or false
        throw "Unexpected literal name PARSE BOOLEAN";
    }

    static entities::entity* parse_jnull(iterator& current, iterator end)
    {
        iterator start(current);
        while(isalpha(*current)) {
            ++current;
            if(current == end) throw "Unexpected end on json string PARSE_NULL";
        }
        auto str = std::string(start, current);
        if(str == "null") {
            return nullptr;
        }

        //expects null
        throw "Unexpected literal name PARSE NULL";
    }

    static entities::entity* parse_jarray(iterator& current, iterator end)
    {
        //REMOVE THIS BLOCK ==================>
            skip_spaces(current, end);//remove this after
            if(*current != '[') throw "Unexpected character when parsing j_array 1";//remove this after
            ++current;//remove this after
            if(current == end) throw "Unexpected end of json array PASRE_JARRAY 1";//remove this after
        //REMOVE THIS BLOCK END ==============>

        std::unique_ptr<j_array> array(new j_array());
        skip_spaces(current, end);
        bool expectingComma = false;
        bool afterComma = false;
        size_t currentItem = 0;
        while( afterComma || *current != ']'  ) {
            if(expectingComma) {
            //expects comma ','
                if(*current != ',') throw "Unexpected character, expecting character ',' PARSE_ARRAY";
                expectingComma = false;
                afterComma = true;
                ++current;
                //expects key
                if(current == end) throw "Unexpected end of json string PARSE_ARRAY 3";
            } else {
                array->initTo(currentItem+1);
                array->get(currentItem++) = parse_entity(current, end);
                expectingComma = true;
                afterComma = false;
            }
            skip_spaces(current, end);
        }
        ++current;
        return array.release();
    }



} }}//namespace nii::json::deserializator

