#pragma once
#include <string>

// #include "entities.hpp"

namespace nii::json
{   
    namespace entities {
        class object;
    }
    
    namespace deserializator
    {
        nii::json::entities::object* parse(const std::string& str);

    }


} //namespace nii::json::deserializator