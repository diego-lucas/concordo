#include "servidor.h"

using namespace std;

Servidor::Servidor(int _usuario_dono_id, std::string _nome)
{
    this->usuario_dono_id = _usuario_dono_id;
    this->nome = _nome;
    this->codigo_convite = "";
}