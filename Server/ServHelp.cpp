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

// print error message when the client send a message with more character than 512 //

std::string Server::Print(std::string string) const
{
	return (string);
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

// give all information //

void Server::WelcomeToIrc(int socket)
{
	send(socket, Print(IRC_WELCOME).c_str(), Print(IRC_WELCOME).length(), 0);
	send(socket, Print(IRC_INFO_LIST).c_str(), Print(IRC_INFO_LIST).length(), 0);

	std::map<int, User>::iterator use;
	std::string info;

	for (std::vector<int>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{	
		use = _users.find(*it);

		info = s_itoa(*it);
		while (info.length() < 10)
			info.append(" ");
		info.append(use->second.getNickname());
		info.append("\n");
		if (use->second.getUserStatus() == true)
			send(socket, Print(info).c_str(), Print(info).length(), 0);
	}
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