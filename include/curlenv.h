#ifndef CURL_ENV_H
#define CURL_ENV_H
#include <curl/curl.h>
#include <stdexcept>
class CurlEnvironment
{
public:
    CurlEnvironment()
    {
        if(curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK)
            throw std::runtime_error("Erro fatal: Falha ao inicializar o ambiente global do CURL"); 
    }
    ~CurlEnvironment()
    {
        curl_global_cleanup();
    }
    CurlEnvironment(const CurlEnvironment&) = delete;
    CurlEnvironment& operator=(const CurlEnvironment&) = delete;
};
#endif  