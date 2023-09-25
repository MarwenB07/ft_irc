#include "Server.hpp"

int Server::UserStep(int socket, int returner, User *user)
{
	if (returner == 1)
	{
		std::vector<std::string>::iterator it = _splited.begin();
		++it;
		user->MakeTrue("USER");
		user->CompleteUser(user->getNickname(), *it);
		if (getPass() != getPassword())
			return (-1);
		UpNbClients();
		setClientConnected(1);
		WelcomeToIrc(socket, user);
		AddUserClassList(user);
		user->MakeTrue("PASS");
		return (1);
	}
	return (0);
}

void Server::PassStep(int socket, int returner, User *user)
{
	if (returner == 1)
		user->MakeTrue("PASS");
}

void Server::NickStep(int socket, std::string message, int returner, User *user)
{
	if (returner == 1)
	{
		user->MakeTrue("NICK");
		user->CompleteUser(message.c_str() + 5, user->getUsername());
		AddUserNickList(user->getNickname());
	}
}