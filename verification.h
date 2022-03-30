#pragma once
#include <sys/stat.h>
#include <string>
class verification {
private:
public:

    bool verify_scripts(){
        if(!exists(".\\dep\\powershell\\nebula_auth.ps1"))
            return false;
        if(!exists(".\\dep\\powershell\\download_conf.ps1"))
            return false;
        if(!exists(".\\dep\\powershell\\sel_org_site.ps1"))
            return false;
        if(!exists(".\\dep\\powershell\\script.ps1"))
            return false;
        if(!exists(".\\dep\\powershell\\source_json.ps1"))
            return false;

        if(!exists(".\\dep\\lua\\auth_call.lua"))
            return false;
        if(!exists(".\\dep\\lua\\download_conf.lua"))
            return false;
        if(!exists(".\\dep\\lua\\sel_org.lua"))
            return false;
        if(!exists(".\\dep\\lua\\send.lua"))
            return false;
        if(!exists(".\\dep\\lua\\source_get.lua"))
            return false;
        if(!exists(".\\dep\\conf"))
            create_conf_folder();
        return true;
    }
private:    
    bool exists (const std::string& name) {
        struct stat buffer;   
        return (stat (name.c_str(), &buffer) == 0); 
    }

    bool create_conf_folder(){
        _wmkdir(L".\\dep\\conf");
        return true;
    }


};