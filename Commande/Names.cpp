#include "../Server/Server.hpp"

void Server::Names(User *user)
{
	std::vector<User *>::iterator it = _users_class_list.begin();

	for (; it != _users_class_list.end(); it++)
	{
		send_msg(user->getClientSocket(), RPL_NAMREPLY(user->getNickname(), "=", "", ""));
	}
}
