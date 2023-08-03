#include "User.hpp"

User::User(std::string nickname, std::string username, int clientSocket) : _nickname(nickname), _username(username), _clientSocket(clientSocket)
{
	_status = false;
}

int User::getClientSocket(void) const
{
	return (_clientSocket);
}

bool User::getStatus(void) const
{
	return (_status);
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

