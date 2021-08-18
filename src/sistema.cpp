#include "sistema.h"
#include "usuario.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>

using namespace std;

/* COMANDOS */
string Sistema::quit()
{
	return "Saindo...";
}

string Sistema::create_user(const string email, const string senha, const string nome)
{
	for (int i = 0; i < (this->usuarios.size()); i++)
	{
		if (this->usuarios[i].email == email)
			return "E-mail já cadastrado no sistema.";
	}

	int id = this->usuarios.size() + 1;

	Usuario novo_usuario(id, email, nome, senha);

	this->usuarios.push_back(novo_usuario);

	return "Usuario criado com sucesso, ID: " + to_string(novo_usuario.id);
}

string Sistema::login(const string email, const string senha)
{
	for (int i = 0; i < (this->usuarios.size()); i++)
	{
		// Checa se existe usuário com o email
		if (this->usuarios[i].email == email)
		{
			// Checa se a senha está correta
			if (this->usuarios[i].senha == senha)
			{
				// Checa se o usuário já estava no vetor de usuariosLogados
				if (this->usuariosLogados.find(this->usuarios[i].id) != this->usuariosLogados.end())
				{
					return "Usuário já estava logado.";
				}
				auto empty_pair = make_pair("", "");
				this->usuariosLogados[this->usuarios[i].id] = empty_pair;
				return "Login realizado com sucesso";
			}
			else
			{
				return "Senha incorreta";
			}
		}
	}

	return "Usuário não existe";
}

string Sistema::list_usuarios_logados(int id)
{
	if (this->usuariosLogados.find(id) == this->usuariosLogados.end())
	{
		return "Usuário não está logado.";
	}
	// string output = "Usuários logados:";
	stringstream ss;
	for (auto usuarioLogado = this->usuariosLogados.begin(); usuarioLogado != this->usuariosLogados.end(); ++usuarioLogado)
	{
		int usuarioId = usuarioLogado->first;
		ss << "\n  ID: " << usuarioId;
		for (int i = 0; i < (this->usuarios.size()); i++)
		{
			if (this->usuarios[i].id == usuarioId)
			{
				ss << "  |  Nome: " << this->usuarios[i].nome << "  |  Email: " << this->usuarios[i].email;
			}
		}
		ss << "  |  Servidor: " << usuarioLogado->second.first << "  |  Canal: " << usuarioLogado->second.second << "  |";
	}
	return ss.str();
}

string Sistema::disconnect(int id)
{
	if (this->usuariosLogados.find(id) != this->usuariosLogados.end())
	{
		this->usuariosLogados.erase(id);
		return "Usuário deslogado com sucesso.";
	}
	return "Usuário não estava logado.";
}

string Sistema::create_server(int id, const string nome)
{
	if (this->usuariosLogados.find(id) == this->usuariosLogados.end())
	{
		return "Usuário não está logado.";
	}
	for (int i = 0; i < (this->servidores.size()); i++)
	{
		if (this->servidores[i].nome == nome)
		{
			return "Servidor com este nome já existe.";
		}
	}
	Servidor novo_servidor(id, nome);
	this->servidores.push_back(novo_servidor);
	return "Servidor criado com sucesso.";
}

string Sistema::set_server_desc(int id, const string nome, const string descricao)
{
	for (int i = 0; i < (this->servidores.size()); i++)
	{
		if (this->servidores[i].nome == nome)
		{
			if (this->servidores[i].usuario_dono_id == id)
			{
				this->servidores[i].descricao = descricao;
				return "Descrição alterada com sucesso.";
			}
			return "Você não pode alterar a descrição de servidores que não é dono.";
		}
	}
	return "Servidor com esse nome não existe.";
}

string Sistema::set_server_invite_code(int id, const string nome, const string codigo)
{
	for (int i = 0; i < (this->servidores.size()); i++)
	{
		if (this->servidores[i].nome == nome)
		{
			if (this->servidores[i].usuario_dono_id == id)
			{
				this->servidores[i].codigo_convite = codigo;
				if (codigo == "")
				{
					return "Código de convite removido com sucesso.";
				}
				return "Código de convite alterado com sucesso.";
			}
			return "Você não pode alterar o código de convite de servidores que não é dono.";
		}
	}
	return "Servidor com esse nome não existe.";
}

string Sistema::list_servers(int id)
{
	if (this->usuariosLogados.find(id) == this->usuariosLogados.end())
	{
		return "Usuário não está logado.";
	}
	string output = "Servidores:";
	for (int i = 0; i < (this->servidores.size()); i++)
	{
		output += "\n" + this->servidores[i].nome + " | " + to_string(this->servidores[i].usuario_dono_id);
	}
	return output;
}

