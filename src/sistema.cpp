#include "sistema.h"

using namespace std;

/* COMANDOS */

// Sistema::Sistema()
// {
// 	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
// 	pqxx::connection c(connection_string.c_str());
// 	this->conn = c;
// }

string Sistema::quit()
{
	return "Saindo...";
}

string Sistema::create_user(const string email, const string senha, const string nome)
{
	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	int usuarios_com_email = cursor.exec("SELECT COUNT(*) FROM projeto.usuarios WHERE email = " + cursor.quote(email))[0][0].as<int>();
	if (usuarios_com_email != 0)
		return "E-mail já cadastrado no sistema.";

	cursor.exec("INSERT INTO projeto.usuarios (email, nome, senha) VALUES (" + cursor.quote(email) + ", " + cursor.quote(nome) + ", " + cursor.quote(senha) + ");");
	cursor.commit();

	return "Usuario criado com sucesso";
}

string Sistema::login(const string email, const string senha)
{
	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_com_email = cursor.exec("SELECT email, senha, logado FROM projeto.usuarios WHERE email = " + cursor.quote(email));

	if (usuario_com_email.size() == 0)
		return "Usuário não existe";

	if (usuario_com_email[0][2].as<bool>() == 1)
		return "Usuário já está logado no sistema.";

	if (usuario_com_email[0][1].as<string>() == senha)
	{
		cursor.exec("UPDATE projeto.usuarios SET logado = true WHERE email = " + cursor.quote(email));
		cursor.commit();
		return "Login realizado com sucesso!";
	}

	return "Senha incorreta.";
}

string Sistema::disconnect(int id)
{
	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_com_id = cursor.exec("SELECT email, logado FROM projeto.usuarios WHERE id = " + cursor.quote(id));

	if (usuario_com_id.size() == 0)
		return "Usuário não existe.";

	if (usuario_com_id[0][1].as<bool>() == 1)
	{
		cursor.exec("UPDATE projeto.usuarios SET logado = false, servidor_visualizado = NULL, canal_visualizado = NULL WHERE id = " + cursor.quote(id));
		cursor.commit();
		return "Usuário desconectado com sucesso.";
	}

	return "Usuário não estava logado.";
}

string Sistema::create_server(int id, const string nome)
{
	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_logado = cursor.exec("SELECT email, logado FROM projeto.usuarios WHERE logado = true AND id = " + cursor.quote(id));

	if (usuario_logado.size() == 0)
		return "Usuário não está logado.";

	pqxx::result servidores_com_nome = cursor.exec("SELECT nome FROM projeto.servidores WHERE nome = " + cursor.quote(nome));

	if (servidores_com_nome.size() != 0)
		return "Servidor com este nome já existe.";

	cursor.exec("INSERT INTO projeto.servidores (usuario_dono_id, nome) VALUES (" + cursor.quote(id) + ", " + cursor.quote(nome) + ");");
	cursor.commit();

	return "Servidor criado com sucesso.";
}

string Sistema::set_server_desc(int id, const string nome, const string descricao)
{
	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_logado = cursor.exec("SELECT email, logado FROM projeto.usuarios WHERE logado = true AND id = " + cursor.quote(id));

	if (usuario_logado.size() == 0)
		return "Usuário não está logado.";

	pqxx::result servidor = cursor.exec("SELECT nome, usuario_dono_id FROM projeto.servidores WHERE nome = " + cursor.quote(nome));
	if (servidor.size() == 0)
		return "Servidor com esse nome não existe.";

	if (servidor[0][1].as<int>() != id)
		return "Você não pode alterar a descrição de servidores que não é dono.";

	cursor.exec("UPDATE projeto.servidores SET descricao = " + cursor.quote(descricao) + " WHERE nome = " + cursor.quote(nome));
	cursor.commit();
	return "Descrição alterada com sucesso.";
}

