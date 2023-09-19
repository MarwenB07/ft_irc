#include "../Server/Server.hpp"

void Server::Kick(User *user, std::map<std::string, Channel *> channel, std::string line)
{
	std::string liste = "";
	std::string word;
	std::vector<std::string> split_list = s_split(line);
	std::vector<std::string>::iterator list = split_list.begin();

	if ((split_list.size() <= 4 && line.find("#") == std::string::npos && line.find(":") == std::string::npos) || split_list.size() == 2)
		return (send_msg(user->getClientSocket(), ERR_NEEDMOREPARAMS(user->getNickname(), *list)));

	++list;
	word = *list;

	if (ChannelAlreadyExists(word, channel, 0) == false)
		return (send_msg(user->getClientSocket(), ERR_NOSUCHCHANNEL(user->getNickname(), word)));

	std::map<std::string, Channel *>::iterator ActualChannel = channel.find(word.c_str() + 1);
	if (AlreadyInChannel(user, ActualChannel->second) == false)
		return (send_msg(user->getClientSocket(), ERR_NOTONCHANNEL(user->getNickname(), word)));
	else if (checkIsOperator(user, ActualChannel->second) == false)
		return (send_msg(user->getClientSocket(), ERR_CHANOPRIVSNEEDED(user->getNickname(), word)));

	++list;
	word = *list;

	if (checkUserExist(word) == false)
		return (send_msg(user->getClientSocket(), ERR_USERNOTINCHANNEL(user->getNickname(), word, ActualChannel->first)));

	User *use = FindUser(word);

	if (AlreadyInChannel(use, ActualChannel->second) == false)
		return (send_msg(user->getClientSocket(), ERR_USERNOTINCHANNEL(user->getNickname(), word, ActualChannel->first)));
	else if (checkIsCreator(use, ActualChannel->second) == true)
		return (send_msg(user->getClientSocket(), ERR_CHANOPRIVSNEEDED(user->getNickname(), word)));
	if (split_list.size() == 3)
	{
		ActualChannel->second->KickUser(use, user, "", 0);
		liste = createListOfMember(ActualChannel->second->getChannelAuthorized(), ActualChannel->second);
		sends_msg(user->getClientSocket(), RPL_NAMREPLY(user->getNickname(), "=", ActualChannel->second->getChannelName() ,liste), ActualChannel->second->getChannelAuthorized(), 0);
		return (sends_msg(user->getClientSocket(), RPL_ENDOFNAMES(user->getNickname(), ActualChannel->second->getChannelName()), ActualChannel->second->getChannelAuthorized(), 0));
	}
	
	++list;
	word = *list;

	if (split_list.size() == 4 && word == ":")
		return ;
	else if (split_list.size() >= 4 && word.find(":") < 2)
	{
		word = takeMessage(line);
		ActualChannel->second->KickUser(use, user, word, 1);
		liste = createListOfMember(ActualChannel->second->getChannelAuthorized(), ActualChannel->second);
		sends_msg(user->getClientSocket(), RPL_NAMREPLY(user->getNickname(), "=", ActualChannel->second->getChannelName() ,liste), ActualChannel->second->getChannelAuthorized(), 0);
		return (sends_msg(user->getClientSocket(), RPL_ENDOFNAMES(user->getNickname(), ActualChannel->second->getChannelName()), ActualChannel->second->getChannelAuthorized(), 0));
	}
	return ;
}