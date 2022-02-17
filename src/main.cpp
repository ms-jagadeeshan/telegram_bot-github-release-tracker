#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include "config.h"
#include "util.h"
#include "telegram.h"

void check_conf(struct config_t &conf)
{
    if (conf.chat_id.empty())
    {
        std::cerr << "Chat id is still empyt!\n";
        exit(1);
    }
    if (conf.token.empty())
    {
        std::cerr << "Token is still empyt!\n";
        exit(1);
    }
    if (conf.repos.empty())
    {
        std::cerr << "Repo list is still empyt!\n";
        exit(1);
    }
}
bool is_exists(const std::string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

bool check_version(std::string new_tag, std::string &repo)
{
    std::string _path = std::string(DATA_FOLDER_PATH).append(repo, 0, repo.find_first_of('/'));
    std::string filepath = std::string(DATA_FOLDER_PATH).append(repo);
    if (!is_exists(_path))
    {
        if (mkdir(_path.c_str(), S_IRWXU) != 0)
        {
            if (errno != EEXIST)
                exit(1);
        }
    }
    std::ifstream infile(filepath);
    if (infile.is_open())
    {
        std::string tag;
        infile >> tag;
        if (tag == new_tag)
            return false;
        else
        {
            infile.close();
            std::ofstream outfile(filepath);
            outfile << new_tag;
            outfile.close();
            return true;
        }
    }
    else
    {
        infile.close();
        std::ofstream outfile(filepath);
        outfile << new_tag;
        outfile.close();
        return true;
    }
}

void reponse_code_check(long &response_code, std::string repo, nlohmann::json j)
{
    if (response_code == 403)
    {
        std::cerr << "Api Limit Exceeded!  To increase limit use github personal token\n";
    }
    else if (response_code == 404)
    {
        std::cerr << "Releases not found in " << repo << "\n";
    }
    else
    {
        if (j["message"] == nullptr)
            std::cerr << "Error while fetching " << repo;
        else
            std::cerr << "Error while fetching " << repo << "\nMessage: " << j["message"] << '\n';
    }
}

int main()
{
    curlHandler curl;
    struct config_t conf;
    while (true)
    {
        get_config(conf);
        check_conf(conf);
        for (auto &repo : conf.repos)
        {
            std::string response_string;
            std::string url = std::string("https://api.github.com/repos/").append(repo).append("/releases/latest");
            curl.get_page(url.c_str(), response_string, conf.personal_token);

            if (response_string == "")
            {
                std::cerr << "Error: Response string is empty while fetching " << repo << "\n";
                continue;
            }

            nlohmann::json j = nlohmann::json::parse(response_string);

            if (curl.response_code != 200)
            {
                reponse_code_check(curl.response_code, repo, j);
                continue;
            }
            std::string new_tag = j["tag_name"].get<std::string>();
            if (check_version(new_tag, repo))
            {
                std::string github = "https://github.com/";
                std::string author_name = j["author"]["login"];
                std::string repo_name = repo.substr(repo.find_first_of('/') + 1);
                std::string repo_link = std::string("https://github.com/").append(repo);
                std::string author_link = std::string("https://github.com/").append(author_name);
                std::string release_name = j["name"];
                std::string changelog = j["body"];
                std::string html_url = j["html_url"];
                std::string msg_text;
                std::stringstream ss;
                ss << "<strong>New <a href='" << repo_link << "'>" << repo_name << "</a> Update is out</strong>\n"
                   << "<strong>Author: </strong><a href='" << author_link << "'>" << author_name << "</a>\n"
                   << "<strong>Release Name: </strong><code>" << release_name << "</code>\n"
                   << "<strong>Release Tag: </strong><code>" << new_tag << "</code>\n"
                   << "<strong>Changelogs: </strong>\n<code>" << changelog << "</code>\n"
                   << html_url << "\n#" << new_tag << " #" << repo_name;
                msg_text=ss.str();
                std::cout << "Sending message\n";
                telegram_message_send(conf, msg_text);
            }
        }
        std::cout << "Up-to-date\n";
        sleep(FETCH_INTERVAL);
    }
    curl_global_cleanup();

    return 0;
}
