#include "../Server/Server.hpp"

void Server::Topic(User *user, std::map<std::string, Channel *> channel, std::string line)
{
	std::string word;
	std::vector<std::string> split_list = s_split(line);
	std::vector<std::string>::iterator list = split_list.begin();

	if (split_list.size() <= 4 && line.find("#") == std::string::npos && line.find(":") == std::string::npos)
		return (send_msg(user->getClientSocket(), ERR_NEEDMOREPARAMS(user->getNickname(), *list)));
	++list;
	word = *list;
	if (ChannelAlreadyExists(word, channel, 0) == false)
		return (send_msg(user->getClientSocket(), ERR_NOSUCHCHANNEL(user->getNickname(), word)));

	std::map<std::string, Channel *>::iterator ActualChannel = channel.find(word.c_str() + 1);
	if (AlreadyInChannel(user, ActualChannel->second) == false)
		return (send_msg(user->getClientSocket(), ERR_NOTONCHANNEL(user->getNickname(), word)));
	else if (checkIsOperator(user, ActualChannel->second) == false && ActualChannel->second->getChannelTopicRestric() == true)
		return (send_msg(user->getClientSocket(), ERR_CHANOPRIVSNEEDED(user->getNickname(), word)));

	++list;
	word = *list;
	if (split_list.size() == 2)
		ActualChannel->second->SendTopic(user);
	else if (split_list.size() == 3 && word == ":")
	{
		ActualChannel->second->setChannelTopic("\r\n");
		ActualChannel->second->setTopicCreationTime();
		ActualChannel->second->setTopicSeter(user->getNickname());
		sends_msg(user->getClientSocket(), RPL_TOPICWHOTIME(user->getNickname(), ActualChannel->second->getChannelName(), ActualChannel->second->getTopicSeter(), std::to_string(ActualChannel->second->getTopicCreationTime())), ActualChannel->second->getChannelAuthorized(), 0);
	}
	else if (split_list.size() >= 3 && word.find(":") < 2)
	{
		std::string newWord = line.c_str() + 1;
		word = takeMessage(newWord + " ");
		std::cout << word << std::endl;
		if (word == "NONE")
			ActualChannel->second->setChannelTopic("\r\n");
		else
			ActualChannel->second->setChannelTopic(word.append("\r\n"));
		ActualChannel->second->setTopicCreationTime();
		ActualChannel->second->setTopicSeter(user->getNickname());
		sends_msg(user->getClientSocket(), RPL_TOPICWHOTIME(user->getNickname(), ActualChannel->second->getChannelName(), ActualChannel->second->getTopicSeter(), std::to_string(ActualChannel->second->getTopicCreationTime())), ActualChannel->second->getChannelAuthorized(), 0);
	}
}