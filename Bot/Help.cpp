#include "Bot.hpp"

void clearBuffer(char *buffer, int len)
{
	for (int i = 0; i < len; i++)
		buffer[i] = '\0';
}

void send_msg(int socket, std::string message)
{
	send(socket, message.c_str(), message.size(), 0);
}

std::vector<std::string> newSplit(std::string s, std::string c)
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

std::vector<std::string> s_split(std::string str)
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

std::string takeMessage(std::string message)
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