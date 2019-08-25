//
// Created by Никита on 2019-08-18.
//
#include <fstream>
#include <streambuf>
#include "rapidjson/document.h"
#include "Config.h"

using namespace rapidjson;

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

    Document cfg;
    cfg.Parse<0>(str.data());
    port = cfg["port"].GetInt();
    numberOfAcceptors = cfg["numberOfAcceptors"].GetInt();
    numberOfProcessors = cfg["numberOfProcessors"].GetInt();
    hashFunction = cfg["hashFunction"].GetString();
}
