#ifndef SERVIDOR_H
#define SERVIDOR_H
#include <vector>
#include <string>

#include "usuario.h"
#include "canaltexto.h"

class Servidor
{
public:
	int usuario_dono_id;
	std::string nome;
	std::string descricao;
	std::string codigo_convite;
	std::vector<CanalTexto> canais_texto;
	std::vector<int> participantes;
	Servidor(int _usuario_dono_id, std::string _nome);
};

#endif
