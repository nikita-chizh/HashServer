//
// Created by Никита on 2019-08-18.
//
#include <fstream>
#include <streambuf>
#include "nlohmann/json.hpp"
#include "Config.h"

using json = nlohmann::json;

Config::Config(const std::string &pathToConfig){
    init(pathToConfig);
}

Config::Config(std::string &&pathToConfig){
    init(pathToConfig);
}

void Config::init(const std::string &pathToConfig){
    std::ifstream t(pathToConfig);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    auto cfg = json::parse(str);

    cfg.at("port").get_to(port);
    cfg.at("numberOfAcceptors").get_to(numberOfAcceptors);
    cfg.at("hashFunction").get_to(hashFunction);
}
