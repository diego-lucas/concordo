#ifndef MENSAGEM_H
#define MENSAGEM_H

#include <string>

class Mensagem
{
public:
    std::string data_hora;
    int enviada_por;
    std::string conteudo;
    Mensagem(int _enviada_por, std::string _conteudo);
};

#endif
