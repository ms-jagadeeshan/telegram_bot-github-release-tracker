/*!
* \file config.h
* \author Name <jagadeeshanmsj@gmail.com>
* \version 0.5
* \date 16/02/2022
* \brief header file definitions of configs
* \remarks None
*
*
*
*/


#ifndef TELEGRAM_BOT_CONFIG_H
#define TELEGRAM_BOT_CONFIG_H
/*! Importation of librairies*/
#include <iostream>
#include <vector>
#include <fstream>
#include "json.hpp"

/*!
* \def CONFIG_PATH
* Description: Configuration file path
*/
#define CONFIG_PATH "../config.json"

/*!
* \def TMP_FILE_PATH
* Temporary file path
*/
#define TMP_FILE_PATH "/tmp/telegram-bot-cachedkjafldfklalg.json"

/*!
* \def DATA_FOLDER_PATH
* Path of folder, where latest version tag stored
*/
#define DATA_FOLDER_PATH "../data/"

/*!
* \def FETCH_INTERVAL
* Interval to fetch latest tags(in seconds)
*/
#define FETCH_INTERVAL 120

/**
 * @brief Struct to store configuration
 * 
 */
struct config_t
{
    std::string chat_id;            // Chat id to be notified
    std::string token;              // Bot token
    std::string personal_token;     // Github personal token to increase api limit
    std::vector<std::string> repos; // Repos link to be monitored
};

/**
 * @brief Get the config struct from json file
 * 
 * @param conf
 */
void get_config(struct config_t &conf);

#endif
