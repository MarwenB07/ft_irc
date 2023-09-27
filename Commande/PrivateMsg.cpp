#include "../Server/Server.hpp"

// bug niveau des PRIVMSG avec un canal le vector _authorized enregistre la socket dans AddChannel mais apres elle oublie cette connasse

std::string takeMessage(std::string message, int stop)
{
	int i = 0;
	int j = 0;
	int s = 0;
	std::string sended;

	j = message.size();
	while (message.c_str()[i] && s != stop)
	{
		while (message.c_str()[i] == ' ' || (message.c_str()[i] >= 10 && message.c_str()[i] <= 13))
			i++;
		while (!(message.c_str()[i] == ' ' || (message.c_str()[i] >= 10 && message.c_str()[i] <= 13)))
			i++;
		s++;
	}
	while (message.c_str()[i] == ' ' || (message.c_str()[i] >= 10 && message.c_str()[i] <= 13))
			i++;
	if (i != j)
	{
		sended = message.substr(i, j - (i + 1));
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
	std::map<int, User *>::iterator cuser = users.find(socket);

	if (buffer.size() < 3)
		return (send_msg(socket, ERR_NOTEXTTOSEND(cuser->second->getNickname())));

	std::string user_client;

	std::string sended_message = takeMessage(getMessage(), 2);

	std::cout << sended_message << std::endl; 

	++message;
	
	user_client = cuser->second->getNickname();
	std::vector<std::string> nickmame = catch_nickname(buffer);
	std::vector<std::string>::iterator itt = nickmame.begin();
	std::string first = *itt;

	if (sended_message == "NONE")
	{
		send_msg(socket, ERR_NOTEXTTOSEND(cuser->second->getNickname())); 
		return ;
	}

	if (first.find('#') != std::string::npos && first.find('#') < 1)
	{
		for (std::vector<std::string>::iterator it = nickmame.begin(); it != nickmame.end(); ++it)
		{
			first = *it;
			if (ChannelAlreadyExists(first, _channel, 0) == true)
			{
				std::map<std::string, Channel *>::iterator channel = channelist.find(first.c_str() + 1);
				if (AlreadyInChannel(cuser->second, channel->second) == true)
					channel->second->SendMsgToChannel(PRIVMSG(cuser->second->getNickname(), first, sended_message), socket);
				else
					send_msg(socket, ERR_CANNOTSENDTOCHAN(cuser->second->getNickname(), first));
			}
			else
				send_msg(socket, ERR_NOSUCHNICK(cuser->second->getNickname(), first)); 
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
				send_msg(dst_socket, PRIVMSG(user_client, user->second->getNickname(), sended_message));
			}
			else
				send_msg(socket, ERR_NOSUCHNICK(cuser->second->getNickname(), (*it)));
		}
	}
}