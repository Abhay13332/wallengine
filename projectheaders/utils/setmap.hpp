#ifndef SET_MAP_H
#define SET_MAP_H
#include <any>
#include<includeall.hpp>
#include<map>
#include <string>
#include <unordered_map>
#define fi_setting_set(...) fi_settings(std::unordered_map<std::string, std::any>(__VA_ARGS__)) 
#define fi_get_else(map,key,type,...) (map).contains(key)? (map).get<type>(key):(__VA_ARGS__)
class fi_settings{
    public:
    std::unordered_map<std::string, std::any> data;
    fi_settings(std::unordered_map<std::string, std::any> data);
    void push(std::string setting,std::any value);
      template<typename T>
    T get(std::string key){
        return (std::any_cast<T>(data[key]));
    };
    bool contains(std::string key) ;
    
    
};
#endif