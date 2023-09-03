#include "../Server/Server.hpp"

// bug niveau des PRIVMSG avec un canal le vector _authorized enregistre la socket dans AddChannel mais apres elle oublie cette connasse

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

std::vector<std::string> Server::catch_nickname(std::vector<std::string> buffer)
{
	std::string name;
	std::vector<std::string> nick;
	std::vector<std::string>::iterator it = buffer.begin();

	++it;

	name = *it;
	nick = newSplit(name, ",");
	return (nick);
}

void Server::PrivateMsg(std::map<int, User *> users, std::vector<std::string> buffer, int socket, std::map<std::string, Channel *> channelist)
{
	int dst_socket;
	std::vector<std::string>::iterator message = buffer.begin();
	std::string sended_message = takeMessage(getMessage());
	std::string user_client;

	++message;

	if (sended_message == "NONE")
	{
		send(socket, Print(PRIVMSG_ERR).c_str(), Print(PRIVMSG_ERR).length(), 0); 
		return ;
	}
	
	std::map<int, User *>::iterator cuser = users.find(socket);
	user_client = cuser->second->getNickname();
	std::vector<std::string> nickmame = catch_nickname(buffer);
	std::vector<std::string>::iterator itt = nickmame.begin();
	std::string first = *itt;

	if (first.find('#') != std::string::npos && first.find('#') < 2)
	{
		for (std::vector<std::string>::iterator it = nickmame.begin(); it != nickmame.end(); ++it)
		{
			first = *it;
			if (ChannelAlreadyExists(first, _channel) == true)
			{
				std::map<std::string, Channel *>::iterator channel = channelist.find(first);
				std::vector<User *> sock = channel->second->getChannelAuthorized();
				for (std::vector<User *>::iterator socke = sock.begin(); socke != sock.end(); ++socke)
				{
					User *use = channel->second->CpyUser(*socke);
					dst_socket = use->getClientSocket();
					std::cout << dst_socket << std::endl;
					delete use;
				}

				for (std::vector<User *>::iterator socke = sock.begin(); socke != sock.end(); ++socke)
				{
					User *use = channel->second->CpyUser(*socke);
					dst_socket = use->getClientSocket();
					delete use;
					if (dst_socket != socket)
					{
						std::cout << "test message" << std::endl;
						send(dst_socket, Print("message\r\n").c_str(), Print("message\r\n").length(), 0);
					}
				}
			}
			else
				send(socket, Print(UNKNOW_CHANNEL(first)).c_str(), Print(UNKNOW_CHANNEL(first)).length(), 0); 
		}
	}
	else
	{
		for (std::vector<std::string>::iterator it = nickmame.begin(); it != nickmame.end(); ++it)
		{
			dst_socket = FindSocket(*it);
			if (dst_socket != -1)
			{
				std::map<int, User *>::iterator user = users.find(dst_socket);
				send(dst_socket, PRIVMSG(user_client, user->second->getNickname(), sended_message).c_str(), 
					PRIVMSG(user_client, user->second->getNickname(), sended_message).length(), 0);
			}
			else
				send(socket, Print(PRIVMSG_ERR_USER(*it)).c_str(), Print(PRIVMSG_ERR_USER(*it)).length(), 0); 
		}
	}
}