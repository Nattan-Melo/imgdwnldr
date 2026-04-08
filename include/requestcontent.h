#ifndef REQUEST_CONTENT_H
#define REQUEST_CONTENT_H

#include <curl/curl.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class RequestContent
{
public:
    RequestContent();
    ~RequestContent();
    std::vector<unsigned char> get_content(const std::string &url_target);
    bool save_content(const std::string& fl_path, const std::vector<unsigned char> &raw_content);
    static std::string get_image_format(std::vector<unsigned char>& rwdt);
private:
    CURL *curl;
    CURLcode result;

    static size_t write_callback(void* content, size_t size, size_t nmemb, void* userp)
    {
        size_t real_size = nmemb * size;
        std::vector<unsigned char> *mem = static_cast<std::vector<unsigned char>*>(userp);

        unsigned char* ptr = static_cast<unsigned char*>(content);
        mem->insert(mem->end(), ptr, ptr + real_size);
        return real_size;

    }

};

#endif