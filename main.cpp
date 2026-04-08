#include <iostream>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"
#include "requestcontent.h"
#include <clocale>
#include <curl/curl.h>
#include <unordered_map>
#include "curlenv.h"
#include <vector>
using namespace std;

std::unordered_map<string, vector<string>> *init_args_tree()
{
    auto *tmp = new std::unordered_map<string, vector<string>>();    
    tmp->insert(make_pair("json", vector<string>{"-l", "--log"}));
    tmp->insert(make_pair("csv", vector<string>{"-o", "--output", "-l", "--log"}));
    tmp->insert(make_pair("help", vector<string>{"-h", "--help"}));
    return tmp;
}

int contains_args(const char* contains, const char* args[], int len)
{
    for(int i=0; i < len; ++i)
    {
        if(std::strcmp(args[i], contains) == 0)
            return i;
    }

    throw std::runtime_error("comando não encontrado");   
}

int main(int argc, char* argv[])
{   
    CurlEnvironment curl_env;
    std::unordered_map<string, string> *ds = new std::unordered_map<string, string>();
    std::unordered_map<string, std::vector<string>> *args_tree = init_args_tree();
    RequestContent *req = new RequestContent();

    std::setlocale(LC_ALL, ".UTF8");
    if(strcmp(argv[1], "json")==0)
    {
        try
        {
            // Tratar log
            rapidjson::Document doc;
            std::ifstream file_stream(argv[2]);
            
            if(!file_stream.is_open()){
                cerr << "Não foi possível carregar o arquivo json informando" << endl;
                return 1;
            }

            rapidjson::IStreamWrapper wrap_stream(file_stream);
            doc.ParseStream(wrap_stream);

            if(doc.HasParseError()){
                cerr << "Erro ao ler o arquivo de dados " << doc.GetParseError() << endl;
                return 1;
            }

            if(doc.IsObject()){
                for(rapidjson::Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
                {
                    ds->insert({itr->name.GetString(), itr->value.GetString()});
                }
            }
            else
            {
                cerr << "-> O arquivo json informado deve estar formatado como objeto seguindo o padrão 'CÓDIGO':'URL' " << endl;
                return 1;
            }
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << "Erro: " << e.what() << endl;   
        }

    }
    else if(strcmp(argv[1],"csv")==0)
    {
        cout << "Fonte de dados CSV não suportado" << endl;
        return 1;   
    }
    else
    {
        cerr << "Argumentos inválidos - acesse o comando -h ou help" << endl;
        return 1;
    }

    for(const auto& element : *ds)
    {
        auto raw_data = req->get_content(element.second);
        auto format = req->get_image_format(raw_data);
        req->save_content(element.first + format ,raw_data);
    }
    return (0);
}