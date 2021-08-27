#ifndef CANALTEXTO_H
#define CANALTEXTO_H
#include "mensagem.h"

#include <vector>
#include <string>

class CanalTexto
{
public:
	std::string nome;
	std::vector<Mensagem> mensagens;
	CanalTexto(std::string _nome);
};

#endif
