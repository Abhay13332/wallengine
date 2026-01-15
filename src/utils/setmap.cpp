#include <any>
#include <includeall.hpp>

#include <string>
#include <unordered_map>
#include <utils/setmap.hpp>

 fi_settings::fi_settings(std::unordered_map<std::string, std::any> data){
        this->data=data;
    }
    void fi_settings::push(std::string setting,std::any value){
        data.insert({setting,value});
    }
    template<typename T>
    T fi_settings::get(std::string key){
        return ((T)data[key]);
    };
bool fi_settings::contains(std::string key){
return this->data.contains(key);
}