// #include <NII/Json/entities.hpp>
#include <NII/json>
#include <RunTest.hpp>

// using namespace json;
namespace json = nii::json::entities;



void printS(std::string i)
{
    std::cout << "string: " << i << std::endl;
}

int main()
{

    // tests::run();
// {
//     nii::json::json j(R"( 
//         {
//             "firstName": "Іван",
//             "lastName": "Коваленко",
//             "address": {
//                 "streetAddress": "вул. Грушевського 14, кв.101",
//                 "city": "Київ",
//                 "postalCode": 21000
//             },
//             "phoneNumbers": [
//                 "044 123-1234",
//                 "050 123-4567"
//             ]
//         }
//     )");

//     try{
//         printS(j["firstName"]);
//     } catch (const char* str) { std::cout << "ERROR: " << str << std::endl; }
//     std::cout << j.serialize() << std::endl;
// }

    {
        nii::json::json j;
        try{
            // j.safe["val"];
            // j->safe["val"];
            // j->string["val"]
            // j["val"] = "oantuhou";

            // get value or if value null return default value
            j["val"]->onNull(123);
            j["val"]->set(123); // set value if it null
            j["val"]->safe(); // throw error if value does not exists


            // convert to string @par true => convert from other types
            j["val"]->string(/* true */); 

            // convert to number @par true => convert from other types
            j["val"]->number(/* true */); 

            // convert to boolean @par true => convert from other types
            j["val"]->boolean(/* true */); 
            // j->get("val")->safe();

            std::cout << j.serialize() << std::endl;
        } catch(const char*str) { std::cout << "ERROR: " << str << std::endl; }
        catch(std::exception& exeption) { std::cout << "ERROR: " << exeption.what() << std::endl;}
        catch(...) { std::cout << "ERROR has been ocured" << std::endl; }
    }
    std::cout << "j_number  count: " << json::number::GetCount() << std::endl;
    std::cout << "j_string  count: " << json::string::GetCount() << std::endl;
    std::cout << "j_boolean count: " << json::boolean::GetCount() << std::endl;
    std::cout << "j_object  count: " << json::object::GetCount() << std::endl;
    std::cout << "j_array   count: " << json::array::GetCount() << std::endl;

    return 0;
}