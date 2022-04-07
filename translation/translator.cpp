#include "translator.h"
#include "../config_parser/serialize_config.h"
#include "../logger.h"

std::vector<request_compiler::wan_lan> translator::interfaces;
std::vector<request_compiler::nat_vs> translator::nat;
std::vector<request_compiler::sts_vpn> translator::vpn;

void translator::translate_interface(){
    temp.initialize_data();
    for(serialize_config::wan_lan_interface wli : serialize_config::interfaces){
        /*
        HACK!

        I have not made it possible to create new interfaces so we will just work with the defaults
        */
        if(wli.name != "wan1" && wli.name != "lan1" && wli.name != "lan2")
            continue;
        try{
            request_compiler::wan_lan wl_buff;
            current_translation = wli.name;
            if(wli.ip.type == "DHCP")
                wli.gateway.ipv4 = data::tokenize_ipv4(wli.ip.ipv4)[0] + "." + data::tokenize_ipv4(wli.ip.ipv4)[1] + "." + data::tokenize_ipv4(wli.ip.ipv4)[2] + ".2";
            wl_buff.gateway = wli.gateway.ipv4;
            wl_buff.idx = wli.idx;
            wl_buff.ip_type = wli.ip.type;
            wl_buff.ipv4 = wli.ip.ipv4;
            wl_buff.ipv4_start = wli.dhcp_pool.start_address;
            wl_buff.lease = {wli.dhcp_pool.lease[0], wli.dhcp_pool.lease[1], wli.dhcp_pool.lease[2]};
            wl_buff.pool_size = std::stoi(wli.dhcp_pool.pool_size);
            wl_buff.type = wli.dhcp_pool.type;
            wl_buff.name = wli.name;
            wl_buff.submask = wli.ip.subnet;
            wl_buff.id = temp.get_loaded_interface(wli.name).id;
            interfaces.push_back(wl_buff);
        }catch(std::exception &e){
            logger::log("%s\n", e.what());
        }
    }
    temp.push_interface_json(request_compiler::wan_lan());
}

void translator::translate_nat_virtual_server(){
    serialize_config conf;
    for(serialize_config::virtual_server vse : serialize_config::virtual_server_entrys){
        request_compiler::nat_vs nat_buff;
        current_translation = vse.name;
        nat_buff.desc = vse.name;
        nat_buff.enabled = true;  // Hack!
        nat_buff.priv_ipv4 = vse.map_to_ipv4.ipv4;
        nat_buff.protocol = protocol_translation(vse.internal_service.protocol);
        if(vse.original_ipv4 == "any" || vse.original_ipv4.empty() || vse.original_ipv4 == "NULL")
            nat_buff.pub_ipv4 = nullptr;
        else 
            nat_buff.pub_ipv4 = vse.original_ipv4;
        //If the ports are a service group, that means there are many diffrent ports or ranges to work with
        //That needs to be parsed into a json object.
        if(vse.map_type == "service-group"){
            nat_buff.priv_ports = parse_ports(vse.internal_service.ports);
            nat_buff.pub_ports = nat_buff.priv_ports;
        }else {
            nat_buff.priv_ports = parse_ports(vse.internal_service.ports);
            nat_buff.pub_ports = parse_ports(vse.external_service.ports);
        }
        nat_buff.remote_ipv4_list = nlohmann::json::array();
        // When we start the program we download the default interface
        // that includes the interface id:s. NAT need the wan id to function...
        nat_buff.wan_iface = get_json_interface(vse.inte).id;   // TEMP!
        nat.push_back(nat_buff);
    }
    // The function has changed. Before we needed to include an nat object to create the json
    // Now the nats are in a static vector inside the data class
    // TODO: Change this
    temp.push_nat_json(request_compiler::nat_vs());

}

