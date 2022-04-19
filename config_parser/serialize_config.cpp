#include "serialize_config.h"
#include <algorithm>
#include <windows.h>
#include "..\logger.h"

std::vector<serialize_config::wan_lan_interface> serialize_config::interfaces;
std::vector<serialize_config::virtual_server> serialize_config::virtual_server_entrys;
std::vector<serialize_config::crypto_vpn> serialize_config::ipsec_vpn;

/*
TODO: This does too much! split it up!

This function parses all of the data needed for nebula.
Puts it all into its designated structures and creates vectors for it all
*/
void serialize_config::serialize(){
    /*
    if we load the config file from drive we use use a different config path
    */
    if (data::startup_conf.find(".conf") != std::string::npos)
        p.tokenize_file(data::startup_conf);
    else
        p.tokenize_file(data::current_path + "\\dep\\conf\\startup-config.conf");
    serialize_interface("wan");
    serialize_interface("lan");

    serialize_virtual_server();
    serialize_vpn_map();

    try {
        /*check for duplicate ipv4s VPN*/
        for (int a = 0; a < ipsec_vpn.size(); a++) {
            for (int b = 0; b < ipsec_vpn.size(); b++) {
                // So it doesnt check itself
                if (b != a) {
                    if (data::tokenize_ipv4(ipsec_vpn.at(a).remote_policy.ipv4).at(2) == data::tokenize_ipv4(ipsec_vpn.at(b).remote_policy.ipv4).at(2)) {
                        std::vector<std::string> _tmp = data::tokenize_ipv4(ipsec_vpn.at(a).remote_policy.ipv4);
                        std::string _new_ip = _tmp.at(0) + "." + _tmp.at(1) + "." + std::to_string(data::random_number(20)) + "." + _tmp.at(3);
                        logger::log("VPN IP %s in %s has the same remote subnet as %s! Changing %s to %s as placeholder\n", ipsec_vpn.at(a).remote_policy.ipv4.c_str(), ipsec_vpn.at(a).name.c_str(), ipsec_vpn.at(a).name.c_str(), ipsec_vpn.at(a).name.c_str(), _new_ip.c_str());
                        ipsec_vpn.at(a).remote_policy.ipv4 = _new_ip;
                    }
                    if (data::tokenize_ipv4(ipsec_vpn.at(a).local_policy.ipv4).at(2) == data::tokenize_ipv4(ipsec_vpn.at(b).local_policy.ipv4).at(2)) {
                        std::vector<std::string> _tmp = data::tokenize_ipv4(ipsec_vpn.at(a).local_policy.ipv4);
                        std::string _new_ip = _tmp.at(0) + "." + _tmp.at(1) + "." + std::to_string(data::random_number(20)) + "." + _tmp.at(3);
                        logger::log("VPN IP %s in %s has the same local subnet as %s! Changing %s to %s as placeholder\n", ipsec_vpn.at(a).local_policy.ipv4.c_str(), ipsec_vpn.at(a).name.c_str(), ipsec_vpn.at(b).name.c_str(), ipsec_vpn.at(a).name.c_str(), _new_ip.c_str());
                        ipsec_vpn.at(a).local_policy.ipv4 = _new_ip;
                    }
                }
            }
            /*
            checking against interfaces
            */
            /*for(wan_lan_interface wli : interfaces){
                if(data::tokenize_ipv4(ipsec_vpn[a].local_policy.ipv4)[2] == data::tokenize_ipv4(wli.ip.ipv4)[2]){
                     std::vector<std::string> _tmp = data::tokenize_ipv4(ipsec_vpn[a].local_policy.ipv4);
                     std::string _new_ip = _tmp[0] + "." + _tmp[1] + "." + std::to_string(data::random_number(20)) + "." + _tmp[3];
                     logger::log("VPN IP %s in %s has the same interface subnet %s! Changing %s to %s as placeholder\n", ipsec_vpn[a].local_policy.ipv4.c_str(), ipsec_vpn[a].name.c_str(), wli.name.c_str(), ipsec_vpn[a].name.c_str(), _new_ip.c_str());
                     ipsec_vpn[a].local_policy.ipv4 = _new_ip;
                }
                if(data::tokenize_ipv4(ipsec_vpn[a].remote_policy.ipv4)[2] == data::tokenize_ipv4(wli.ip.ipv4)[2]){
                     std::vector<std::string> _tmp = data::tokenize_ipv4(ipsec_vpn[a].remote_policy.ipv4);
                     std::string _new_ip = _tmp[0] + "." + _tmp[1] + "." + std::to_string(data::random_number(20)) + "." + _tmp[3];
                     logger::log("VPN IP %s in %s has the same interface subnet %s! Changing %s to %s as placeholder\n", ipsec_vpn[a].remote_policy.ipv4.c_str(), ipsec_vpn[a].name.c_str(), wli.name.c_str(), ipsec_vpn[a].name.c_str(), _new_ip.c_str());
                     ipsec_vpn[a].remote_policy.ipv4 = _new_ip;
                }
            }*/
        }
        /*check for duplicate ipv4s VPN*/
    }
    catch (std::exception e) {
        logger::log("\"serialize\" out of range! Tell Teodor!! saved in log0.txt %s\n", e.what());
        system("pause");
        exit(0);
    }
}


