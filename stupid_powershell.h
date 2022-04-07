/*
HACK!!
*/


#pragma once
#include <string>
#include "request_compiler.h"
#include "config_parser/data.h"
//https://ccapi.nebula.zyxel.com/nebula/v9/organization/623acc482e139e19872ea7b9/site/623acc6e6426b7e745f755b9/gw-flex/interface
class stupid_powershell {
    std::string interface_url = "https://ccapi.nebula.zyxel.com/nebula/v9/organization/*ORG*/site/*SITE*/gw-flex/interface";
    std::string vpn_url = "https://ccapi.nebula.zyxel.com/nebula/v9/organization/*ORG*/site/*SITE*/gw-flex/site-to-site-vpn";
    std::string nat_url = "https://ccapi.nebula.zyxel.com/nebula/v9/organization/*ORG*/site/*SITE*/gw-flex/nat";
    std::string interface_json_file_path = "source\\interface.json";
    std::string vpn_json_file_path = "source\\vpn.json";
    std::string nat_json_file_path = "source\\nat.json";
    std::string default_cookies = "$body = Get-Content -raw -Path '*JSON*'\n"
                                  "$url = '*URL*'\n"
                                  "$session = New-Object Microsoft.PowerShell.Commands.WebRequestSession;\n" \
                                  "$session.UserAgent = \"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.74 Safari/537.36\";\n" \
                                  "$ErrorActionPreference = \"silentlycontinue\"\n" \
                                  "$raw = Invoke-WebRequest -UseBasicParsing -Uri $url -ErrorAction SilentlyContinue -ErrorVariable e `\n" \
                                  "-Method \"PUT\" `\n" \
                                  "-WebSession $session `\n "\
                                    "-Headers @{\n "\
                                    "\"method\"=\"PUT\"\n "\
                                    "\"authority\"=\"ccapi.nebula.zyxel.com\" \n"\
                                    "\"scheme\"=\"https\" \n"\
                                    "\"path\"=\"/nebula/v9/organization/*pORG*/site/*pSITE*/gw-flex/*pLOC*\" \n"\
                                    "\"sec-ch-ua\"=\"`\" Not A;Brand`\";v=`\"99`\", `\"Chromium`\";v=`\"99`\", `\"Google Chrome`\";v=`\"99`\"\" \n"\
                                    "\"sec-ch-ua-mobile\"=\"?0\" \n"\
                                    "\"accept\"=\"application/json, text/plain, */*\"\n "\
                                    "\"x-auth-renew\"=\"true\" \n"\
                                    "\"x-refer-page\"=\"/*rORG*/*rSITE*/usg/configure/*rLOC*\" \n"\
                                    "\"x-auth-token\"=\"*AUTH*\" \n"\
                                    "\"sec-ch-ua-platform\"=\"`\"Windows`\"\" \n"\
                                    "\"origin\"=\"https://nebula.zyxel.com\"\n "\
                                    "\"sec-fetch-site\"=\"same-site\" \n"\
                                    "\"sec-fetch-mode\"=\"cors\" \n"\
                                    "\"sec-fetch-dest\"=\"empty\"\n "\
                                    "\"referer\"=\"https://nebula.zyxel.com/\"\n"\
                                    "\"accept-encoding\"=\"gzip, deflate, br\" \n"\
                                    "\"accept-language\"=\"sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7\" \n"\
                                    "} `\n" \
                                    "-ContentType \"application/json\" `\n" \
                                    "-Body $body;\n" \
                                    "$raw.statusdescription\n" \
                                    "foreach($a in $e){\n" \
                                    "\"[POWERSHELL]: \" + $a.message.tostring() | add-content \"log0.txt\"\n" \
                                    "}";


public:
    void call_stupid_powershell(int idx, data::organisation org){
        std::fstream f(data::current_path + "\\dep\\powershell\\script.ps1", std::fstream::out);
        std::string buff = default_cookies;
        std::string _url;
        switch(idx){
            case 1:
            _url = format_url(org, interface_url);
            break;
            case 2:
            _url = format_url(org, nat_url);
            break;
            case 3:
            _url = format_url(org, vpn_url);
            break;
        }
        replace_in_string("*URL*", _url, buff);
        switch(idx){
            case 1:
            replace_in_string("*JSON*", interface_json_file_path, buff);
            break;
            case 2:
            replace_in_string("*JSON*", nat_json_file_path, buff);
            break;
            case 3:
            replace_in_string("*JSON*", vpn_json_file_path, buff);
            break;
        }
        replace_in_string("*pORG*", data::current_org.org_id, buff);
        replace_in_string("*pSITE*", data::current_org.site_id, buff);
        replace_in_string("*rORG*", data::current_org.org_id, buff);
        replace_in_string("*rSITE*", data::current_org.site_id, buff);
        switch(idx){
            case 1:
            replace_in_string("*pLOC*", "interface",buff);
            replace_in_string("*rLOC*", "interface",buff);
            break;
            case 2:
            replace_in_string("*pLOC*", "nat",buff);
            replace_in_string("*rLOC*", "nat",buff);
            break;
            case 3:
            replace_in_string("*pLOC*", "site-to-site-vpn",buff);
            replace_in_string("*rLOC*", "site-to-site-vpn",buff);
            break;
        }
        replace_in_string("*AUTH*", data::auth_token, buff);
        f << buff;
        f.close();
        std::string script = data::current_path  + "\\dep\\lua\\ps.exe -send"; 
        system(script.c_str());
    }


    std::string format_url(data::organisation org, std::string _tmp){
        std::string _url = _tmp;
        int _org = _url.find("*ORG*");
        _url.erase(_org, 5);
        _url.insert(_org, org.org_id);
        int _site = _url.find("*SITE*");
        _url.erase(_site, 6);
        _url.insert(_site, org.site_id);
        //logger::log("%s\n", _url.c_str());
        return _url;
    }

    void replace_in_string(std::string to_replace, std::string replace_with, std::string &ref){
        int i = ref.find(to_replace);
        ref.erase(ref.find(to_replace), to_replace.size());
        ref.insert(i, replace_with);
    }

};