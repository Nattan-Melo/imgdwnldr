#include <iostream>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidcsv.h"
#include "requestcontent.h"
#include <clocale>
#include <curl/curl.h>
#include <unordered_map>
#include "curlenv.h"
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <random>
#include <csignal>
#include <atomic>

using namespace std;
static std::random_device rd;
static std::mt19937 generator(rd());
volatile std::atomic<bool> handle_signal(true);

void callback_capture_interrupt(int signal)
{
    if(signal == SIGINT)
        handle_signal = false;
}

struct Datasource
{  
    string id;
    string url;
    string status;
};

int delay_it()
{   
    std::uniform_int_distribution<int> delay(2,7);
    std::uniform_int_distribution<int> overtime(1,100);

    if(overtime(generator) <= 20)
        return 7;
    else
        return delay(generator);
}

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
    std::unordered_map<string, std::vector<string>> *args_tree = init_args_tree();
    RequestContent *req = new RequestContent();

    std::signal(SIGINT, callback_capture_interrupt);
    std::setlocale(LC_ALL, ".UTF8");
    
    if(strcmp(argv[1], "json")==0)
    {
        std::unordered_map<string, string> *ds = new std::unordered_map<string, string>();
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
            for(const auto& element : *ds)
            {
                auto raw_data = req->get_content(element.second);
                auto format = req->get_image_format(raw_data);
                req->save_content(element.first + format ,raw_data);
                std::this_thread::sleep_for(std::chrono::seconds(delay_it()));
            }

            delete req;
            delete ds;
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
    else if(strcmp(argv[1], "test") == 0)
    {
        std::string mandatory_cols[] = {"CD","URL","STS"};
        size_t sz_mandatory_cols = std::size(mandatory_cols);
        try
        {
            rapidcsv::Document doc(argv[2]);

            if(sz_mandatory_cols != doc.GetColumnCount())
            {
                std::cout << "Erro: o arquivo deve conter três colunas - CD, URL e STS - Não foi possível concluir o processamento" << endl;
                return (1);
            }
            
            auto doc_cols = doc.GetColumnNames();
            if(std::equal(doc_cols.begin(), doc_cols.end(), mandatory_cols) == true)
            {
                long http_status = 0;
                for(size_t i = 0; i < doc.GetRowCount(); ++i)
                {
                    vector<string> line = doc.GetRow<string>(i);
                    
                    if(line[3] == "0")
                    {

                        auto raw_data = req->get_content(line[1], &http_status);
                        if(http_status != 200)
                        {
                            line[2] = "-1";
                            doc.SetRow(i, line); 
                            continue;
                        }
                        else
                        {
                            line[2] = "1";
                            doc.SetRow(i, line); 
                            auto img_format = req->get_image_format(raw_data);
                            req->save_content(line[0] + img_format, raw_data);

                        }
                    }
                    else
                        continue;
                    
                    std::this_thread::sleep_for(std::chrono::seconds(delay_it()));
                }
                doc.Save();
            }
            else
            {
                std::cout << "Error: o arquivo .csv deve conter as colunas - CD, URL e STS - Verifique o arquivo e tente novamente" << endl;
                return (1);
            }

        }
        catch(const std::exception& e){
            cerr << "Erro ao ler o arquivo .csv " << e.what() << endl;
            return (1);
        }
        return (0);
    }
    else
    {
        cerr << "Argumentos inválidos - acesse o comando -h ou help" << endl;
        return 1;
    }
    


    delete args_tree;
    return (0);
}