void translator::translate_vpn(){
/*
There are a few points of data that we cant just move over... without parsing
soooooo...
TODO: Do that parsing...
*/
    std::string msg_buffer = "";
    for(serialize_config::crypto_vpn _vpn : serialize_config::ipsec_vpn){
        request_compiler::sts_vpn vpn_buff;
        current_translation = _vpn.name;
        vpn_buff.address = "";
        vpn_buff.availability = "CURRENT_SITE";
        vpn_buff.enabled = _vpn.state == "activate" ? true : false;
        vpn_buff.ipsec_asso_lifetime = std::stoi(_vpn.secure_associtaion);
        vpn_buff.ipsec_esp =  parse_vpn_crypt_auth(_vpn.transform_set);
        logger::log("Using ipsec_ike_diffie = DH2 for %s\n", _vpn.name.c_str());
        vpn_buff.ipsec_ike_diffie = "DH2";   // Helper function?
        logger::log("Using ipsec_ike_version IKE_V1 for %s\n", _vpn.name.c_str());
        vpn_buff.ipsec_ike_version = "IKE_V1";
        vpn_buff.ipsec_lifetime = std::stoi(_vpn.ipsec_isakmp.life_time);
        //vpn_buff.ipsec_local_id
        vpn_buff.ipsec_negotiation_mode = _vpn.ipsec_isakmp.mode;
        logger::log("Using ipsec_pfs_diffie = DH2 for %s\n", _vpn.name.c_str());
        vpn_buff.ipsec_pfs_diffie = "DH2";
        vpn_buff.ipsec_preset = "DEFAULT";
        vpn_buff.ipsec_proposal =  parse_vpn_crypt_auth(_vpn.ipsec_isakmp.transform_set);
        vpn_buff.name = _vpn.name;
        // TODO: Decrypt the key? somehow...
        logger::log("Preshared key is encrypted for premise firewalls, using 1234567890 as placeholder for %s\n", _vpn.name.c_str());
        vpn_buff.pre_shared_key = "1234567890";
        if(_vpn.remote_policy.ipv4.find("/") != std::string::npos){
            _vpn.remote_policy.ipv4.erase(_vpn.remote_policy.ipv4.find("/"), _vpn.remote_policy.ipv4.length());
        }
        vpn_buff.remote_address = {_vpn.remote_policy.ipv4};
        vpn_buff.remote_cidr_list = {_vpn.local_policy.ipv4};
        vpn_buff.ipsec_vti = {{"ipv4_address", nullptr}, {"ipv4_subnet_mask", nullptr}};
        vpn.push_back(vpn_buff);
    }

    temp.push_sts_vpn_json(request_compiler::sts_vpn());

}


/*PRIVATE HELPER FUNCTIONS*/
/*NAT PORTS FORMATTING*/

/*
Nebula uses integers to identify which protocol we are using.
SEE .h file for the integers.
*/
int translator::protocol_translation(std::string prot){
    //For ease of conditional we change it to lower
    std::transform(prot.begin(), prot.end(), prot.begin(), ::tolower);
    if(prot.find("tcp") == std::string::npos && prot.find("udp") != std::string::npos)
        return UDP_PROTOCOL;
    else if(prot.find("tcp") != std::string::npos && prot.find("udp") == std::string::npos)
        return TCP_PROTOCOL;
    else if(prot.find("tcp") != std::string::npos && prot.find("udp") != std::string::npos)
        return BOTH_PROTOCOL;
    return 0;
}

nlohmann::json translator::parse_ports(std::string _ports){
    nlohmann::json buff;

    std::vector<std::string> tmp = data::tokenize(_ports);
    std::vector<std::string> dup_check;
    /*
    Duplicate check!
    The standalone firewalls had two different objects with the same ports but different
    protocols. Nebula has a entry that specifies both, tcp and udp.
    If we dont check for the same ports nebula wines.
    */
    for(std::string s : tmp){
        int found = false;
        for(std::string check : dup_check){
            if(s == check){
                found = true;
            }
        }
        if(!found)
            dup_check.push_back(s);
    }
    /**/
    for(std::string s : dup_check){
        buff += s;
    }

    return buff;

}

/*NAT PORTS FORMATTING*/

/*VPN FORMATTING*/
nlohmann::json translator::parse_vpn_crypt_auth(std::string old){
    if(old.find("esp")!= std::string::npos) {
        old.erase(0, old.find("-") + 1);
    }
    std::string aes = old.substr(0, old.find("-"));
    std::string sha = old.substr(old.find("-") + 1, old.length());
    if(aes.size() < 4){
        logger::log("Encryption could not be parsed for %s! Using default of AES128\n", current_translation.c_str());
        aes = "AES128";
    }
    if(sha.size() < 4 && sha.find("sha") != std::string::npos){
        logger::log("Authentication could not be parsed for %s! Using default of SHA256\n", current_translation.c_str());
        sha = "SHA256";
    }
    /*
    Nebula wants the auth and encrypt to be uppercase... 
    */
    if(sha.find("sha") != std::string::npos)
        sha.replace(0, 3, "SHA");
    if(aes.find("aes") != std::string::npos)
        aes.replace(0, 3, "AES");
    nlohmann::json tmp = {{{"encryption", aes}, {"authentication", sha}}};
    return tmp;
}
/*VPN FORMATTING*/

request_compiler::wan_lan translator::get_json_interface(std::string iface){
    for(request_compiler::wan_lan wl : interfaces){
        if(wl.name == iface)
            return wl;
    }
    return request_compiler::wan_lan();
}

/*PRIVATE HELPER FUNCTIONS*/