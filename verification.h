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

        if(!exists(".\\dep\\lua\\ps.exe"))
            return false;
        if(!exists(".\\dep\\conf"))
            create_conf_folder();
        return true;
    }
    bool exists (const std::string& name) {
        struct stat buffer;   
        return (stat (name.c_str(), &buffer) == 0); 
    }
private:    
    bool create_conf_folder(){
        _wmkdir(L".\\dep\\conf");
        return true;
    }


};