void serialize_config::serialize_virtual_server(){
    for(std::string s : data::objects){
        if(s.find("ip virtual-server") != std::string::npos){
            //logger::log("%s\n", s.c_str());
            virtual_server_entrys.push_back(parse_virtual_server_object_to_struct(s));
        }
    }
}

void serialize_config::serialize_vpn_map(){
    for(data::tokenized_stuff s : data::blocks){
        if(s.name.find("crypto map") != std::string::npos){
            //logger::log("%s\n", s.name.c_str());
            ipsec_vpn.push_back(parse_crypto_map(s));
        }
    }
}

/*PRIVATE*/

/*SECURE_POLICY PARSING*/

serialize_config::virtual_server serialize_config::parse_virtual_server_object_to_struct(std::string s){
    //ip virtual-server citrix interface wan1 source-ip any original-ip any map-to citrix map-type original-service HTTPS mapped-service HTTPS
    virtual_server buff;
    try {
        std::vector<data::service_object> ranges;
        std::vector<std::string> _tok = data::tokenize(s);
        for (int a = 0; a < _tok.size(); a++) {

            if (_tok.at(a) == "virtual-server") {
                buff.name = _tok.at(a + 1);
            }
            else if (_tok.at(a) == "interface") {
                buff.inte = _tok.at(a + 1);
            }
            else if (_tok.at(a) == "source-ip") {
                buff.source_ipv4 = _tok.at(a + 1);
            }
            else if (_tok.at(a) == "original-ip") {
                buff.original_ipv4 = data::search_address_object(_tok.at(a + 1)).ipv4;
            }
            else if (_tok.at(a) == "map-to") {
                buff.map_to_ipv4 = data::search_address_object(_tok.at(a + 1));
            }
            else if (_tok.at(a) == "map-type") {
                buff.map_type = _tok.at(a + 1);
                if (_tok.at(a + 1) == "original-service") {
                    buff.external_service = data::search_service_object(_tok.at(a + 2));
                }
                else if (_tok.at(a + 1) == "service-group") {
                    parse_virtual_server_service_group(buff, data::search_object_group(_tok.at(a + 2)), ranges);
                }
            }
            else if (_tok.at(a) == "mapped-service") {
                buff.internal_service = data::search_service_object(_tok.at(a + 1));
            }
        }

        /*
        Nebula does not like range ports and single ports in the same entry so we need to
        save the range ports into a seperate buffer and create a new entry with the same ipv4 and interface etc.
        but only the ranges
        */
        if (ranges.size() > 0) {
            virtual_server range_buff;
            for (data::service_object r : ranges) {
                range_buff.internal_service.name = r.name + " ";
                range_buff.internal_service.ports = r.ports + " ";
                range_buff.internal_service.protocol = r.protocol + " ";
                range_buff.internal_service.type = r.type + " ";
            }
            virtual_server_entrys.push_back(range_buff);
        }
    }
    catch (std::exception e) {
        logger::log("\"parse_virtual_server_object_to_struct\" out of range! Tell Teodor!! saved in log0.txt %s, %s\n", e.what(), s.c_str());
        system("pause");
        exit(0);
    }
    return buff;

}

/*SECURE_POLICY PARSING*/

