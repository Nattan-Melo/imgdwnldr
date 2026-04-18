# 📸 IMGDWNLDR
Utilitário para download em massa de imagens via CSV e JSON

**imgdwnldr** é uma ferramenta de linha de comando (CLI) desenvolvida em C++ para o download massivo de imagens a partir de arquivos CSV ou JSON. Ideal para criação de banco de imagens

![Status do Projeto](https://img.shields.io/badge/status-em%20desenvolvimento-green)
![Licença](https://img.shields.io/badge/license-MIT-blue)

## 🚀 Funcionalidades

- **Download em Massa:** Processa listas de URLs e salva localmente com nomes personalizados.
- **Detecção Inteligente:** Identifica automaticamente o formato da imagem (PNG, JPG, WEBP, etc.) através dos *magic bytes*.
- **Controle de Fluxo:** Sistema de delay inteligente para evitar bloqueios por parte dos servidores.
- **Persistência:** Atualiza o status de cada download diretamente no arquivo CSV (Coluna STS).

## 🛠️ Tecnologias Utilizadas

- **Linguagem:** C++17
- **Requisições HTTP:** [libcurl](https://curl.se/libcurl/)
- **Manipulação de CSV:** [RapidCSV](https://github.com/d969/rapidcsv)
- **Manipulação de JSON:** [RapidJSON](https://github.com/Tencent/rapidjson)

## 📦 Instalação e Compilação

### Pré-requisitos
Certifique-se de ter o compilador `g++` (com suporte a C++17) e a biblioteca `libcurl` instalada. No MSYS2/MinGW:
```bash
pacman -S mingw-w64-x86_64-curl
````
### Compilando

```bash
g++ main.cpp requestcontent.cpp cli_help.cpp -o imgdwnldr -lcurl -std=c++17
```

## 📖 Como Usar

Para ver todos os comandos disponíveis, use:

```bash
./imgdwnldr help
```

### Exemplos práticos

**Baixar imagens de um CSV:**
O arquivo deve conter as colunas `CD` (Código), `URL` (Link) e `STS` (Status).

```bash
./imgdwnldr csv datasource.csv --output ./minha_pasta_de_fotos
```
**Baixar imagens de um JSON:**
O arquivo deve conter as colunas `CD` (Código), `URL`.

```bash
./imgdwnldr csv datasource.json --o ./minha_pasta_de_fotos
```

## 📄 Licença

Este projeto está sob a licença [MIT](https://www.google.com/search?q=LICENSE).

## ✍️ Autor

Desenvolvido por **Nattan Melo**
