#include "../Server/Server.hpp"

/*

KICK #Finnish Matthew           ; Command to kick Matthew from
                                   #Finnish

KICK &Melbourne Matthew         ; Command to kick Matthew from
                                &Melbourne
KICK #Finnish John :Speaking English

*/

void Server::Kick(User *user, std::map<std::string, Channel *> channel, std::string line)
{
	std::string word;
	std::vector<std::string> split_list = s_split(line);
	std::vector<std::string>::iterator list = split_list.begin();
	
	std::cout << split_list.size() << std::endl;

	if ((split_list.size() <= 4 && line.find("#") == std::string::npos && line.find(":") == std::string::npos) || split_list.size() == 2)
		return (send_msg(user->getClientSocket(), ERR_NEEDMOREPARAMS(user->getNickname(), *list)));

	++list;
	word = *list;

	if (ChannelAlreadyExists(word, channel, 0) == false)
		return (send_msg(user->getClientSocket(), ERR_NOSUCHCHANNEL(user->getNickname(), word)));

	std::cout << word << std::endl;

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
		ActualChannel->second->KickUser(use, "ez", 0);
}