#include "request_compiler.h"
#include "translation/translator.h"
#include <thread>
#include "config_parser/data.h"

/*
TODO: if the file is not loaded. we just continue now... dont do that...
*/

/*
Here we parse out the defaults of the nebula interfaces!
Including the interface id:s.
*/
void request_compiler::initialize_data(){ 
    nlohmann::json source;
    try{
        std::fstream f(data::current_path + "\\source\\interface_source.json", std::fstream::in);
        f >> source;
        f.close();
        int size = source["body"]["wan_ports"].size();
        for(int a = 0; a < size; a++)
            interface.push_back(interface_id(source["body"]["wan_ports"][a]["name"], source["body"]["wan_ports"][a]["l2_conf"]["port_group_id"], source["body"]["wan_ports"][a]["id"]));

        size = source["body"]["lan_ports"].size();
        for(int a = 0; a < size; a++)
            interface.push_back(interface_id(source["body"]["lan_ports"][a]["name"], source["body"]["lan_ports"][a]["l2_conf"]["port_group_id"], source["body"]["lan_ports"][a]["id"]));
    }catch(std::exception &e){
        printf("%s\n", e.what());
    }
}

void request_compiler::push_interface_json(wan_lan _wl){
    try{
        nlohmann::json source;
        std::fstream f(data::current_path + "\\source\\interface_source.json", std::fstream::in);

        if(!f.is_open())
            printf("Could not open file! INTERFACE\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        f >> source;
        //Slow down incase of drive speed bottleneck
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        f.close();
        for(wan_lan wl : translator::interfaces){
            std::string iface;
            if(wl.idx == WAN)
                iface = "wan_ports";
            else if(wl.idx == LAN)
                iface = "lan_ports";

            int size = source["body"][iface].size();
            for(int a = 0; a < size; a++){
                std::string _name = source["body"][iface][a]["name"];
                if(wl.name == _name){
                    if(wl.idx == WAN){
                        source["body"][iface][a]["l3_conf"]["gateway"] = wl.gateway;
                        source["body"][iface][a]["l3_conf"]["ip_type"] = wl.ip_type;
                    }
                    if(wl.idx == LAN){
                        source["body"][iface][a]["ipv4_dhcp_pool"]["ipv4_address_start"] = wl.ipv4_start;
                        source["body"][iface][a]["ipv4_dhcp_pool"]["lease_time"] = wl.lease;
                        source["body"][iface][a]["ipv4_dhcp_pool"]["size"] = wl.pool_size;
                        source["body"][iface][a]["ipv4_dhcp"]["mode"] = wl.type;
                    }
                    source["body"][iface][a]["l3_conf"]["ipv4_address"] = wl.ipv4;
                    source["body"][iface][a]["l3_conf"]["ipv4_subnet_mask"] = wl.submask;
                    break;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        f.open(data::current_path + "\\source\\interface.json", std::fstream::out);
        if(!f.is_open())
            printf("Could not open file! INTERFACE\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        f << source["body"];
        //Slow down incase of drive speed bottleneck
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        f.close();
        //last_json = source["body"].dump().c_str();
        //printf("%s\n\n\n", source["body"].dump(4).c_str());
    }catch(std::exception &e){
        printf("%s\n", e.what());
    }        
}


void request_compiler::push_nat_json(nat_vs v){
    try{
        nlohmann::json source;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::fstream f(data::current_path + "\\source\\nat_source.json", std::fstream::in);
        if(!f.is_open())
            printf("Could not open file! NAT\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        f >> source;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        f.close();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        for(nat_vs n : translator::nat){
            source["body"]["ipv4_address_translation"]["virtual_server"] += {
                {"enabled", n.enabled},
                {"wan_iface", n.wan_iface},
                {"protocol", n.protocol},
                {"public_ipv4", n.pub_ipv4},
                {"public_ports", n.pub_ports},
                {"private_ipv4", n.priv_ipv4},
                {"private_ports", n.priv_ports},
                {"remote_ipv4_list", n.remote_ipv4_list},
                {"description", n.desc}
            };
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        f.open(data::current_path + "\\source\\nat.json", std::fstream::out);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        f << source["body"];
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        f.close();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }catch(std::exception &e){
        printf("%s\n", e.what());
    }
}


void request_compiler::push_sts_vpn_json(sts_vpn vpn){
    try{
        nlohmann::json source;
        std::fstream f(data::current_path + "\\source\\vpn_source.json", std::fstream::in | std::fstream::binary);
        if(!f.is_open()){
            printf("Could not open file! VPN\n");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        f >> source;
        f.close();
        for(sts_vpn _vpn : translator::vpn){
            source["body"]["vpn_to_gateways"] += {
                {"enabled", _vpn.enabled},
                {"remote_name", _vpn.name},
                {"remote_address", _vpn.remote_address},
                {"remote_cidr_list", _vpn.remote_cidr_list},
                {"address", _vpn.address},
                {"availability", _vpn.availability},
                {"custom_availability", {{{"site_id", data::current_org.site_id}, {"device_id", nullptr}, {"remote_name", nullptr}}}},
                {"pre_shared_key", _vpn.pre_shared_key},
                {"ipsec_preset", _vpn.ipsec_preset},
                {"ipsec_preset_fe", "Default"},
                {"ipsec_proposal", _vpn.ipsec_proposal},
                {"ipsec_ike_diffle_hellman", _vpn.ipsec_ike_diffie},
                {"ipsec_lifetime", _vpn.ipsec_lifetime},
                {"ipsec_esp", _vpn.ipsec_esp},
                {"ipsec_asso_lifetime", _vpn.ipsec_asso_lifetime},
                {"ipsec_pfs_diffle_hellman", _vpn.ipsec_pfs_diffie},
                {"ipsec_negotiation_mode", _vpn.ipsec_negotiation_mode},
                {"ipsec_local_id", _vpn.ipsec_local_id},
                {"ipsec_peer_id", _vpn.ipsec_peer_id},
                {"ipsec_ike_version", _vpn.ipsec_ike_version},
                {"ipsec_vti", _vpn.ipsec_vti},
                {"connectivity_check", _vpn.conn_check}
            };
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        f.open(data::current_path + "\\source\\vpn.json", std::fstream::out);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        f << source["body"];
        f.close();
    }catch(std::exception &e){
        printf("%s\n", e.what());
    }
}