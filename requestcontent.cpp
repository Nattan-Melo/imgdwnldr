#include "requestcontent.h"


RequestContent::RequestContent()
{
    this->curl = curl_easy_init();
    if(this->curl == NULL)
        throw std::runtime_error(
            "Erro: não foi possível inicializar o mecanismo de requisições"
        );
    
}


RequestContent::~RequestContent()
{
    curl_easy_cleanup(this->curl);
}

std::vector<unsigned char> RequestContent::get_content(const std::string &url_target)
{
    struct curl_slist *headers = NULL;
    std::vector<unsigned char> imageData;
    if(this->curl)
    {   
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");
        curl_easy_setopt(this->curl, CURLOPT_URL, url_target.c_str());
        curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, this->write_callback);
        curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &imageData);
        curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);

        this->result = curl_easy_perform(this->curl);

        if(this->result != CURLE_OK)
        {
            throw std::runtime_error(
                curl_easy_strerror(this->result)
            );
        }

        curl_slist_free_all(headers);
    }

    return imageData;
}
std::vector<unsigned char> RequestContent::get_content(const std::string &url_target, long &http_status)
{
    struct curl_slist *headers = NULL;
    std::vector<unsigned char> imageData;
    if(this->curl)
    {   
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");
        curl_easy_setopt(this->curl, CURLOPT_URL, url_target.c_str());
        curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, this->write_callback);
        curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &imageData);
        curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(this->curl, CURLOPT_TIMEOUT_MS, 5000);

        this->result = curl_easy_perform(this->curl);
        long status = 0;
        if(this->result != CURLE_OK)
        {
            throw std::runtime_error(
                curl_easy_strerror(this->result)
            );
        }
        else
        {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_status);
        }

        curl_slist_free_all(headers);
    }

    return imageData;
}
bool RequestContent::save_content(const std::string& fl_path, const std::vector<unsigned char> &raw_content)
{
    std::ofstream file_content(fl_path, std::ios::binary);

    if(!file_content)
        throw std::runtime_error("Erro: não foi possível manipular o fluxo de arquivo motivos possíveis: diretório sem permissão, bloqueio de acesso, disco com defeito");

    file_content.write(reinterpret_cast<const char*>(raw_content.data()), raw_content.size());
    file_content.close();
    return true;
    
}


std::string RequestContent::get_image_format(std::vector<unsigned char>& rwdt)
{
    if(rwdt.size() < 12)
        return ".bin";

    if(rwdt[0] == 0x89 && rwdt[1] == 0x50 && rwdt[2] == 0x4E && rwdt[3] == 0x47)
    {
        return ".png";
    }
    if(rwdt[0] == 0xFF && rwdt[1] == 0xD8 && rwdt[2] == 0xFF)
    {
        return ".jpg";
    }
    if(rwdt[0] == 0x52 && rwdt[1] == 0x49 && rwdt[2] == 0x46 && rwdt[3] == 0x46
        && rwdt[0] == 0x57 && rwdt[1] == 0x45 && rwdt[2] == 0x42 && rwdt[3] == 0x50)
    {
        return ".wep";        
    }

    return ".bin";
}