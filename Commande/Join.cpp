#include "../Server/Server.hpp"

void Server::CreateChannel(void)
{
	std::cout << "caca test" << std::endl;
	send(getSocket(), Print("#channel\r\n").c_str(), Print("#channel\r\n").size(), 0);
}