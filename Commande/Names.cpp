#include "../Server/Server.hpp"

void Server::Names(User *user, std::map<std::string, Channel *> channel, std::string line)
{
	std::string word;
	std::vector<std::string> pass;
	std::string liste = "";
	std::vector<std::string> split_list = s_split(line);
	std::vector<std::string>::iterator list = split_list.begin();

	if (split_list.size() > 2)
		return (send_msg(user->getClientSocket(), ERR_NEEDMOREPARAMS(user->getNickname(), (*list))));
	else if (line == "NAMES" || (split_list.size() == 2 && line.find("#") == std::string::npos))
	{
		for (std::vector<User *>::iterator it = _users_class_list.begin(); it != _users_class_list.end(); ++it)
		{
			liste.append((*it)->getNickname());
			liste.append(" ");
		}
		send_msg(user->getClientSocket(), RPL_NAMREPLY2(user->getNickname(), "=", getServerName(), liste));
	}
	else if (split_list.size() == 2)
	{
		++list;
		word = *list;
		word = correctChar(word, ',');
		pass = newSplit(word, ",");
		for (std::vector<std::string>::iterator it = pass.begin(); it != pass.end(); ++it)
		{
			if (ChannelAlreadyExists(*it, channel, 0) == true)
			{
				for (std::map<std::string, Channel *>::iterator it2 = channel.begin(); it2 != channel.end(); ++it2)
				{
					word = *it;
					word.erase(0, 1);
					std::cout << (it2)->second->getChannelName() << std::endl;
					std::cout << word << std::endl;
					if ((it2)->second->getChannelName() == word)
					{
						liste = createListOfMember((it2)->second->getChannelAuthorized(), (it2)->second);
						break;
					}
					std::cout << "end" << std::endl;
				}
				send_msg(user->getClientSocket(), RPL_NAMREPLY2(user->getNickname(), "=", (*it), liste));
				send_msg(user->getClientSocket(), RPL_ENDOFNAMES2(user->getNickname(), (*it)));
			}
		}
	}
}