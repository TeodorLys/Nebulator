#include "data.h"
#include "parser.h"
/*STATIC DECLARATION*/
std::vector<data::tokenized_stuff> data::blocks;
std::vector<std::string> data::objects;
std::vector<data::tokenized_stuff> data::lookup_object_groups;
std::vector<data::address_object> data::lookup_address_objects;
std::vector<data::service_object> data::lookup_service_objects;
data::organisation data::current_org;
std::string data::auth_token;
std::string data::current_path = "";
/*STATIC DECLARATION*/

// Splits a service object into name, protocol, port-type and the ports
// Ex. service-object BONJOUR udp eq 5353
data::service_object data::tokenize_service_object(std::string s){
    std::vector<std::string> buff = data::tokenize(s);
    data::service_object _tmp;

    if(buff.size() < 5)
        return data::service_object();

    _tmp.name = buff[1];
    _tmp.protocol = buff[2];
    _tmp.type = buff[3];
    /*
    Some edge cases that might occur
    */
    if(_tmp.protocol != "icmpv6"){
        if(_tmp.type == "range"){
            _tmp.ports = buff[4] + "-" + buff[5];         
        }else{
            _tmp.ports = buff[4]; 
        }
    }else {
        _tmp.type = "eq";
        _tmp.ports = buff[3];
    }
    return _tmp;
}


// Splits an address-object into name and ipaddress
data::address_object data::tokenize_address_object(std::string s){
    std::vector<std::string> buff = data::tokenize(s);
    data::address_object _tmp;
    if(buff.size() < 3)
        return data::address_object();

    _tmp.name = buff[1];
    _tmp.ipv4 = buff[2];
    if(_tmp.ipv4 == "interface-subnet")
        _tmp.ipv4 = buff[3];
    return _tmp;
}
