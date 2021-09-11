# Projeto Concordo

Projeto "Concordo" da disciplina de Linguagem de Pogramação I.
A atividade consiste de criar um sistema com funções semelhantes ao Discord, mas que vai funcionar somente em modo texto e sem recursos de rede.
A ideia principal é simular o “backend” de um serviço como o Discord.

## Para compilar

Antes de compilar, é necessário instalar a biblioteca libpqxx, com o comando:

```console
sudo apt install libpqxx-6.4 libpqxx-dev
```

Para saber mais sobre a biblioteca, acesse: http://pqxx.org/development/libpqxx/
E então, para compilar use o comando abaixo:

```console
mkdir build
make
```

## Para executar

Você pode rodar o sistema e em logo depois digitar os comandos seguidos de ENTER

```console
./build/concordo
```

Ou pode criar um arquivo de texto com uma lista de comandos (um por linha) e executar o sistema redirecionando esse arquivo como entrada padrão:

```console
./build/concordo < script_comandos.txt
```

## Comandos disponíveis

`quit`
Encerra o funcionamento do Concordo, o programa termina ao executar este comando.

`create-user <email> <senha> <nome>`
Cria um usuário e retorna uma string de erro/sucesso

`login <email> <senha>`
Realiza o login do usuário com email e senha.

`disconnect <id>`
Desconecta um usuário específico do sistema. A função retorna uma mensagem de sucesso ou de erro caso o usuário não esteja logado.

`create-server <id> <nome>`
Cria um novo servidor no sistema e o adiciona na lista de servidores.

`set-server-desc <id> <nome> <descricao>`
Modifica a descrição do servidor passado no comando, para a descrição desejada.

`set-server-invite-code <id> <nome> <codigo>`
Modifica o código de convite de um servidor no sistema.

`list-servers <id>`
Lista os servidores cadastrados no sistema.

`remove-server <id> <nome>`
Apaga um servidor da lista de servidores.

`enter-server <id> <nome> <codigo>`
Faz com que o usuário logado com o id dado entre no servidor de nome com a senha fornecida
(se necessário).

`leave-server <id> <nome>`
Faz com que o usuário conectado em algum servidor saia do mesmo.

`list-participants <id>`
Lista os participantes presentes no servidor que o usuário com o id passado está visualizando.

`list-channels <id>`
Lista os canais do servidor que o usuário com o id passado está vizualizando.

`create-channel <id> <nome>`
Cria um canal em um servidor com o nome passado. As mensagens serão enviadas dentro do canal.

`enter-channel <id> <nome>`
Faz com que o usuário com id dado entre em um canal específico.

`leave-channel <id>`
Faz com que o usuário com id dado saia do canal que está visualizando atualmente.

`send-message <id> <mensagem>`
Envia uma mensagem no canal em que o usuáiro com id passado está visualizando.

`list-messages <id>`
Lista as mensagem no canal que o usuário com id passado está visualizando.
