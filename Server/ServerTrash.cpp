#include "Server.hpp"

int Server::UserStep(int socket, int returner, User *user)
{
	if (returner == 1)
	{
		std::vector<std::string>::iterator it = _splited.begin();
		++it;
		user->MakeTrue("USER");
		user->CompleteUser(user->getNickname(), *it);
		if (user->getNickStatus() == true && getPass() != getPassword())
			return (-1);
		else if (user->getNickStatus() == false)
			return (1);
		setClientConnected(1);
		WelcomeToIrc(socket, user);
		if (user->getNickStatus() == true && getPass() == getPassword())
			user->MakeTrue("PASS");
		AddUserClassList(user);
		return (1);
	}
	return (0);
}

int Server::NickStep(std::string message, int returner, User *user)
{
	if (returner == 1)
	{
		user->MakeTrue("NICK");
		user->CompleteUser(message.c_str() + 5, user->getUsername());
		AddUserNickList(user->getNickname());
		if (user->getUserStatus() == true && getPass() != getPassword())
			return (-1);
		else if (user->getUserStatus() == false)
			return (1);
		setClientConnected(1);
		WelcomeToIrc(user->getClientSocket(), user);
		if (user->getNickStatus() == true && getPass() == getPassword())
			user->MakeTrue("PASS");
		AddUserClassList(user);
	}
	return (0);
}