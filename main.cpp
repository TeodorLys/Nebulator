#include "request_compiler.h"
#include "stupid_powershell.h"
#include "config_parser/serialize_config.h"
#include "translation/translator.h"
#include "verification.h"
#include <Windows.h>
#include <chrono>
#include "config_parser/data.h"

/*

TODO:

VPN check: So two ipaddresses arent used by two different vpn entries
single range ports for nat: Sometimes the port config on the nat are range but only ex. 6000-6000 this needs to be a translated into a single port entry

Comment everything!
Do some general error checking! 
Should have done that from the start but, nooooo im not that smart.....

Make a polymorphic class for different parsing sections, one for NAT one for VPN etc...


*/
int main(int argc, char** argv){

    CHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileNameA( NULL, buffer, MAX_PATH );
    data::current_path = buffer;
    data::current_path.erase(data::current_path.find_last_of("\\"), data::current_path.length());
    data::current_path.erase(data::current_path.find_last_of("\\"), data::current_path.length());  // TEMP
    printf("Checking dependencies...\n");
    verification verify;
    if(!verify.verify_scripts()){
        printf("Could not verify dependencies... contact Teodor...exiting\n");
        system("pause");
        exit(0);
    }
    printf("OK!\n");

    printf("Loading Authentication...\n");
    std::string script = "lua " + data::current_path  + "\\dep\\lua\\auth_call.lua"; 
    system(script.c_str());
    std::fstream f(data::current_path + "\\dep\\conf\\auth.txt", std::fstream::in);
    std::getline(f, data::auth_token);
    f.close();
    printf("OK!\n");

    script = "lua " + data::current_path  + "\\dep\\lua\\sel_org.lua"; 
    system(script.c_str());
    f.open(data::current_path + "\\dep\\conf\\org_site.txt", std::fstream::in);
    std::string org_site;
    std::getline(f, org_site);
    f.close();

    printf("Downloading source files...\n");
    script = "lua " + data::current_path  + "\\dep\\lua\\source_get.lua"; 
    system(script.c_str());
    printf("OK!\n");

    printf("Downloading startup-config...\n");
    printf("Enter Address, user and password for the firewall\n");
    script = "lua " + data::current_path  + "\\dep\\lua\\download_conf.lua";
    system(script.c_str());
    std::fstream f_conf("\\dep\\conf\\startup-config.conf", std::fstream::in);
    std::string conf_check;
    std::getline(f_conf, conf_check);
    if(conf_check == "nope"){
        printf("Could not download startup config... exiting\n");
        system("pause");
        exit(0);
    }
    f_conf.close();
    printf("OK!\n");


    request_compiler rc;
    data::current_org.org_id = data::tokenize(org_site)[0];
    data::current_org.site_id = data::tokenize(org_site)[1];

    //system("pause");
    stupid_powershell sp;
    serialize_config conf;
    conf.serialize();
    translator trans;
    printf("Translating interfaces...\n");
    trans.translate_interface();
    printf("OK!\n");
    printf("Translating virtual servers...\n");
    trans.translate_nat_virtual_server();
    printf("OK!\n");
    printf("Translating site_to_site VPN...\n");
    trans.translate_vpn();
    printf("OK!\n");
    printf("Sending interface config to Nebula...");
    sp.call_stupid_powershell(1, data::current_org);
    printf("Sending NAT config to Nebula...");
    sp.call_stupid_powershell(2, data::current_org);
    printf("Sending VPN config to Nebula...");
    sp.call_stupid_powershell(3, data::current_org);
}
