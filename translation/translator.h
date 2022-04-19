#pragma once
#include "../request_compiler.h"
#include "../config_parser/serialize_config.h"

class translator {
private:
    request_compiler temp; // TEMPORARY TESTING
    const int UDP_PROTOCOL = 17;
    const int TCP_PROTOCOL = 6;
    const int BOTH_PROTOCOL = 23;
    // For the helper functions, so i dont need to pass the name of the current translation to each function
    std::string current_translation;
public:
    static std::vector<request_compiler::wan_lan> interfaces;
    static std::vector<request_compiler::nat_vs> nat;
    static std::vector<request_compiler::sts_vpn> vpn;
public:
    void translate_interface();
    void translate_nat_virtual_server();
    void translate_vpn();
    void display_data();
private:
    int protocol_translation(std::string prot);
    nlohmann::json parse_ports(std::string ports);
    nlohmann::json parse_vpn_crypt_auth(std::string old);
    request_compiler::wan_lan get_json_interface(std::string iface);
};