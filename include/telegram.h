#ifndef TELEGRAM_H
#define TELEGRAM
#include <tgbot/tgbot.h>
#include "config.h"

void telegram_message_send(struct config_t &conf,  std::string &msg_text);

#endif