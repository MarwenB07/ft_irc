#include "../Server/Server.hpp"

void Server::PrivateMsg(std::map<int, User> users, std::vector<std::string> buffer, int socket)
{
	std::vector<std::string>::iterator message = buffer.begin();
	++message;
	
	// function to return the good socket just with name return - 1 if error //
}