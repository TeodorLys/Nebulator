#include "request_compiler.h"
#include "stupid_powershell.h"
#include "config_parser/serialize_config.h"
#include "translation/translator.h"
#include "verification.h"
#include <Windows.h>
#include <chrono>
#include <thread>
#include "config_parser/data.h"
#include <iostream>
#include <filesystem>
#include "logger.h"

std::string openfilename() {


  OPENFILENAMEA ofn;
  char fileName[MAX_PATH] = "";
  ZeroMemory(&ofn, sizeof(ofn));

  ofn.lStructSize = sizeof(OPENFILENAMEA);
  ofn.hwndOwner = NULL;
  ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
  ofn.lpstrFile = fileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt = "";

  std::string fileNameStr;

  if ( GetOpenFileNameA(&ofn) )
    fileNameStr = fileName;

  return fileNameStr;
}

/*

TODO:

VPN check: So two ipaddresses arent used by two different vpn entries
single range ports for nat: Sometimes the port config on the nat are range but only ex. 6000-6000 this needs to be a translated into a single port entry

Comment everything!
Do some general error checking! 
Should have done that from the start but, nooooo im not that smart.....

Make a polymorphic class for different parsing sections, one for NAT one for VPN etc...
*/
int main(int argc, char** argv) {
    data::current_path = std::filesystem::current_path().string();
    logger::log("{BEGINING}\n");
    logger::log("Checking dependencies...\n");
    verification verify;
    if (!verify.verify_scripts()) {
        logger::log("Could not verify dependencies... contact Dev...exiting\n");
        logger::log("{END}\n");
        system("pause");
        exit(0);
    }

    logger::log("OK!\n");
    logger::log("Checking execution policy...\n");
    std::string script = data::current_path + "\\dep\\lua\\ps.exe -exec";
    system(script.c_str());
    if (verify.exists(data::current_path + "\\dep\\conf\\nope.txt")) {
        logger::log("Failed!...exiting\n");
        remove(std::string(data::current_path + "\\dep\\conf\\nope.txt").c_str());
        logger::log("{END}\n");
        system("pause");
        logger::safe_exit(0);
    }
    logger::log("OK!\n");

    logger::log("Loading Authentication...\n");
    script = data::current_path + "\\dep\\lua\\ps.exe -auth";
    system(script.c_str());
    std::fstream f(data::current_path + "\\dep\\conf\\auth.txt", std::fstream::in);
    std::getline(f, data::auth_token);
    f.close();
    if (data::auth_token.empty()) {
        logger::log("Could not find authentication token...exiting\n");
        logger::log("{END}\n");
        system("pause");
        exit(0);
    }
    logger::log("OK!\n");

    script = data::current_path + "\\dep\\lua\\ps.exe -sel_org";
    system(script.c_str());
    f.open(data::current_path + "\\dep\\conf\\org_site.txt", std::fstream::in);
    std::string org_site;
    std::getline(f, org_site);
    f.close();

    logger::log("Downloading source files...\n");
    script = data::current_path + "\\dep\\lua\\ps.exe -json";
    system(script.c_str());
    logger::log("OK!\n");

    logger::log("Downloading startup-config...\n");
    printf("Enter Address, user and password for the firewall\n");
    script = data::current_path + "\\dep\\lua\\ps.exe -conf";
    system(script.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::fstream f_conf(data::current_path + "\\dep\\conf\\startup-config.conf", std::fstream::in);
    std::string conf_check;
    std::getline(f_conf, conf_check);
    if (conf_check == "nope" || conf_check.empty()) {
        logger::log("Could not download startup config\n");
        logger::log("Want to load from drive? [Y/N]: ");
        std::string input;
        std::getline(std::cin, input);
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == "y") {
            data::startup_conf = openfilename();
            // openfilename changes the working directory, so here we change it back
            std::wstring p_tmp = std::wstring(data::current_path.begin(), data::current_path.end());
            _wchdir(p_tmp.c_str());
            if (data::startup_conf.empty()) {
                logger::log("Could not load startup config from drive... exiting\n");
                logger::log("{END}\n");
                system("pause");
                exit(0);
            }
        }
        else {
            logger::log("{END}\n");
            system("pause");
            exit(0);
        }
    }
    f_conf.close();
    logger::log("OK!\n");


    request_compiler rc;
    data::current_org.org_id = data::tokenize(org_site)[0];
    data::current_org.site_id = data::tokenize(org_site)[1];

    //system("pause");
    stupid_powershell sp;
    serialize_config conf;
    conf.serialize();
    translator trans;
    logger::log("Translating interfaces...\n");
    trans.translate_interface();
    logger::log("OK!\n");
    logger::log("Translating virtual servers...\n");
    trans.translate_nat_virtual_server();
    logger::log("OK!\n");
    logger::log("Translating site_to_site VPN...\n");
    trans.translate_vpn();
    logger::log("OK!\n");
    logger::log("\nTHIS WILL BE SENT!(BELOW)\n");
    trans.display_data();
    logger::log("Sending interface config to Nebula...\n");
    sp.call_stupid_powershell(1, data::current_org);
    logger::log("Sending NAT config to Nebula...\n");
    sp.call_stupid_powershell(2, data::current_org);
    logger::log("Sending VPN config to Nebula...\n");
    sp.call_stupid_powershell(3, data::current_org);
    logger::log("{END}\n\n");
    system("pause");
}