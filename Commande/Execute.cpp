#include "../Server/Server.hpp"

/*

PRIVMSG == 1

JOIN == 2

INVALIDE CMD == -1

*/

void Server::ExectuteIrcCmd(int cmd, int socket, std::vector<std::string> split, std::map<std::string, Channel> channel)
{
	if (cmd == 1)
		PrivateMsg(_users, split, socket, channel);
	else if (cmd == 2)
		Join(socket, split, channel);
	else if (cmd == -1)
		send(socket, Print(INVALIDE).c_str(), Print(INVALIDE).length(), 0);
}