string Sistema::remove_server(int id, const string nome)
{
	if (this->usuariosLogados.find(id) == this->usuariosLogados.end())
	{
		return "Usuário não está logado.";
	}
	for (int i = 0; i < (this->servidores.size()); i++)
	{
		if (this->servidores[i].nome == nome)
		{
			if (this->servidores[i].usuario_dono_id == id)
			{
				this->servidores.erase(this->servidores.begin() + i);
				auto empty_pair = make_pair("", "");
				// Se algum usuario estiver visualizando o servidor, deixa o servidor e canal vazios
				for (auto usuarioLogado = this->usuariosLogados.begin(); usuarioLogado != this->usuariosLogados.end(); ++usuarioLogado)
				{
					if (usuarioLogado->second.first == nome)
					{
						usuarioLogado->second = empty_pair;
					}
				}
				return "Servidor apagado com sucesso.";
			}
			return "Você não é o dono do servidor.";
		}
	}
	return "Não existe servidor com esse nome.";
}

string Sistema::enter_server(int id, const string nome, const string codigo)
{
	if (this->usuariosLogados.find(id) == this->usuariosLogados.end())
	{
		return "Usuário não está logado.";
	}
	for (int i = 0; i < (this->servidores.size()); i++)
	{
		if (this->servidores[i].nome == nome)
		{
			if (find(this->servidores[i].participantes.begin(), this->servidores[i].participantes.end(), id) != this->servidores[i].participantes.end())
			{
				auto pair = make_pair(nome, "");
				this->usuariosLogados[id] = pair;
				return "Usuário entrou no servidor com sucesso.";
			}
			if (this->servidores[i].codigo_convite == codigo)
			{
				auto pair = make_pair(nome, "");
				this->usuariosLogados[id] = pair;
				this->servidores[i].participantes.push_back(id);
				return "Usuário adicionado ao servidor com sucesso.";
			}
			return "Código de convite inválido.";
		}
	}

	return "Não existe servidor com esse nome.";
}

string Sistema::leave_server(int id, const string nome)
{
	// if (this->usuariosLogados.count(id) > 0)
	if (this->usuariosLogados.find(id) == this->usuariosLogados.end())
	{
		return "Usuário não está logado.";
	}

	for (int i = 0; i < (this->servidores.size()); i++)
	{
		if (this->servidores[i].nome == nome)
		{
			if (find(this->servidores[i].participantes.begin(), this->servidores[i].participantes.end(), id) != this->servidores[i].participantes.end())
			{
				// Seguindo o padrão: vector.erase(left,right)
				// O remove retorna o novo final da lista após as exclusões,
				// e então é necessário apagar as posições à direita do "novo final"
				this->servidores[i].participantes.erase(remove(this->servidores[i].participantes.begin(), this->servidores[i].participantes.end(), id), this->servidores[i].participantes.end());
				if (this->usuariosLogados[id].first == nome)
				{
					auto empty_pair = make_pair("", "");
					this->usuariosLogados[id] = empty_pair;
				}
				return "Usuário removido do servidor.";
			}
			return "Usuário não está no servidor.";
		}
	}
	return "Não existe servidor com esse nome.";
}

string Sistema::list_participants(int id)
{
	if (this->usuariosLogados.find(id) == this->usuariosLogados.end())
	{
		return "Usuário não está logado.";
	}

	string serverName = this->usuariosLogados[id].first;

	if (serverName == "")
	{
		return "Usuário não está visualizando nenhum servidor.";
	}

	for (int i = 0; i < (this->servidores.size()); i++)
	{
		if (this->servidores[i].nome == serverName)
		{
			string output = "Participantes:";
			for (int j = 0; j < (this->servidores[i].participantes.size()); j++)
			{
				int usuarioId = this->servidores[i].participantes[j];
				for (int k = 0; k < (this->usuarios.size()); k++)
				{
					if (this->usuarios[k].id == usuarioId)
					{
						output += "\n" + this->usuarios[k].email;
					}
				}
			}
			return output;
		}
	}

	return "Servidor não encontrado";
}

// PARTE 2
string Sistema::list_channels(int id)
{
	return "list_channels NÃO IMPLEMENTADO";
}

string Sistema::create_channel(int id, const string nome)
{
	return "create_channel NÃO IMPLEMENTADO";
}

string Sistema::enter_channel(int id, const string nome)
{
	return "enter_channel NÃO IMPLEMENTADO";
}

string Sistema::leave_channel(int id)
{
	return "leave_channel NÃO IMPLEMENTADO";
}

string Sistema::send_message(int id, const string mensagem)
{
	return "send_message NÃO IMPLEMENTADO";
}

string Sistema::list_messages(int id)
{
	return "list_messages NÃO IMPLEMENTADO";
}

/* IMPLEMENTAR MÉTODOS PARA OS COMANDOS RESTANTES */
