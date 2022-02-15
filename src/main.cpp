#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <curl/curl.h>
#include "config.h"
#include "util.h"
#include "telegram.h"

void get_page(const char *url, struct MemoryStruct &chunk /*, CURL *&curl_handle*/)
{
    static CURL *curl_handle;
    if (!curl_handle)
        curl_handle = curl_easy_init();

    CURLcode res;
    if (curl_handle)
    {
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.111 Safari/537.36");
        // curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);
        // curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        res = curl_easy_perform(curl_handle);

        if (res != CURLE_OK)
        {
            std::cerr << "error: " << curl_easy_strerror(res) << "\n";
            exit(1);
        }
        curl_easy_reset(curl_handle);
    }
}
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

int main()
{
    // Curl initialize
    curl_global_init(CURL_GLOBAL_ALL);
    // Memory Struct
    struct MemoryStruct chunk;

    struct config_t conf;
    while (true)
    {
        get_config(conf);
        check_conf(conf);
        for (auto &repo : conf.repos)
        {
            chunk.memory = (char *)malloc(1);
            chunk.size = 0;

            std::string url = std::string("https://api.github.com/repos/").append(repo).append("/releases/latest");
            get_page(url.c_str(), chunk);
            nlohmann::json j;
            std::ofstream out(TMP_FILE_PATH);
            out << chunk.memory;
            out.close();
            std::ifstream input(TMP_FILE_PATH);
            input >> j;
            if (j["message"] != nullptr)
            {
                std::cout << "Error: " << j["message"];
                continue;
            }
            std::string new_tag = j["tag_name"].get<std::string>();
            if (check_version(new_tag, repo))
            {
                std::string reponame = j["assets"][0]["name"];
                std::string repo_link = std::string("https://github.com/").append(repo);
                std::string author_link = std::string("https://github.com/").append(repo, 0, repo.find_first_of('/'));
                std::string uploader_name = j["assets"][0]["uploader"]["login"];
                std::string release_name = j["name"];
                std::string changelog = j["body"];
                std::string download_text = j["assets"][0]["browser_download_url"];
                std::string msg_text = std::string("<strong>New <a href='").append(repo_link).append("'>").append(reponame).append("</a> Update is out</strong>\n<strong>Author: </strong><a href='").append(author_link).append("'>").append(uploader_name).append("</a>\n<strong>Release Name: </strong><code>").append(release_name).append("</code>\n<strong>Release Tag: </strong><code>").append(new_tag).append("</code>\n<strong>Changelogs: </strong>\n<code>").append(changelog).append("</code>\n").append(download_text).append("#").append(new_tag).append(" #").append(reponame);
                //std::cout << msg_text;
                // std::cout << "<strong>New <a href='" << repo_link << "'>"
                //           << reponame << "</a> Update is out</strong>\n<strong>Author: </strong><a href='"
                //           << author_link << "'>"
                //           << uploader_name << "</a>\n<strong>Release Name: </strong><code>"
                //           << release_name << "</code>\n<strong>Release Tag: </strong><code>"
                //           << new_tag << "</code>\n<strong>Changelogs: </strong>\n<code>"
                //           << changelog << "</code>\n"
                //           << download_text << "#"
                //           << new_tag << " #"
                //           << reponame;
                std::cout << "Sending message\n";
                telegram_message_send(conf, msg_text);
            }

            free(chunk.memory);
        }
        std::cout << "Up-to-date\n";
        sleep(300);
    }
    curl_global_cleanup();

    return 0;
}
