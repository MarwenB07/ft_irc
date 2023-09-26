#include "../Server/Server.hpp"

void Server::Names()
{
	std::vector<User *>::iterator it = _users_class_list.begin();

	for (; it != _users_class_list.end(); it++)
	{
		std::cout << (*it)->getNickname() << std::endl;
	}
}