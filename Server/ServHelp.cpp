#include "Server.hpp"

/* semi-usefull */

std::string Server::s_itoa(int number)
{
	std::string returner;
	std::stringstream ss;
	ss << number;
	returner = ss.str();
	return (returner);
}

// clean my buffer for recv //

void Server::cleanBuffer(char *buffer, int len)
{
	for (int i = 0; i < len; i++)
		buffer[i] = '\0';
}

// link the nickname and the message //

std::string Server::makeIdenticalChat(char *buffer, std::string name)
{
	std::string namedChat;

	namedChat = name.erase(name.length() - 1, name.length());
	namedChat.append(" : ");
	namedChat.append(buffer);

	return (namedChat);
}

// imagine tu sais pas utiliser substr ... //

std::vector<std::string> Server::s_split(std::string str)
{
	int pos = 0;
	std::vector<std::string> splited;
	std::string sub;

	for (int i = 0; str.c_str()[i];)
	{
		if (str.c_str()[i] && (str.c_str()[i] == '\t' || str.c_str()[i] == '\n' || str.c_str()[i] == ' ' || str.c_str()[i] == '\r' || str.c_str()[i] == '\v'))
			i++, pos++;
		else
		{
			while (str.c_str()[i] && str.c_str()[i] != '\n' && str.c_str()[i] != '\t' && str.c_str()[i] != ' ' && str.c_str()[i] != '\r' && str.c_str()[i] != '\v')
				i++;
			sub = str.substr(pos, i - pos);
			splited.push_back(sub);
			pos = i;
		}
	}
	return (splited);
}

// new split //

std::vector<std::string> Server::newSplit(std::string s, std::string c)
{
    std::vector<std::string>    splitted;
    int last_space = 0;
    size_t space = s.find(c, 0);

    while (space != std::string::npos) {
        splitted.push_back(s.substr(last_space, space - last_space));
        last_space = space + 1;
        space = s.find(c, space + 1);
    }
    splitted.push_back(s.substr(last_space, s.size()));
    return (splitted);
}

// join if ^D //

void Server::Ctrl_D_Join(char *buffer, std::string oldMess, std::string newMess)
{
	int i = 0;
	std::cout << "old = " << oldMess << " / new = " << newMess << std::endl;
	cleanBuffer(buffer, 513);
	for (; oldMess.c_str()[i];)
	{
		buffer[i] = oldMess.c_str()[i];
		i++;
	}
	for (int j = 0; newMess.c_str()[j]; j++)
	{
		buffer[i] = newMess.c_str()[j];
		i++;
	}
}

// give all information //

//void Server::WelcomeToIrc(int socket)
//{
//	send(socket, Print(IRC_WELCOME).c_str(), Print(IRC_WELCOME).length(), 0);
//	send(socket, Print(IRC_INFO_LIST).c_str(), Print(IRC_INFO_LIST).length(), 0);
//
//	std::map<int, User *>::iterator use;
//	std::string info;
//
//	for (std::vector<int>::iterator it = _clients.begin(); it != _clients.end(); ++it)
//	{	
//		use = _users.find(*it);
//
//		info = s_itoa(*it);
//		while (info.length() < 10)
//			info.append(" ");
//		info.append(use->second->getNickname());
//		info.append("\n");
//		if (use->second->getUserStatus() == true)
//			send(socket, Print(info).c_str(), Print(info).length(), 0);
//	}
//}

// give all information //

void Server::WelcomeToIrc(int socket, User *user)
{
	std::string time = "010205";
	std::string unknow = "inconnue";

	send_msg(socket, RPL_WELCOME(user->getNickname(), getServerName(), user->getUsername(), user->getUserHost())); // 001
	send_msg(socket, RPL_YOURHOST(user->getNickname(), getServerName(), getServerVersion())); // 002
	send_msg(socket, RPL_CREATED(user->getNickname(), time)); // 003
	send_msg(socket, RPL_MYINFO(user->getNickname(), getServerName(), getServerVersion(), unknow, unknow, unknow)); // 004
	send_msg(socket, RPL_ISUPPORT(user->getNickname(), unknow)); // 005

	send_msg(socket, RPL_LUSERCLIENT(user->getNickname(), s_itoa(getNbClient()))); // 251

	// mettre operator

	
	send_msg(socket, RPL_LUSEROP(user->getNickname(), s_itoa(getNbClient()))); // 252
	send_msg(socket, RPL_LUSERCHANNELS(user->getNickname(), s_itoa(getNbClient()))); // 254 
	send_msg(socket, RPL_LUSERME(user->getNickname(), s_itoa(getNbClient()))); // 255
	send_msg(socket, RPL_LOCALUSERS(user->getNickname(), s_itoa(getNbClient()), s_itoa(getMaxClient()))); // 265
	send_msg(socket, RPL_GLOBALUSERS(user->getNickname(), s_itoa(getNbClient()), s_itoa(getMaxClient()))); // 266

	// message //

	send_msg(socket, IRC_WELCOME);

	// end //

	send_msg(socket, RPL_ENDOFMOTD(user->getNickname()));
}

// functiom just to view invisible char //

std::string visiblechar(char *buffer)
{
	char *test = strdup(buffer);
	int j = 0;

	for (int i = 0; buffer[i]; i++)
	{
		if (buffer[i] == '\n')
		{
			test[j++] = '\\';
			test[j] = 'n';
		}
		else if (buffer[i] == '\r')
		{
			test[j++] = '\\';
			test[j] = 'r';
		}
		else if (buffer[i] == '\v')
		{
			test[j++] = '\\';
			test[j] = 'v';
		}
		else if (buffer[i] == '\f')
		{
			test[j++] = '\\';
			test[j] = 'f';
		}
		else if (buffer[i] == '\t')
		{
			test[j++] = '\\';
			test[j] = 't';
		}
		j++;
	}
	return (test);
}

void Server::sends_msg(int socket, std::string message, std::vector<User *> all, int open)
{
	for (std::vector<User *>::iterator it = all.begin(); it != all.end(); ++it)
		if ((*it)->getClientSocket() != socket)
			send((*it)->getClientSocket(), message.c_str(), message.size(), 0);
	if (open == 0)
		send(socket, message.c_str(), message.size(), 0);
}

void send_msg(int socket, std::string message)
{
	send(socket, message.c_str(), message.size(), 0);
}

User *Server::FindUser(std::string nickname)
{
	for (std::vector<User *>::iterator it = _users_class_list.begin(); it != _users_class_list.end(); ++it)
		if ((*it)->getNickname() == nickname)
			return ((*it));
	return (NULL);
}

Channel *Server::FindChannel(std::string channelname)
{
	std::map<std::string, Channel *>::iterator it = _channel.find(channelname);
	return (it->second);
}

std::string Server::correctChar(std::string line, char c)
{
	int recur = 0;
	std::string newWord = "";

	for (int i = 0; line.c_str()[i]; i++)
	{
		if (line.c_str()[i] == c)
			recur++;
	}
	return (newWord);
}