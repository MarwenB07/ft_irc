#include "../Server/Server.hpp"

// check avant si le channel est sur inviter
// envoie un message a l'inviter

void Server::SendInvitedChannel(User *user, std::vector<Channel *> channel)
{
	std::string list = "";

	for (std::vector<Channel *>::iterator it = channel.begin(); it != channel.end(); ++it)
	{
		if (checkIsInvited(user, (*it)) == true)
		{
			list.append("#");
			list.append((*it)->getChannelName());
			if (it + 1 != channel.end())
				list.append(", ");
		}
	}
	send_msg(user->getClientSocket(), RPL_INVITELIST(user->getNickname(), list));
	send_msg(user->getClientSocket(), RPL_ENDOFINVITELIST(user->getNickname()));
}

void Server::Invite(User *user, std::map<std::string, Channel *> channel, std::string line)
{
	std::string word;
	std::vector<std::string> split_list = s_split(line);
	std::vector<std::string>::iterator list = split_list.begin();

	if (line == "INVITE\r\n")
		SendInvitedChannel(user, _channel_class_list);
	else if (split_list.size() != 3)
		return (send_msg(user->getClientSocket(), ERR_NEEDMOREPARAMS(user->getNickname(), *list)));
	
	++list;
	word = *list;

	if (checkUserExist(word) == false)
		return ;
	
	User *use = FindUser(word);

	++list;
	word = *list;
	if (ChannelAlreadyExists(word, channel, 0) == false)
		return (send_msg(user->getClientSocket(), ERR_NOSUCHCHANNEL(user->getNickname(), word)));

	std::map<std::string, Channel *>::iterator theChannel = channel.find(word.c_str() + 1);

	if (AlreadyInChannel(user, theChannel->second) == false)
		return (send_msg(user->getClientSocket(), ERR_NOTONCHANNEL(user->getNickname(), word)));
	//else if (theChannel->second->getChannelInvitation() == false && use->getNickname() != "BotFeur")
	//	return ;
	else if (checkIsOperator(user, theChannel->second) == false)
		return (send_msg(user->getClientSocket(), ERR_CHANOPRIVSNEEDED(user->getNickname(), word)));
	else if (AlreadyInChannel(use, theChannel->second) == true)
		return (send_msg(user->getClientSocket(), ERR_USERONCHANNEL(user->getNickname(), use->getNickname() ,word)));
	
	theChannel->second->AddInvitedList(use);
	send_msg(use->getClientSocket(), INVITE(user->getNickname(), use->getNickname() , theChannel->first));
	send_msg(user->getClientSocket(), RPL_INVITING(user->getNickname(), use->getNickname(), theChannel->second->getChannelName()));
}