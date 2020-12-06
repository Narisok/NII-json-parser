#pragma once
#include <string>

// #include "entities.hpp"

namespace nii
{
    namespace json
    {   
        namespace entities {
            class Object;
        }
        
        namespace deserializator
        {
            nii::json::entities::Object* parse(const std::string& str);

        }


    } //namespace nii::json::deserializator
}