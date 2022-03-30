#pragma once
#include <vector>
#include <string>
#include <random>
#include <time.h>

class data {
public:
    struct tokenized_stuff{
        std::vector<std::string> raw;
        std::string name;
        std::string sub_name;
        std::string value;
    };

    struct lookup{
        tokenized_stuff *block;
        std::string *object;
    };

    struct service_object{
        std::string name = "NULL", protocol = "NULL", type = "NULL", ports = "NULL";
    };

    struct address_object {
        std::string name = "NULL", ipv4 = "NULL";
    };

    struct organisation {
        std::string org_id;
        std::string site_id;
    };

    static std::vector<tokenized_stuff> blocks;
    static std::vector<std::string> objects;
    static std::vector<tokenized_stuff> lookup_object_groups;
    static std::vector<address_object> lookup_address_objects;
    static std::vector<service_object> lookup_service_objects;
public:
    static organisation current_org;
    static address_object tokenize_address_object(std::string s);
    static service_object tokenize_service_object(std::string s);
    static std::string auth_token;
    static std::string current_path;
public:

    static address_object search_address_object(std::string s) {
        for(address_object _s : lookup_address_objects){
            if(_s.name.find(s) != std::string::npos){
                return _s;
            }
        }
        address_object obj;
        obj.ipv4 = s;
        return obj;
    }
    
    static service_object search_service_object(std::string s) {
        for(service_object _s : lookup_service_objects){
            if(_s.name.find(s) != std::string::npos){
                return _s;
            }
        }
            return service_object();
    }

    //Returns all of the service-objects inside and service group
    static std::vector<service_object> search_object_group(std::string s){
        std::vector<service_object> ret;
        for(tokenized_stuff _s : lookup_object_groups){
            if(_s.name.find(s) != std::string::npos){
                for(std::string t : _s.raw){
                    ret.push_back(search_service_object(tokenize(t)[1]));
                }
                return ret;
            }
        }
        return ret;
    }


    //Uses whitespaces to split the string
    static std::vector<std::string> tokenize(std::string _s) {
        std::string buffer;
        std::vector<std::string> _tokened;
        for(int a = 0; a < _s.size(); a++){
            // Pushback each char into a string buffer until a space is found
            if(_s[a] != ' ')
                buffer += _s[a];
            // Pushback the string buffer from above and resets it
            else if(_s[a] == ' '){
                _tokened.push_back(buffer);
                buffer = "";
            }
        }
        // If there isnt a space at the end the function param(rarely is)
        // we will have data left in the buffer after the loop is done
        // So if there is data left, push it to the vector
        if(!buffer.empty())
            _tokened.push_back(buffer);
        return _tokened;
    }

    // Here we split and ip address into 4 strings, the numbers seperated by a .
    static std::vector<std::string> tokenize_ipv4(std::string _s) {
        std::string buffer;
        std::vector<std::string> _tokened;

        //See function "tokenize" for description of what this is...
        for(int a = 0; a < _s.size(); a++){
            if(_s[a] != '.')
                buffer += _s[a];
            else if(_s[a] == '.'){
                _tokened.push_back(buffer);
                buffer = "";
            }
        }
        if(!buffer.empty())
            _tokened.push_back(buffer);
        return _tokened;
    }

    /*
    In some cases the ip addess are the same in ex. vpn configs, se we call this random number gen and changes the ip.
    */
    static int random_number(int mod){
        static bool set = false;
        if(!set){
            srand(time(NULL));
            set = true;
        }
        return rand() % mod;
    }
};