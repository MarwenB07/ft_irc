#include "../Server/Server.hpp"

bool Server::AlreadyInChannel(User *user, Channel *channel)
{
	std::vector<User *> list = channel->getChannelAuthorized();

	for (std::vector<User *>::iterator user_list = list.begin(); user_list != list.end(); ++user_list)
	{
		if ((*user_list)->getNickname() == user->getNickname())
			return (true);
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
	std::vector<User *>::iterator invited_list;

	for (invited_list = list.begin(); invited_list != list.end(); ++invited_list)
		if ((*invited_list)->getClientSocket() == user->getClientSocket())
			return (true);
	return (false);
}

bool Server::checkIsOperator(User *user, Channel *channel)
{
	std::vector<User *> list = channel->getChannelOperator();
	std::vector<User *>::iterator operator_list;

	std::cout << list.size() << std::endl;
	for (operator_list = list.begin(); operator_list != list.end(); ++operator_list)
	{
		if ((*operator_list)->getClientSocket() == user->getClientSocket() || user->getNickname() == channel->getChannelCreator())
			return (true);
	}
	std::cout << "false" << std::endl;
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

bool Server::checkChannelHaveActivePass(std::string name, std::map<std::string, Channel *> channel_list)
{
	std::cout << name << std::endl;
	std::map<std::string, Channel *>::iterator channel = channel_list.find(name);
	return (channel->second->getChannelActifPass());
}