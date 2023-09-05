#include "Server.hpp"

void Server::UserStep(int socket, int returner, User *user)
{
	if (returner == 1)
	{
		std::vector<std::string>::iterator it = _splited.begin();
		++it;
		user->MakeTrue("USER");
		user->CompleteUser(user->getNickname(), *it);
		send_msg(socket, GoodUser);
		UpNbClients();
		setClientConnected(1);
		WelcomeToIrc(socket, user);
	}
}

void Server::PassStep(int socket, std::string message, int returner, User *user)
{
	if (returner == 1)
	{
		user->MakeTrue("NICK");
		user->CompleteUser(message.c_str() + 5, user->getUsername());
		send_msg(socket, GoodNick);
	}
}

void Server::NickStep(int socket, int returner, User *user)
{
	if (returner == 1)
	{
		user->MakeTrue("PASS");
		send_msg(socket, GoodPass);
	}
}