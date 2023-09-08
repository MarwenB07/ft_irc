#include "../Server/Server.hpp"

int Server::FindCmd(std::vector<std::string> message)
{
	std::vector<std::string>::iterator it = message.begin();

	if (*it == "HELPER")
		return (0);
	else if (*it == "PRIVMSG")
		return (1);
	else if (*it == "JOIN")
		return (2);
	else if (*it == "TOPIC")
		return (3);
	else if (*it == "KICK")
		return (4);
	else if (*it == "INVITE")
		return (5);
	else if (*it == "PART")
		return (6);
	return (-1);
}

int Server::FindSocket(std::string name)
{ 
	int i;
	for (std::vector<int>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		i = *it;
		std::map<int, User *>::iterator users = _users.find(*it);
		if (name == users->second->getNickname())
			return (users->second->getClientSocket());
	}
	return (-1);
}