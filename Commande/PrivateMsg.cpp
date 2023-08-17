#include "../Server/Server.hpp"

static std::string takeMessage(std::string message)
{
	int i = 0;

	std::string sended;

	if (message.find(":") != std::string::npos)
	{
		i = message.find(":");
		sended = message.substr(i, message.length() - i);
		return (sended);
	}
	return ("NONE");
}

static std::vector<std::string> catch_nickname(std::vector<std::string> buffer)
{
	int n = 0;
	int i = 0;
	std::string name;
	std::vector<std::string> nick;

	for (std::vector<std::string>::iterator it = buffer.begin(); it != buffer.end(); ++it)
	{
		n++;
		name = *it;
		if (name.c_str()[0] == ':' && (name.c_str()[1] != ' ' || name.c_str()[1] != '\t'))
			break;
	}

	n -= 2;
	std::vector<std::string>::iterator it = buffer.begin();
	++it;

	while (i < n)
	{
		name = *it;
		if (i + 1 < n && name.find(",") != std::string::npos)
			nick.push_back(name.erase(name.find(","), 1));
		else
			nick.push_back(name);
		i++;
		++it;
	}
	return (nick);
}

void Server::PrivateMsg(std::map<int, User> users, std::vector<std::string> buffer, int socket)
{
	int dst_socket;
	std::vector<std::string>::iterator message = buffer.begin();
	std::string sended_message = takeMessage(getMessage());
	++message;

	if (sended_message == "NONE")
	{
		send(socket, Print(PRIVMSG_ERR).c_str(), Print(PRIVMSG_ERR).length(), 0); 
		return ;
	}
	
	std::vector<std::string> nickmame = catch_nickname(buffer);
	for (std::vector<std::string>::iterator it = nickmame.begin(); it != nickmame.end(); ++it)
	{
		dst_socket = FindSocket(*it);
		if (dst_socket != -1)
		{
			std::map<int, User>::iterator user = users.find(dst_socket);
			std::map<int, User>::iterator cuser = users.find(socket);
			send(dst_socket, PRIVMSG(user->second.getNickname(), cuser->second.getNickname(), sended_message).c_str(), 
				PRIVMSG(user->second.getNickname(), cuser->second.getNickname(), sended_message).length(), 0);
		}
		else
			send(socket, Print(PRIVMSG_ERR_USER(*it)).c_str(), Print(PRIVMSG_ERR_USER(*it)).length(), 0); 
	}
}