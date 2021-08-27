#include "mensagem.h"
#include <ctime>

using namespace std;

Mensagem::Mensagem(int _enviada_por, std::string _conteudo)
{
    this->enviada_por = _enviada_por;
    this->conteudo = _conteudo;

    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", timeinfo);
    string str(buffer);

    this->data_hora = str;
}