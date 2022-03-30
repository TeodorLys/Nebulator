#include "parser.h"
#include <fstream>
#include <algorithm>

void parser::tokenize_file(std::string path){
    data::blocks.clear();
    data::objects.clear();
    std::fstream file(path, std::fstream::in);
    std::string buffer;
    while(!file.eof()){
        std::getline(file, buffer);
        // In zyxels configs, each object are seperated by a "!"
        // So while we dont se a ! at the begining the line
        // we push the line to the parse buffer
        if(buffer != "!"){
            _buffer.push_back(buffer);
        } else {
            // We just stumbled upon a "!"
            bool block_token = false;
            // A block is a piece of data that contains a name and objects. 
            // Objects inside a block allways begin with a space " " 
            for(std::string _s : _buffer)
                if(_s[0] == ' '){
                    token_block();
                    block_token = true;
                    break;
                }
            if(!block_token)
                token_object_list();  // Reads the class variable: _buffer
            _buffer.clear();
        }
    }
}

/*
A block 
*/

std::vector<data::tokenized_stuff> parser::find_block(std::string name, std::string include) {
    /*
    Here we look into the blocks buffer and try to find a specified block
    we search by name, but the name is used to search inside the name string
    so if we search for "interface wan"
    then we find:
    "interface wan1", "interface wan1_ppp"
    But we want to find the block containing the ip address.
    so we can include say that the block needs to include and "ip address" object inside the block
    */
    std::vector<data::tokenized_stuff> buffer;
    for(data::tokenized_stuff s : data::blocks){
        if(s.name.find(name) != std::string::npos){
            if(!include.empty()){
                bool found = false;
                for(std::string raw : s.raw)
                    if(raw.find(include) != std::string::npos)
                        found = true;
                if(found){
                    buffer.push_back(s);
                    continue;
                }else
                    continue;
            }

            buffer.push_back(s);
        }
    }
    return buffer;
}

/*PRIVATE*/

/*
This pushes back the raw data. no specific parsing happens here.
The only parsing that happens are splitting the block into name and objects(seperate vector inside the block struct)
*/
void parser::token_block(){
    data::tokenized_stuff _tmp;
    _tmp.name = _buffer[0];
    for(int a = 1; a < _buffer.size(); a++){
        _buffer[a].erase(0, 1);  // Remove the begining space " "
        _tmp.raw.push_back(_buffer[a]);
    }

    // There are a few lookup tables that I want to save in seperate vector
    if(_tmp.name.find("object-group") != std::string::npos)
        data::lookup_object_groups.push_back(_tmp);
    data::blocks.push_back(_tmp);
}


/*
Pushes the raw data containing the object and some lookup table checks
*/
void parser::token_object_list(){
    for(std::string _s : _buffer){
        data::objects.push_back(_s);
        if(_s.find("service-object") != std::string::npos)
            data::lookup_service_objects.push_back(data::tokenize_service_object(_s));
        if(_s.find("address-object") != std::string::npos)
            data::lookup_address_objects.push_back(data::tokenize_address_object(_s));
    }
}


/*PRIVATE*/