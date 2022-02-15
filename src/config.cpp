#include "config.h"

void json2config(nlohmann::json &j, struct config_t &conf)
{
    j.at("chat_id").get_to(conf.chat_id);
    j.at("token").get_to(conf.token);
    for (auto &element : j["repos"])
    {
        conf.repos.push_back(element.get<std::string>());
    }
}

void get_config(struct config_t &conf)
{

    std::ifstream conf_in(CONFIG_PATH);
    if (conf_in.is_open())
    {
        nlohmann::json config_json;
        conf_in >> config_json;
        json2config(config_json, conf);
    }
    else
    {
        std::cerr << "telgram-bot : cannot access \'" << CONFIG_PATH << "\' : " << strerror(errno) << '\n';
        exit(1);
    }
}