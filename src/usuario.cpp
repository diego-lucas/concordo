#include "usuario.h"

Usuario::Usuario(int _id, std::string _email, std::string _nome, std::string _senha)
{
    this->id = _id;
    this->email = _email;
    this->nome = _nome;
    this->senha = _senha;
}