string Sistema::set_server_invite_code(int id, const string nome, const string codigo)
{
	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_logado = cursor.exec("SELECT email, logado FROM projeto.usuarios WHERE logado = true AND id = " + cursor.quote(id));

	if (usuario_logado.size() == 0)
		return "Usuário não está logado.";

	pqxx::result servidor = cursor.exec("SELECT nome, usuario_dono_id, codigo_convite FROM projeto.servidores WHERE nome = " + cursor.quote(nome));
	if (servidor.size() == 0)
		return "Servidor com esse nome não existe.";

	if (servidor[0][1].as<int>() != id)
		return "Você não pode alterar o código de convite de servidores que não é dono.";

	cursor.exec("UPDATE projeto.servidores SET codigo_convite = " + cursor.quote(codigo) + " WHERE nome = " + cursor.quote(nome));
	cursor.commit();

	if (codigo == "")
		return "Código de convite removido com sucesso.";
	else
		return "Código de convite alterado com sucesso.";
}

string Sistema::list_servers(int id)
{
	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_logado = cursor.exec("SELECT email, logado FROM projeto.usuarios WHERE logado = true AND id = " + cursor.quote(id));

	if (usuario_logado.size() == 0)
		return "Usuário não está logado.";

	pqxx::result servidores = cursor.exec("SELECT nome, usuario_dono_id FROM projeto.servidores");
	string output = "Servidores:\nNome do servidor | ID do dono";
	for (int i = 0; i < servidores.size(); i++)
	{
		output += "\n" + servidores[i][0].as<string>() + " | " + servidores[i][1].as<string>();
	}
	return output;
}

string Sistema::remove_server(int id, const string nome)
{
	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_logado = cursor.exec("SELECT email, logado FROM projeto.usuarios WHERE logado = true AND id = " + cursor.quote(id));

	if (usuario_logado.size() == 0)
		return "Usuário não está logado.";

	pqxx::result servidor = cursor.exec("SELECT nome, usuario_dono_id FROM projeto.servidores WHERE nome = " + cursor.quote(nome));
	if (servidor.size() == 0)
		return "Servidor com esse nome não existe.";

	if (servidor[0][1].as<int>() != id)
		return "Você não é o dono do servidor.";

	cursor.exec("DELETE FROM projeto.servidores WHERE nome = " + cursor.quote(nome));
	cursor.exec("UPDATE projeto.usuarios SET servidor_visualizado = NULL, canal_visualizado = NULL WHERE servidor_visualizado = " + cursor.quote(nome));
	cursor.commit();
	return "Servidor apagado com sucesso.";
}

string Sistema::enter_server(int id, const string nome, const string codigo)
{
	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_logado = cursor.exec("SELECT email, logado FROM projeto.usuarios WHERE logado = true AND id = " + cursor.quote(id));

	if (usuario_logado.size() == 0)
		return "Usuário não está logado.";

	pqxx::result usuario_participante = cursor.exec("SELECT id_usuario FROM projeto.servidores_usuarios WHERE servidor = " + cursor.quote(nome) + " AND id_usuario = " + cursor.quote(id));
	pqxx::result servidor = cursor.exec("SELECT nome, coalesce(codigo_convite, '') AS codigo_convite FROM projeto.servidores WHERE nome = " + cursor.quote(nome));
	if (servidor.size() == 0)
		return "Servidor com esse nome não existe.";

	if (usuario_participante.size() == 0)
	{
		if (servidor[0][1].as<string>() != codigo)
		{
			return "Código de convite inválido.";
		}
		cursor.exec("INSERT INTO projeto.servidores_usuarios (id_usuario, servidor) VALUES (" + cursor.quote(id) + ", " + cursor.quote(nome) + ");");
	}
	cursor.exec("UPDATE projeto.usuarios SET servidor_visualizado = " + cursor.quote(nome) + " WHERE id = " + cursor.quote(id));
	cursor.commit();
	return "Usuário entrou no servidor com sucesso.";
}

