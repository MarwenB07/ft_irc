#include "User.hpp"

User::User(int clientSocket) : _clientSocket(clientSocket)
{
	_nickStatus = false;
	_passwordStatus = false;
	_userStatus = false;
	_nickname = "NULL_NICKNAME";
	_username = "NULL_USERNAME";
}

void User::CompleteUser(std::string nickname, std::string username)
{	
	char *test = strdup(nickname.c_str());
	for (int i = 0; test[i]; i++)
	{
		if (test[i] == '\r' || test[i] == '\n')
			test[i] = '\0';
	}
	_nickname = test;
	_username = username;
	delete test;
}

void User::MakeTrue(std::string statusName)
{
	if (statusName == "PASS")
		_passwordStatus = true;
	else if (statusName == "NICK")
		_nickStatus = true;
	else if (statusName == "USER")
		_userStatus = true;
}

int User::getClientSocket(void) const
{
	return (_clientSocket);
}

bool User::getPasswordStatus(void) const
{
	return (_passwordStatus);
}

bool User::getNickStatus(void) const
{
	return (_nickStatus);
}

bool User::getUserStatus(void) const
{
	return (_userStatus);
}

std::string User::getNickname(void) const
{
	return (_nickname);
}

std::string User::getUsername(void) const
{
	return (_username);
}

User::~User()
{

}