/*INTERFACE PARSING*/
void serialize_config::serialize_interface(std::string iface){
    /*
    Here you can se that we search for interface wan, and include ip address
    Some interfaces just has an object called account. we dont want that
    */
    try {
        for (data::tokenized_stuff s : p.find_block("interface " + iface, "ip address")) {
            //logger::log("%s\n", s.name.c_str());
            wan_lan_interface _buff;
            _buff.name = data::tokenize(s.name).at(1);
            /*
            Nebula has different objects for wan and lan.
            But instead of having different structures for each or polymorphic class.
            We just have and identifier;
            */
            if (_buff.name.find("wan") != std::string::npos)
                _buff.idx = WAN;
            else
                _buff.idx = LAN;
            for (std::string raw : s.raw) {
                if (raw.find("ip address") != std::string::npos)
                    _buff.ip = parse_interface_ip_address(raw);
                if (raw.find("ip gateway") != std::string::npos)
                    _buff.gateway = parse_gateway_ipv4(raw);
                if (raw.find("dhcp-pool") != std::string::npos)
                    _buff.dhcp_pool = parse_dhcp_pool(raw);
            }
            interfaces.push_back(_buff);
        }
    }
    catch (std::exception e) {
        logger::log("\"parse_virtual_server_object_to_struct\" out of range! Tell Teodor!! saved in log0.txt %s, %s\n", e.what(), iface.c_str());
        system("pause");
        exit(0);
    }
}


serialize_config::ip_address_config_style_interface serialize_config::parse_interface_ip_address(std::string s){
    serialize_config::ip_address_config_style_interface buff;
    // ip address 192.168.1.1 255.255.255.0
    try {
        std::vector<std::string> buffer = data::tokenize(s);
        if (buffer.at(2) == "dhcp") {
            buff.type = "DHCP";
            if (buffer.size() > 3) {
                if (buffer.at(3) == "metric")
                    buff.metric = buffer.at(4);
            }
            buff.ipv4 = "192.168.1.1";
            buff.subnet = "255.255.255.0";
        }
        else {
            buff.ipv4 = buffer.at(2);
            buff.subnet = buffer.at(3);
            buff.type = "STATIC";
        }
    }
    catch (std::exception e) {
        logger::log("\"parse_interface_ip_address\" out of range! Tell Teodor!! saved in log0.txt %s, %s\n", e.what(), s.c_str());
        system("pause");
        exit(0);
    }

    return buff;
}

serialize_config::ip_address_config_style_interface serialize_config::parse_gateway_ipv4(std::string s){
    serialize_config::ip_address_config_style_interface buff;
    try {
        std::vector<std::string> buffer = data::tokenize(s);
        // ip gateway 192.168.1.1 metric 0
        buff.ipv4 = buffer.at(2);
        if (buff.ipv4.empty())
            buff.ipv4 = "192.168.1.0";
    }
    catch (std::exception e) {
        logger::log("\"parse_gateway_ipv4\" out of range! Tell Teodor!! saved in log0.txt %s, %s\n", e.what(), s.c_str());
        system("pause");
        exit(0);
    }
    return buff;
}

serialize_config::ip_dhcp_pool_interface serialize_config::parse_dhcp_pool(std::string s){
    ip_dhcp_pool_interface buff;
    /*
    If the interface are NOT a dhcp-server doesnt need a ip address nor pool size
    but nebula does not accept those to be null so here we set it to some default
    data, that will be changed if the interface IS dhcp-server
    */
    try {
        buff.start_address = "192.168.1.33";
        buff.pool_size = "50";
        buff.type = "NONE";
        // ip dhcp-pool LAN1
        s.erase(0, s.find(" ") + 1);
        s.erase(0, s.find(" ") + 1);
        std::vector<data::tokenized_stuff> dhcp_pool = p.find_block(s);
        // Only want one object in here FOR NOW! TEMP FOR TESTING!
        if (dhcp_pool.size() != 1)
            return buff;

        buff.type = "SERVER";

        for (std::string raw : dhcp_pool.at(0).raw) {
            if (raw.find("network") != std::string::npos) {
                raw.erase(0, raw.find(" ") + 1);
                buff.network = raw;
            }
            else if (raw.find("default-router") != std::string::npos) {
                raw.erase(0, raw.find(" ") + 1);
                buff.def_router = raw;
            }
            else if (raw.find("starting-address") != std::string::npos) {
                raw.erase(0, raw.find(" ") + 1);
                buff.start_address = raw.substr(0, raw.find(" "));
                raw.erase(0, raw.find(" ") + 1);
                raw.erase(0, raw.find(" ") + 1);
                buff.pool_size = raw;
            }
            else if (raw.find("first-dns-server") != std::string::npos) {
                raw.erase(0, raw.find(" ") + 1);
                buff.dns_server1 = raw;
            }
            else if (raw.find("second-dns-server") != std::string::npos) {
                raw.erase(0, raw.find(" ") + 1);
                buff.dns_server2 = raw;
            }
            else if (raw.find("lease") != std::string::npos) {
                raw.erase(0, raw.find(" ") + 1);
                if (raw.size() >= 5) {
                    buff.lease += raw.at(0);
                    buff.lease += raw.at(2);
                    buff.lease += raw.at(4);
                }
                else {
                    logger::log("Lease for %s, could not be parsed! using default 2,0,0\n", s.c_str());
                    buff.lease = "200";
                }
            }
        }
    }
    catch (std::exception e) {
        logger::log("\"parse_dhcp_pool\" out of range! Tell Teodor!! saved in log0.txt %s, %s\n", e.what(), s.c_str());
        system("pause");
        exit(0);
    }
    return buff;
}
/*INTERFACE PARSING*/


