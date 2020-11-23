#include <NII/Json/deserializator.hpp>

#include <NII/Json/entities.hpp>
#include <tuple>

namespace nii::json::deserializator
{
    namespace entities = nii::json::entities;
    using j_object = entities::object;
    using j_array = entities::array;
    using j_number = entities::number;
    using j_string = entities::string;
    using j_boolean = entities::boolean;

    using iterator = std::string::const_iterator;
    
    static std::string parse_string(iterator& current, iterator end);
    static entities::entity* value(iterator& current, iterator end);

    static std::string parse_jstring(iterator& current, iterator end);
    static std::string parse_jnumber(iterator& current, iterator end);
    static std::string parse_jboolean(iterator& current, iterator end);
    static std::string parse_jnull(iterator& current, iterator end);

    void parse(const std::string& str)
    {
        auto begin = str.begin();
        auto end = str.end();
        j_object obj;
        // std::cout << key(begin, end) << "||" << value(begin, end) << std::endl;
        
        obj.get(key(begin, end)) = value(begin, end);
        std::cout << obj.serialize() << std::endl;
    }


    static std::string parse_string(iterator& current, iterator end)
    {
        iterator start_key(nullptr), end_key(nullptr);
        bool started = true;
        for(; current != end; ++current) {
            if(*current == '"') {
                if(started) {
                    start_key = ++current;
                    started = false;
                }
                else  {
                    end_key = current++;
                    break;
                }
            }
        }

        return std::string(start_key, end_key);
    }

    static entities::entity* value(iterator& current, iterator end)
    {
        for(; current != end; ++current) {
            switch(*current) {
                case '"':
                    return new j_string(key(current, end));
            }
        }
    }
} //namespace nii::json::deserializator