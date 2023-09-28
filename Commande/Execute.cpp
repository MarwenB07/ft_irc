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
	std::vector<std::string> CMD;
	std::vector<std::string> splited_cmd;
	if (message.find("\r\n") != std::string::npos)
		CMD = newSplit(message, "\r\n");
	else
		CMD = newSplit(message, "\n");
	std::map<int, User *>::iterator use = _users.find(socket);
	if (use == _users.end())
		return ;

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
		else if (cmd == 3)
			Topic(use->second, channel, line);
		else if (cmd == 4)
			Kick(use->second, channel, line);
		else if (cmd == 5)
			Invite(use->second, channel, line);
		else if (cmd == 6)
			Part(use->second, channel, line);
		else if (cmd == 7)
			Mode(use->second, channel, line);
		else if (cmd == 8)
			continue ;
		else if (cmd == 10)
			send_msg(socket, ERR_ALREADYREGISTERED(use->second->getNickname()));
		else if (cmd == 11)
			send_msg(socket, HELPER);
		else if (cmd == 12)
			Names(use->second, channel, line);
		else if (cmd == -1)
			send_msg(socket, INVALIDE);
		++it;
	}
}
