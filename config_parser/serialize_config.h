#pragma once
#include "parser.h"

#define WAN 0
#define LAN 1

class serialize_config{
private:
    parser p;
public:
    struct ip_address_config_style_interface {
        std::string type;  // dhcp or null
        std::string ipv4;  // not used with type dhcp
        std::string subnet;  // not used with type dhcp
        std::string metric;   // ONLY used with dhcp
    };
    struct ip_dhcp_pool_interface{
        std::string network;
        std::string def_router;
        std::string dns_server1;
        std::string dns_server2;
        std::string start_address = "192.168.1.33";
        std::string pool_size = "50";
        std::string lease = "200";
        std::string type = "NONE";
    };

    struct wan_lan_interface{
        int idx = -1; // WAN = 0, LAN = 1
        ip_address_config_style_interface ip;
        ip_address_config_style_interface gateway;
        ip_dhcp_pool_interface dhcp_pool;

        std::string name;
    };
    // NAT virtual server
    struct virtual_server{
        std::string inte;  // usually wan1
        std::string source_ipv4;  
        std::string original_ipv4; // Same as external  
        data::address_object map_to_ipv4;  // Local ip, internal ip
        std::string map_type;  // Original service(service) and service-group
        data::service_object internal_service;
        data::service_object external_service;
        std::string name;
    };
    // ipsec policy
    struct isakmp_vpn{
        std::string state;
        ip_address_config_style_interface local_ipv4;
        ip_address_config_style_interface peer_ipv4;
        std::string auth;
        std::string key;
        std::string local_id;
        std::string peer_id;
        std::string fall_back_check;
        std::string life_time;
        std::string group;
        std::string transform_set;
        std::string mode;
        std::string dpd_interval;
        std::string name;
    };  


    // Site to Site VPN
    struct crypto_vpn {
        std::string state; // ACTIVATE OR DEACTIVATE
        std::string adjust_mss;
        isakmp_vpn ipsec_isakmp;
        std::string scenario; // ex. site-to-site-static
        std::string encapsulation; // ex. tunnel
        std::string transform_set;
        std::string secure_associtaion;
        std::string pfs;
        data::address_object local_policy;
        data::address_object remote_policy;
        std::string conn_check;
        std::string name;
    };

public:
    static std::vector<wan_lan_interface> interfaces;
    static std::vector<virtual_server> virtual_server_entrys;
    static std::vector<crypto_vpn> ipsec_vpn;

public:
    void serialize();
    void serialize_virtual_server();
    void serialize_vpn_map();
public:
    wan_lan_interface get_interface(std::string name){
        for(wan_lan_interface wli : interfaces)
            if(wli.name == name)
                return wli;
        wan_lan_interface tmp;
        tmp.idx = -1;
        return tmp;
    }
private:
    void serialize_interface(std::string iface);
    ip_address_config_style_interface parse_interface_ip_address(std::string s);
    ip_address_config_style_interface parse_gateway_ipv4(std::string s);
    ip_dhcp_pool_interface parse_dhcp_pool(std::string s);
    virtual_server parse_virtual_server_object_to_struct(std::string s);
    crypto_vpn parse_crypto_map(data::tokenized_stuff map);
    isakmp_vpn parse_isakmap(data::tokenized_stuff map);
    void parse_virtual_server_service_group(virtual_server &buff, std::vector<data::service_object> obj, std::vector<data::service_object> &ranges);
};