#include "../Server/Server.hpp"

void Server::CreateChannel(User *user, std::string name)
{
	std::string list = "";
	if (checkNameOfChannel(name) == false)
		send_msg(user->getClientSocket(), CREATE_ERROR);
	else
	{
		name.erase(0, 1);
		send_msg(user->getClientSocket(), JOIN_CHANNEL(user->getNickname(), name));
		_channel.insert(std::make_pair(name, new Channel(name, user)));
		std::map<std::string, Channel *>::iterator chan = _channel.find(name);
		chan->second->AddChannelAuthorized(user);
		chan->second->AddChannelOperator(user);
		_channel_class_list.push_back(chan->second);
		list = createListOfMember(chan->second->getChannelAuthorized(), chan->second);
		sends_msg(user->getClientSocket(), JOIN(user->getNickname(), chan->second->getChannelName()), chan->second->getChannelAuthorized(), 0);
		sends_msg(user->getClientSocket(), RPL_NAMREPLY(user->getNickname(), "=", chan->second->getChannelName() ,list), chan->second->getChannelAuthorized(), 0); 
		sends_msg(user->getClientSocket(), RPL_ENDOFNAMES(user->getNickname(), chan->second->getChannelName()), chan->second->getChannelAuthorized(), 0);
	}
}

void Server::JoinZero(User *user)
{
	std::string temp = "PART #";
	std::string re = "";
	for (std::vector<Channel *>::iterator it = _channel_class_list.begin(); it != _channel_class_list.end(); ++it)
	{
		if (AlreadyInChannel(user, (*it)) == true)
		{
			temp.append((*it)->getChannelName());
			Part(user, _channel, temp);
			(*it)->PartChannel(user);
			// newChan->second->PartChannel(user);
			// message mise a jour list of name
			send_msg(user->getClientSocket(), PART(user->getNickname(), (*it)->getChannelName(), re));
			temp = "PART #";
			re = "";
		}
	}
	return ;
}

void Server::JoinChannel(int socket, std::string nickname, std::string name, std::map<std::string, Channel *> channel, std::string pass)
{
	std::string list;
	std::map<std::string, Channel *>::iterator canal = channel.find(name);
	std::map<int, User *>::iterator user = _users.find(socket);
	if (canal->second->getChannelInvitation() == true)
		if (checkIsInvited(user->second, canal->second) == false)
			return (send_msg(socket, ERR_INVITEONLYCHAN(user->second->getNickname(), canal->first)));
	if (canal->second->getChannelActifPass() == true && pass != canal->second->getChannelPass())
		return (send_msg(socket, ERR_BADCHANNELKEY(user->second->getNickname(), canal->first)));
	if (AlreadyInChannel(user->second, canal->second) == true)
		return (send_msg(socket, ALREADY_IN_CHANNEL(name)));
	else if (canal->second->getChannelHaveClientsLimits() == true && canal->second->getChannelClientsLimits() <= canal->second->getChannelSizeofClients())
		return (send_msg(socket, ERR_CHANNELISFULL(user->second->getNickname(), name)));
	else
	{
		canal->second->AddToChannel(user->second, nickname);
		canal->second->AddChannelAuthorized(user->second);
		if (checkIsInvited(user->second, canal->second) == true)
			canal->second->DeleteInvited(user->second);
		list = createListOfMember(canal->second->getChannelAuthorized(), canal->second);
		sends_msg(socket, JOIN(user->second->getNickname(), canal->second->getChannelName()), canal->second->getChannelAuthorized(), 0);
		sends_msg(socket, RPL_NAMREPLY(user->second->getNickname(), "=", canal->second->getChannelName() ,list), canal->second->getChannelAuthorized(), 0);
		return (sends_msg(socket, RPL_ENDOFNAMES(user->second->getNickname(), canal->second->getChannelName()), canal->second->getChannelAuthorized(), 0));
	}
	return ;
}

void Server::Join(int socket, std::vector<std::string> split, std::map<std::string, Channel *> channel)
{
	std::string word;
	std::string noPass = "#";
	std::vector<std::string> channelname;
	std::vector<std::string> pass;
	std::vector<std::string>::iterator w = split.begin();
	std::map<int, User *>::iterator user = _users.find(socket);


	++w;
	word = *w;
	word = correctChar(word, ',');
	channelname = newSplit(word, ",");
	if (split.size() == 2 && word == "0")
		JoinZero(user->second);
	else if (split.size() == 2)
	{
		for (std::vector<std::string>::iterator it = channelname.begin(); it != channelname.end(); ++it)
		{
			word = *it;
			word.erase(0, 1);
			if (ChannelAlreadyExists(word, _channel, 1) == true)
				JoinChannel(socket, user->second->getNickname(), word, channel, "");
			else if ((*it).find("#") > 0)
				send_msg(socket, ERR_NOSUCHCHANNEL(user->second->getNickname(), *it));
			else
				CreateChannel(user->second, *it);
		}
	}
	else if (split.size() == 3)
	{
		++w;
		word = *w;
		word = correctChar(word, ',');
		pass = newSplit(word, ",");
		std::vector<std::string>::iterator passList = pass.begin();
 
		for (std::vector<std::string>::iterator it = channelname.begin(); it != channelname.end(); ++it)
		{
			if (ChannelAlreadyExists(*it, channel, 0) == true)
			{
				word = *it;
				word.erase(0, 1);
				if (checkChannelHaveActivePass(word, channel) == true && passList != pass.end())
				{
					JoinChannel(socket, user->second->getNickname(), word, channel, *passList);
					++passList;
				}
				else
				{
					if ((*passList).find("#") > 0)
						send_msg(socket, ERR_NOSUCHCHANNEL(user->second->getNickname(), *passList));
					else
						JoinChannel(socket, user->second->getNickname(), word, channel, "");
				}
			}
		}
		return ;
	}
}