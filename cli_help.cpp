#include "cli_help.h"
void display_help(){

cout << "imgdwnldr - Utilitário para download em massa de imagens via CSV e JSON" << endl;

cout << "SINOPSE" << endl << "\t./imgdwnldr [COMANDO] [ARQUIVO] [OPÇÕES]" << endl;

cout <<
"DESCRIÇÃO" << endl <<
    "\tO imgdwnldr lê um arquivo CSV ou JSON contendo códigos e URLs, realiza o download" << endl
    << "\tdas imagens e (se .csv) atualiza o status de processamento no arquivo original." << endl;

cout <<
"COMANDOS DISPONÍVEIS" << endl <<
    "\tcsv      Processa um arquivo CSV para baixar as imagens." << endl << 
    "\tjson     Processa um arquivo JSON para baixar as imagens." << endl;
cout <<
"OPÇÕES"<< endl
<< 
    "\thelp, --help" << endl
    <<"\t\tExibe esta tela de ajuda."<<endl;

cout <<
    "\t-o, --output <diretório>"<<endl<<
    "\t\tDefine a pasta onde as imagens serão salvas."<<endl<< 
        "\t\t\t(Padrão: diretório de execução)"<<endl;
cout <<
"EXEMPLOS" << endl <<
    "\tProcessar um CSV salvando em uma pasta específica:"<<endl<<
        "\t\t./imgdwnldr csv lista.csv --output ./fotos_produtos"<<endl<<endl;
cout <<
    "\tProcessar um JSON salvando em uma pasta específica:" << endl <<
    "\t\t./imgdwnldr json lista.json" <<endl; 
cout <<
"AUTOR"<<endl<<
    "\tDesenvolvido por Jefferson Nattan - 2026"<<endl;
}
