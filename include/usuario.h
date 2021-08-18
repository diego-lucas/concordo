#include <iostream>
#include <string>
#ifndef USUARIO_H
#define USUARIO_H

class Usuario
{
public:
	int id;
	std::string email;
	std::string nome;
	std::string senha;
	Usuario(int _id, std::string _email, std::string _nome, std::string _senha);

	// private:
};

#endif
