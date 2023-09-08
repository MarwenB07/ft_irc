#include "../Server/Server.hpp"

bool Server::AlreadyInChannel(User *user, Channel *channel)
{
	std::vector<User *> list = channel->getChannelAuthorized();

	for (std::vector<User *>::iterator user_list = list.begin(); user_list != list.end(); ++user_list)
	{
		User *use = CpyUser(*user_list);
		if (use->getNickname() == user->getNickname())
		{
			delete use;
			return (true);
		}
		delete use;
	}
	return (false);
}

// check if my channel already 

bool Server::ChannelAlreadyExists(std::string channel, std::map<std::string, Channel *> channel_list, int c)
{
	std::string chan = channel;
	if (c == 0)
		chan.erase(0, 1);
	for (std::map<std::string, Channel *>::iterator it = channel_list.begin(); it != channel_list.end(); ++it)
	{
		if (it->second->getChannelName() == chan)
			return (true);
	}
	return (false);	
}

bool Server::checkNameOfChannel(std::string channel)
{
	if (channel.find(' ') != std::string::npos || channel.length() > 200)
		return (false);
	return (true);
}

bool Server::checkIsInvited(User *user, Channel *channel)
{
	std::vector<User *> list = channel->getInvitedList();
	std::vector<User *>::iterator invited_list = list.begin();

	while (invited_list != list.end())
	{
		User *use = CpyUser(*invited_list);
		if (use->getClientSocket() == user->getClientSocket())
			return (true);
		delete use;
		++invited_list;
	}
	return (false);
}

bool Server::checkIsOperator(User *user, Channel *channel)
{
	std::vector<User *> list = channel->getChannelOperator();
	std::vector<User *>::iterator operator_list = list.begin();

	while (operator_list != list.end())
	{
		User *use = CpyUser(*operator_list);
		if (use->getClientSocket() == user->getClientSocket() || user->getNickname() == channel->getChannelCreator())
			return (true);
		delete use;
		++operator_list;
	}
	return (false);
}

bool Server::checkUserExist(std::string nickname)
{
	for (std::vector<std::string>::iterator it = _users_nick_list.begin(); it != _users_nick_list.end(); ++it)
	{
		if (nickname == *it)
			return (true);
	}
	return (false);
}

bool Server::checkIsCreator(User *user, Channel *channel)
{
	if (user->getNickname() == channel->getChannelCreator())
		return (true);
	return (false);
}