/*VPN PARSING*/

serialize_config::isakmp_vpn serialize_config::parse_isakmap(data::tokenized_stuff map){
    isakmp_vpn _tmp;
    std::string current_parse = "";
    try {
        _tmp.name = data::tokenize(map.name).at(2);

        for (std::string raw : map.raw) {
            if (raw == "activate" || raw == "deactivate") {
                current_parse = "activate/deactivate";
                _tmp.state = raw;
            }
            else if (raw.find("local-ip") != std::string::npos) {
                current_parse = "local-ip";
                ip_address_config_style_interface ip = get_interface(data::tokenize(raw).at(2)).ip;
                _tmp.local_ipv4 = ip;
            }
            else if (raw.find("peer-ip") != std::string::npos) {
                current_parse = "peer-ip";
                ip_address_config_style_interface iacsi;
                iacsi.ipv4 = data::tokenize(raw).at(1);
                iacsi.subnet = data::tokenize(raw).at(2);
                _tmp.peer_ipv4 = iacsi;
            }
            else if (raw.find("authentication") != std::string::npos) {
                current_parse = "authentication";
                _tmp.auth = data::tokenize(raw).at(1);
            }
            else if (raw.find("local-id") != std::string::npos) {
                current_parse = "local-id";
                _tmp.local_id = data::tokenize(raw).at(1);  //TEMPORARY, DONT THINK THIS IS USED
            }
            else if (raw.find("peer-id") != std::string::npos) {
                current_parse = "peer-id";
                _tmp.peer_id = data::tokenize(raw).at(1); //TEMPORARY, DONT THINK THIS IS USED
            }
            else if (raw.find("fall-back-check-interval") != std::string::npos) {
                current_parse = "fall-back-check-interval";
                _tmp.fall_back_check = data::tokenize(raw).at(1);
            }
            else if (raw.find("lifetime") != std::string::npos) {
                current_parse = "lifetime";
                _tmp.life_time = data::tokenize(raw).at(1);
            }
            else if (raw.find("group2") != std::string::npos) {
                current_parse = "group2";
                _tmp.group = raw;  //TODO: CHECK THIS ONE! MIGHT BE DIFFIE HELLMAN
            }
            else if (raw.find("transform-set") != std::string::npos) {
                current_parse = "transform-set";
                _tmp.transform_set = data::tokenize(raw).at(1);
            }
            else if (raw.find("mode") != std::string::npos) {
                current_parse = "mode";
                _tmp.mode = data::tokenize(raw).at(1);
                std::transform(_tmp.mode.begin(), _tmp.mode.end(), _tmp.mode.begin(), ::toupper);
            }
            else if (raw.find("dpd-interval") != std::string::npos) {
                current_parse = "dpd-interval";
                _tmp.dpd_interval = data::tokenize(raw).at(1);
            }
        }
    }
    catch(std::exception e) {
        logger::log("\"parse_isakmap\" out of range! Tell Teodor!! saved in log0.txt %s, %s, %s\n", e.what(), current_parse.c_str(), map.name.c_str());
        system("pause");
        exit(0);
    }
    return _tmp;
}

