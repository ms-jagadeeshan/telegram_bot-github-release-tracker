# Telegram Bot
This is Telegram bot written in C++, it will send message in telegram when there is new release out from your favorite repo in github.    
This program is inspired from [git_release_tracker](https://github.com/dracarys18/git_release_tracker) written in rust by [dracarys18](https://github.com/dracarys18)

# Dependency 
It needs [telegram bot c++ api](https://github.com/reo7sp/tgbot-cpp).    
Install that using below commands
```sh
sudo apt-get install g++ make binutils cmake libssl-dev libboost-system-dev zlib1g-dev libcurl4-openssl-dev
git clone https://github.com/reo7sp/tgbot-cpp
cd tgbot-cpp
cmake .
make -j4
sudo make install
```

# Install from source
Clone this repo and compile.
```sh
git clone https://github.com/ms-jagadeeshan/telegram_bot-github-release-tracker.git
cd telegram_bot-github-release-tracker
make all
```
Rename config-sample.json to config.json and fill the chat id, token,repos to be monitored    

`mv config-sample.json config.json`
```json
{
    "chat_id": "",     // Enter the chat id to be notified
    "token": "",       // Enter your bot token(stored locally,so don't have to worry)
    "repos": [         // add the repos you want to track
        "ms-jagadeeshan/telegram_bot-github-release-tracker",
        "torvalds/linux"
    ],
    "github_personal_token":""  // Format:  "<github_username>:token"
}
```
Go to telegram and https://t.me/botfather to get bot token from telegram.    
And https://t.me/rawdatabot to get you chat id.    
After filling chat id and token just run this to start the bot
```sh
make run
# or
# cd bin && ./telegram_bot-github-release-tracker
```

# Customizing
In [config.h](./include/config.h) header file paths are defined, you can change them and compile
```c++
// Configuration file path
#define CONFIG_PATH "../config.json"

// Temporary file path
#define TMP_FILE_PATH "/tmp/telegram-bot-cachedkjafldfklalg.json"

// Path of folder, where latest version tag stored
#define DATA_FOLDER_PATH "../data/"
```

# Things to be noted
- We can use github api only 60 times a hour without github user authentication. So generate your github token from [here](https://github.com/settings/tokens)
- Note `"github_personal_token"` should be filled in the format of `"yourusername:token"`

