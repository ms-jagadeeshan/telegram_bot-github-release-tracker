#ifndef TELEGRAM_BOT_CONFIG_H
#define TELEGRAM_BOT_CONFIG_H
#include <iostream>
#include <vector>
#include <fstream>
#include "json.hpp"

// Configuration file path
#define CONFIG_PATH "../config.json"

// Temporary file path
#define TMP_FILE_PATH "/tmp/telegram-bot-cachedkjafldfklalg.json"

// Path of folder, where latest version tag stored
#define DATA_FOLDER_PATH "../data/"

// Struct to store config
struct config_t
{
    std::string chat_id;            // Chat id to be notified
    std::string token;              // Bot token
    std::vector<std::string> repos; // Repos link to be monitored
};

// Gets config from json and converts to struct
void get_config(struct config_t &conf);

#endif