serialize_config::crypto_vpn serialize_config::parse_crypto_map(data::tokenized_stuff map){
    crypto_vpn _tmp;
    std::string current_parse = "";
    try {
        _tmp.name = data::tokenize(map.name).at(2);

        for (std::string raw : map.raw) {
            if (raw == "activate" || raw == "deactivate") {
                current_parse = "activate/deactivate";
                _tmp.state = raw;
            }
            else if (raw.find("adjust-mss") != std::string::npos) {
                current_parse = "adjust-mss";
                _tmp.adjust_mss = data::tokenize(raw).at(1);
            }
            else if (raw.find("ipsec-isakmp") != std::string::npos) {
                current_parse = "ipsec-isakmp";
                _tmp.ipsec_isakmp = parse_isakmap(p.find_single_block("policy " + data::tokenize(raw).at(1)));
            }
            else if (raw.find("scenario") != std::string::npos) {
                current_parse = "scenario";
                _tmp.scenario = data::tokenize(raw).at(1);
            }
            else if (raw.find("encapsulation") != std::string::npos) {
                current_parse = "encapsulaton";
                _tmp.encapsulation = data::tokenize(raw).at(1);
            }
            else if (raw.find("transform-set") != std::string::npos) {
                current_parse = "transform-set";
                _tmp.transform_set = data::tokenize(raw).at(1);
            }
            else if (raw.find("security-association") != std::string::npos) {
                current_parse = "security-association";
                _tmp.secure_associtaion = data::tokenize(raw).at(4);
            }
            else if (raw.find("pfs") != std::string::npos) {
                current_parse = "pfs";
                _tmp.pfs = data::tokenize(raw).at(2);
            }
            else if (raw.find("local-policy") != std::string::npos) {
                current_parse = "local-policy";
                _tmp.local_policy = data::search_address_object(data::tokenize(raw).at(1));
                // If the local policy are an interface, we need to find the interface and link to it.
                if (_tmp.local_policy.ipv4.find("lan") != std::string::npos) {
                    wan_lan_interface wlint = get_interface(_tmp.local_policy.ipv4);
                    _tmp.local_policy.ipv4 = wlint.ip.ipv4;
                    _tmp.local_policy.name = wlint.name;
                }
            }
            else if (raw.find("remote-policy") != std::string::npos) {
                current_parse = "remote-policy";
                _tmp.remote_policy = data::search_address_object(data::tokenize(raw).at(1));
            }
            else if (raw.find("conn-check") != std::string::npos) {
                current_parse = "conn-check";
                _tmp.conn_check = data::tokenize(raw).at(2);
            }
        }
    }
    catch (std::exception e) {
        logger::log("\"parse_crypto_map\" out of range! Tell Teodor!! saved in log0.txt %s, %s, %s\n", e.what(), current_parse.c_str(), map.name.c_str());
        system("pause");
        exit(0);
    }
    return _tmp;
}

/*VPN PARSING*/

void serialize_config::parse_virtual_server_service_group(virtual_server &buff, std::vector<data::service_object> tmp, std::vector<data::service_object> &ranges) {
    bool f_range = false, f_eq = false;
    for(data::service_object type_check : tmp){
        if(type_check.type == "range")
            f_range = true;
        if(type_check.type == "eq")
            f_eq = true;
    }
    /*
    Nebula does not like single ports and port ranges in the same virtual server. 
    so we need to seperate them and create a new virtual server entry
    */
    if(f_range && f_eq){
        for(data::service_object _types : tmp){
            if(_types.type == "range")
                ranges.push_back(_types);
            else if(_types.type == "eq"){
                buff.internal_service.name += _types.name + " ";
                buff.internal_service.ports += _types.ports + " ";
                buff.internal_service.protocol += _types.protocol + " ";
                buff.internal_service.type += _types.type + " ";
            }
        }
        /*remove the last space from the loop above!*/
        buff.internal_service.name.pop_back();
        buff.internal_service.ports.pop_back();
        buff.internal_service.protocol.pop_back();
        buff.internal_service.type.pop_back();
    }else {
        /*All objects in the service-object structure are initialized with "NULL" we need to "reset" the variables*/
        buff.internal_service.name = "";
        buff.internal_service.ports = "";
        buff.internal_service.protocol = "";
        buff.internal_service.type = "";
        for(data::service_object _types : tmp){
            //Jlogger::log("%s\n", _types.name.c_str());
            buff.internal_service.name += _types.name + " ";
            buff.internal_service.ports += _types.ports + " ";
            buff.internal_service.protocol += _types.protocol + " ";
            buff.internal_service.type += _types.type + " ";
        }
        buff.internal_service.name.pop_back();
        buff.internal_service.ports.pop_back();
        buff.internal_service.protocol.pop_back();
        buff.internal_service.type.pop_back();
    }
}

/*PRIVATE*/