string Sistema::leave_server(int id, const string nome)
{
	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_logado = cursor.exec("SELECT email, logado, servidor_visualizado FROM projeto.usuarios WHERE logado = true AND id = " + cursor.quote(id));

	if (usuario_logado.size() == 0)
		return "Usuário não está logado.";

	if (usuario_logado[0][2].as<string>() != nome)
		return "Usuário não está no servidor.";

	cursor.exec("UPDATE projeto.usuarios SET servidor_visualizado = NULL WHERE id = " + cursor.quote(id) + " AND servidor_visualizado = " + cursor.quote(nome));
	cursor.exec("DELETE FROM projeto.servidores_usuarios WHERE id_usuario = " + cursor.quote(id) + " AND servidor = " + cursor.quote(nome));
	cursor.commit();
	return "Usuário removido do servidor.";
}

string Sistema::list_participants(int id)
{
	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_logado = cursor.exec("SELECT email, logado, coalesce(servidor_visualizado, '') AS servidor_visualizado FROM projeto.usuarios WHERE logado = true AND id = " + cursor.quote(id));

	if (usuario_logado.size() == 0)
		return "Usuário não está logado.";

	string servidor_visualizado = usuario_logado[0][2].as<string>();

	if (servidor_visualizado == "")
		return "Usuário não está visualizando nenhum servidor.";

	string output = "Participantes do servidor " + servidor_visualizado + ":";
	pqxx::result participantes = cursor.exec("SELECT usuarios.email FROM projeto.servidores_usuarios JOIN projeto.usuarios ON(servidores_usuarios.id_usuario = usuarios.id) WHERE servidores_usuarios.servidor = " + cursor.quote(servidor_visualizado));
	for (int i = 0; i < participantes.size(); i++)
	{
		output += "\n" + participantes[i][0].as<string>();
	}
	return output;
}

// PARTE 2
string Sistema::list_channels(int id)
{

	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_logado = cursor.exec("SELECT email, logado, coalesce(servidor_visualizado, '') AS servidor_visualizado FROM projeto.usuarios WHERE logado = true AND id = " + cursor.quote(id));

	if (usuario_logado.size() == 0)
		return "Usuário não está logado.";

	string servidor_visualizado = usuario_logado[0][2].as<string>();

	if (servidor_visualizado == "")
		return "Usuário não está visualizando nenhum servidor.";

	pqxx::result canais = cursor.exec("SELECT nome FROM projeto.canais WHERE servidor = " + cursor.quote(servidor_visualizado));
	if (canais.size() == 0)
		return "O servidor visualizado não possui nenhum canal de texto.";

	string output = "Canais de texto do servidor " + servidor_visualizado + ":";
	for (int i = 0; i < canais.size(); i++)
	{
		output += "\n" + canais[i][0].as<string>();
	}
	return output;
}

string Sistema::create_channel(int id, const string nome)
{

	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_logado = cursor.exec("SELECT email, logado, coalesce(servidor_visualizado, '') AS servidor_visualizado FROM projeto.usuarios WHERE logado = true AND id = " + cursor.quote(id));

	if (usuario_logado.size() == 0)
		return "Usuário não está logado.";

	string servidor_visualizado = usuario_logado[0][2].as<string>();

	if (servidor_visualizado == "")
		return "Usuário não está visualizando nenhum servidor.";

	pqxx::result canais = cursor.exec("SELECT nome FROM projeto.canais WHERE servidor = " + cursor.quote(servidor_visualizado) + " AND nome = " + cursor.quote(nome));
	if (canais.size() != 0)
		return "Já existe um canal de texto com esse nome.";

	cursor.exec("INSERT INTO projeto.canais (nome, servidor) VALUES (" + cursor.quote(nome) + ", " + cursor.quote(servidor_visualizado) + ");");
	cursor.commit();
	return "Canal de texto criado com sucesso.";
}

