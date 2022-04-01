#pragma once
#include <string>
#include "json.hpp"
#include <fstream>
#include <iostream>

#define WAN 0
#define LAN 1

class request_compiler {
public:

    const std::string NEG_MODE_AGGRESSIVE = "AGGRESSIVE";
    const std::string NEG_MODE_MAIN = "MAIN";
    const std::string ENCRYP_DES = "DES";
    const std::string ENCRYP_3DES = "3DES";
    const std::string ENCRYP_AES128 = "AES128";
    const std::string ENCRYP_AES192 = "AES192";
    const std::string ENCRYP_AES256 = "AES256";
    const std::string AUTH_MD5 = "MD5";
    const std::string AUTH_SHA128 = "SHA128";
    const std::string AUTH_SHA256 = "SHA256";
    const std::string AUTH_SHA512 = "SHA512";
    const std::string DIFFIE_1 = "DH1";
    const std::string DIFFIE_2 = "DH2";
    const std::string DIFFIE_5 = "DH5";
    const std::string DIFFIE_14 = "DH14";
    const std::string IKE_VERSION_1 = "IKE_V1";
    const std::string IKE_VERSION_2 = "IKE_V2";

    struct interface_id{
        std::string name;
        nlohmann::json port_group_id;
        nlohmann::json id;
        interface_id(std::string _n, nlohmann::json _pgi, nlohmann::json _id) : name(_n), port_group_id(_pgi), id(_id){}
    };

    struct nat_vs {
        nlohmann::json desc;
        nlohmann::json enabled;
        nlohmann::json priv_ipv4;
        nlohmann::json pub_ipv4;
        nlohmann::json priv_ports;
        nlohmann::json pub_ports;
        nlohmann::json protocol;
        nlohmann::json remote_ipv4_list;
        nlohmann::json wan_iface;
    };

    struct wan_lan {
        int idx; // 0 = WAN, 1 = LAN
        //l3_conf
        nlohmann::json gateway;
        nlohmann::json ip_type; // Static or DHCP
        nlohmann::json ipv4;
        nlohmann::json submask;
        //l3_conf
        nlohmann::json id;
        nlohmann::json port_group_id;
        //LAN
        nlohmann::json ipv4_start;
        nlohmann::json pool_size;
        nlohmann::json lease;
        nlohmann::json type;
        std::string name;
    };

    struct sts_vpn{
        nlohmann::json address;
        nlohmann::json availability;
        nlohmann::json enabled;
        nlohmann::json ipsec_asso_lifetime;
        nlohmann::json ipsec_esp;  // Encryption and authentication
        nlohmann::json ipsec_ike_diffie;
        nlohmann::json ipsec_ike_version;
        nlohmann::json ipsec_lifetime;
        nlohmann::json ipsec_local_id;
        nlohmann::json ipsec_negotiation_mode; // Usually AGGRESSIVE OR MAIN
        nlohmann::json ipsec_peer_id;
        nlohmann::json ipsec_pfs_diffie;
        nlohmann::json ipsec_preset;
        nlohmann::json ipsec_preset_fe;
        nlohmann::json ipsec_proposal;  // Encryption and authentication
        nlohmann::json ipsec_vti;   //  ip and subnet
        nlohmann::json pre_shared_key; 
        nlohmann::json remote_address;
        nlohmann::json remote_cidr_list;
        nlohmann::json conn_check;
        std::string name;
    };


private:
    std::vector<interface_id> interface;
public:
    const int UDP_PROTOCOL = 17;
    const int TCP_PROTOCOL = 6;
    const int BOTH_PROTOCOL = 23;
public:
    std::string last_json;
    void initialize_data();
    void push_sts_vpn_json(sts_vpn vpn);
    void push_nat_json(nat_vs v);
    void push_interface_json(wan_lan _wl);

public:
    interface_id get_loaded_interface(std::string name){
        for(interface_id ii : interface){
            if(ii.name == name)
                return ii;
        }
        return interface_id("", "", "");
    }
private:
};