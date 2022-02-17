#include "telegram.h"

void telegram_message_send(struct config_t &conf, std::string &msg_text)
{
    TgBot::Bot bot(conf.token);
    TgBot::Message::Ptr message;
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message)
                              { bot.getApi().sendMessage(message->chat->id, "Hi!"); });
    bot.getApi().sendMessage(conf.chat_id, msg_text, false, 0, std::make_shared<TgBot::GenericReply>(), "HTML");

    // try
    // {
    //     TgBot::TgLongPoll longPoll(bot);
    //     while (true)
    //     {
    //         printf("Long poll started\n");

    //         longPoll.start();
    //     }
    // }
    // catch (TgBot::TgException &e)
    // {
    //     printf("error: %s\n", e.what());
    // }
}

//int main()
//{
// bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message)
//                           { bot.getApi().sendMessage(message->chat->id, "Hi!"); });
// bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message)
//                              {
//                                  printf("User wrote %s\n", message->text.c_str());
//                                  if (StringTools::startsWith(message->text, "/start"))
//                                  {
//                                      return;
//                                  }
//                                  bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
//                              });
// try
// {
//     printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
//     TgBot::TgLongPoll longPoll(bot);
//     while (true)
//     {
//         printf("Long poll started\n");

//         longPoll.start();
//     }
// }
// catch (TgBot::TgException &e)
// {
//     printf("error: %s\n", e.what());
// }
//}
//