string Sistema::enter_channel(int id, const string nome)
{

	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_logado = cursor.exec("SELECT email, logado, coalesce(servidor_visualizado, '') AS servidor_visualizado FROM projeto.usuarios WHERE logado = true AND id = " + cursor.quote(id));

	if (usuario_logado.size() == 0)
		return "Usuário não está logado.";

	string servidor_visualizado = usuario_logado[0][2].as<string>();

	if (servidor_visualizado == "")
		return "Usuário não está visualizando nenhum servidor.";

	pqxx::result canais = cursor.exec("SELECT nome FROM projeto.canais WHERE nome = " + cursor.quote(nome) + " AND servidor = " + cursor.quote(servidor_visualizado));
	if (canais.size() == 0)
		return "Não existe canal com esse nome.";

	cursor.exec("UPDATE projeto.usuarios SET canal_visualizado = " + cursor.quote(nome) + " WHERE id = " + cursor.quote(id));
	cursor.commit();
	return "Usuário entrou no canal com sucesso.";
}

string Sistema::leave_channel(int id)
{
	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_logado = cursor.exec("SELECT email, logado, coalesce(canal_visualizado, '') AS canal_visualizado FROM projeto.usuarios WHERE logado = true AND id = " + cursor.quote(id));

	if (usuario_logado.size() == 0)
		return "Usuário não está logado.";

	string canal_visualizado = usuario_logado[0][2].as<string>();

	if (canal_visualizado == "")
		return "Usuário não está visualizando nenhum canal.";

	cursor.exec("UPDATE projeto.usuarios SET canal_visualizado = NULL WHERE id = " + cursor.quote(id));
	cursor.commit();
	return "Usuário saiu do canal.";
}

string Sistema::send_message(int id, const string mensagem)
{

	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_logado = cursor.exec("SELECT email, logado, coalesce(canal_visualizado, '') AS canal_visualizado FROM projeto.usuarios WHERE logado = true AND id = " + cursor.quote(id));

	if (usuario_logado.size() == 0)
		return "Usuário não está logado.";

	string canal_visualizado = usuario_logado[0][2].as<string>();
	if (canal_visualizado == "")
		return "Usuário não está visualizando nenhum canal.";

	cursor.exec("INSERT INTO projeto.mensagens (enviada_por, conteudo, canal) VALUES (" + cursor.quote(id) + ", " + cursor.quote(mensagem) + ", " + cursor.quote(canal_visualizado) + ");");
	cursor.commit();
	return "Usuário enviou a mensagem com sucesso.";
}

string Sistema::list_messages(int id)
{
	string connection_string = "host=kesavan.db.elephantsql.com port=5432 dbname=znlcibmv user=znlcibmv password=YznrvZygGWNuSAKfwesE7MuWDCP3Hgoa";
	pqxx::connection c(connection_string.c_str());
	pqxx::work cursor(c);

	pqxx::result usuario_logado = cursor.exec("SELECT email, logado, coalesce(canal_visualizado, '') AS canal_visualizado FROM projeto.usuarios WHERE logado = true AND id = " + cursor.quote(id));

	if (usuario_logado.size() == 0)
		return "Usuário não está logado.";

	string canal_visualizado = usuario_logado[0][2].as<string>();
	if (canal_visualizado == "")
		return "Usuário não está visualizando nenhum canal.";

	pqxx::result mensagens = cursor.exec("SELECT usuarios.nome AS enviada_por, mensagens.conteudo, mensagens.data_hora::timestamp(0) FROM projeto.mensagens JOIN projeto.usuarios ON (usuarios.id = mensagens.enviada_por) WHERE canal = " + cursor.quote(canal_visualizado));

	if (mensagens.size() == 0)
		return "O canal visualizado não possui nenhuma mensagem.";

	string output = "Mensagens do canal " + canal_visualizado + ":";
	for (int i = 0; i < mensagens.size(); i++)
	{
		output += "\n " + mensagens[i][0].as<string>() + " <" + mensagens[i][2].as<string>() + ">: " + mensagens[i][1].as<string>();
	}
	return output;
}
