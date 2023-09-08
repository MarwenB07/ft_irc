#include "../Server/Server.hpp"

void Server::Topic(User *user, std::map<std::string, Channel *> channel, std::string line)
{
	// si le mode +t alors operator pour changer le topic //

	std::string word;
	std::vector<std::string> split_list = s_split(line);
	std::vector<std::string>::iterator list = split_list.begin();
	
	std::cout << split_list.size() << std::endl;

	if (split_list.size() <= 4 && line.find("#") == std::string::npos && line.find(":") == std::string::npos)
		return (send_msg(user->getClientSocket(), ERR_NEEDMOREPARAMS(user->getNickname(), *list)));
	++list;
	word = *list;
	if (ChannelAlreadyExists(word, channel, 0) == false)
		return (send_msg(user->getClientSocket(), ERR_NOSUCHCHANNEL(user->getNickname(), word)));

	std::cout << word << std::endl;

	std::map<std::string, Channel *>::iterator ActualChannel = channel.find(word.c_str() + 1);
	if (AlreadyInChannel(user, ActualChannel->second) == false)
		return (send_msg(user->getClientSocket(), ERR_NOTONCHANNEL(user->getNickname(), word)));
	else if (checkIsOperator(user, ActualChannel->second) == false && ActualChannel->second->getChannelMode().find("t") != std::string::npos)
		return (send_msg(user->getClientSocket(), ERR_CHANOPRIVSNEEDED(user->getNickname(), word)));

	++list;
	word = *list;

	// cmd = TOPIC #channel
	if (split_list.size() == 2)
		ActualChannel->second->SendTopic(user);
	else if (split_list.size() == 3 && word == ":")
		ActualChannel->second->setChannelTopic("\r\n");
	else if (split_list.size() >= 3 && word.find(":") < 2)
	{
		word = takeMessage(line);
		word.erase(0, 2);
		std::cout << word << std::endl;
		if (word == "NONE")
			ActualChannel->second->setChannelTopic("\r\n");
		else
			ActualChannel->second->setChannelTopic(word.append("\r\n"));
	}
}