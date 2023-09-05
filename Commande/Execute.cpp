#include "../Server/Server.hpp"

/*

PRIVMSG == 1

JOIN == 2

INVALIDE CMD == -1

*/

void Server::ExectuteIrcCmd(int socket, std::string message, std::map<std::string, Channel *> channel)
{
	int cmd;
	std::string line;
	std::vector<std::string> splited_cmd;
	std::vector<std::string> CMD = newSplit(message, "\r\n");

	int j = CMD.size();

	std::vector<std::string>::iterator it = CMD.begin();

	for (int i = 0; i < j - 1; i++)
	{
		line = *it;
		splited_cmd = s_split(line);
		cmd = FindCmd(splited_cmd);
		if (cmd == 1)
			PrivateMsg(_users, splited_cmd, socket, channel);
		else if (cmd == 2)
			Join(socket, splited_cmd, channel);
		else if (cmd == -1)
			send_msg(socket, INVALIDE);
		++it;
	}
}

/*

int cmd;
	std::string line;
	std::vector<std::string> splited_cmd;
	std::vector<std::string> CMD = newSplit(message, "\r\n");

	std::cout << "en vie\n";

	for (std::vector<std::string>::iterator it = CMD.begin(); it != CMD.end(); ++it)
	{
		line = *it;
		splited_cmd = s_split(line);
		cmd = FindCmd(splited_cmd);
		if (cmd == 1)
			PrivateMsg(_users, splited_cmd, socket, channel);
		else if (cmd == 2)
			Join(socket, splited_cmd, channel);
		else if (cmd == -1)
			send_msg(socket, INVALIDE);
	}
}
*/