#include "../Server/Server.hpp"

bool Server::AlreadyInChannel(User *user, Channel *channel)
{
	std::vector<User *> list = channel->getChannelAuthorized();
	std::vector<User *>::iterator user_list = list.begin();

	std::cout << "autorized = [" << list.size() << "]" << std::endl;

	while (user_list != list.end())
	{
		User *use = CpyUser(*user_list);
		std::cout << use->getNickname() << " == " << user->getNickname() << std::endl;
		if (use->getNickname() == user->getNickname())
			return (true);
		delete use;
		++user_list;
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
		std::cout << it->second->getChannelName() << " == " << chan << std::endl;
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
		if (use->getClientSocket() == user->getClientSocket())
			return (true);
		delete use;
		++operator_list;
	}
	return (false);
}