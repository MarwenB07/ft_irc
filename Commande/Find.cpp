#include "../Server/Server.hpp"

int Server::FindCmd(std::vector<std::string> message, int socket)
{
	std::vector<std::string>::iterator it = message.begin();

	if (*it == "PRIVMSG")
		return (1);
	else
		return (-1);
	return (0);
}

int Server::FindSocket(std::string name)
{ 
	for (std::vector<int>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		std::map<int, User>::iterator users = _users.find(*it);

		if (name == users->second.getNickname())
			return (users->second.getClientSocket());
	}
	return (-1);
}