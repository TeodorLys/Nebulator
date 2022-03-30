#pragma once
#include <string>
#include <vector>
#include "data.h"
class parser {
private:
    std::vector<std::string> _buffer;
public:
    
    void tokenize_file(std::string path);
    std::vector<data::tokenized_stuff> find_block(std::string name, std::string include = "");
public:
    /*HACK!*/
    data::tokenized_stuff find_single_block(std::string name){
        std::vector<data::tokenized_stuff> buff = find_block(name);
        if(buff.size() == 1)
            return buff[0];
        return data::tokenized_stuff();
    }
private:
    void token_block();
    void token_object_list();
};