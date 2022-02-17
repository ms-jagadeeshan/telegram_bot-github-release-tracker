#include "util.h"

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string *data)
{
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}

void curlHandler::get_page(const char *url, std::string &response_string, const std::string personal_token)
{
    if (!this->curl_handle)
        this->curl_handle = curl_easy_init();

    CURLcode res;
    if (this->curl_handle)
    {
        curl_easy_setopt(this->curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(this->curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

        /* ask libcurl to show us the verbose output */
        // curl_easy_setopt(this->curl_handle, CURLOPT_VERBOSE, 1L);

        curl_easy_setopt(this->curl_handle, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(this->curl_handle, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(this->curl_handle, CURLOPT_HEADERDATA, &(this->response_header));
        curl_easy_setopt(this->curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.111 Safari/537.36");
        if (personal_token != "")
        {
            curl_easy_setopt(this->curl_handle, CURLOPT_USERNAME, personal_token.substr(0, personal_token.find_first_of(":")).c_str());
            curl_easy_setopt(this->curl_handle, CURLOPT_PASSWORD, personal_token.substr(personal_token.find_first_of(":") + 1).c_str());
        }

        res = curl_easy_perform(this->curl_handle);
        curl_easy_getinfo(this->curl_handle, CURLINFO_RESPONSE_CODE, &(this->response_code));
        curl_easy_getinfo(this->curl_handle, CURLINFO_TOTAL_TIME, &(this->elapsed));

        if (res != CURLE_OK)
        {
            std::cerr << "error: " << curl_easy_strerror(res) << "\n";
        }
        curl_easy_reset(this->curl_handle);
    }
}

// size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
// {
//     size_t realsize = size * nmemb;
//     struct MemoryStruct *mem = (struct MemoryStruct *)userp;

//     char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
//     if (ptr == NULL)
//     {
//         printf("error: not enough memory\n");
//         return 0;
//     }

//     mem->memory = ptr;
//     memcpy(&(mem->memory[mem->size]), contents, realsize);
//     mem->size += realsize;
//     mem->memory[mem->size] = 0;

//     return realsize;
// }
