#include "../Server/Server.hpp"

/*

JOIN #foobar ; accède au canal #foobar.

JOIN &foo fubar ; accède au canal &foo en utilisant la clé "fubar".

JOIN #foo,&bar fubar ; accède au canal #foo en utilisant la clé "fubar", et &bar en n'utilisant pas de clé.

JOIN #foo,#bar fubar,foobar ; accède au canal #foo en utilisant la clé "fubar", et au canal #bar en utilisant la clé "foobar".

JOIN #foo,#bar ; accède au canaux #foo and #bar.

:WiZ JOIN #Twilight_zone ; message JOIN de WiZ *fait*

*/

// ouai si ya 2 , ca fait un vieu truc

void Server::CreateChannel(User *user, std::string name)
{
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
	}
}

void Server::JoinZero(User *user, std::map<std::string, Channel *> channel)
{
	std::string temp = "#";
	std::string re = "";
	for (std::vector<Channel *>::iterator it = _channel_class_list.begin(); it != _channel_class_list.end(); ++it)
	{
		if (AlreadyInChannel(user, (*it)) == true)
		{
			temp.append((*it)->getChannelName());
			std::map<std::string, Channel *>::iterator newChan = channel.find(temp);
			newChan->second->PartChannel(user);
			send_msg(user->getClientSocket(), PART(user->getNickname(), newChan->first, re));
			return (JoinZero(user, channel));
		}
	}
	return ;
}

void Server::JoinChannel(int socket, std::string nickname, std::string name, std::map<std::string, Channel *> channel, std::string pass)
{
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
		return (sends_msg(socket, JOIN(user->second->getNickname(), canal->second->getChannelName()), canal->second->getChannelAuthorized(), 0));
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
	std::cout << "line w = " << word << std::endl;
	channelname = newSplit(word, ",");
	if (split.size() == 2 && word == "0\r\n")
		JoinZero(user->second, channel);
	else if (split.size() == 2)
	{
		for (std::vector<std::string>::iterator it = channelname.begin(); it != channelname.end(); ++it)
		{
			word = *it;
			word.erase(0, 1);
			if (ChannelAlreadyExists(word, _channel, 1) == true)
				JoinChannel(socket, user->second->getNickname(), word, channel, "");
			else if ((*it).find("#") > 2)
				continue;
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
		std::cout << "pass[" << pass.size() << "]" << std::endl;

		for (std::vector<std::string>::iterator it = channelname.begin(); it != channelname.end(); ++it)
		{
			if (ChannelAlreadyExists(*it, channel, 0) == true)
			{
				word = *it;
				word.erase(0, 1);
				if (checkChannelHaveActivePass(word, channel) == true && passList != pass.end())
				{
					std::cout << *passList << std::endl;
					JoinChannel(socket, user->second->getNickname(), word, channel, *passList);
					++passList;
					std::cout << "hop" << std::endl;
				}
				else
					JoinChannel(socket, user->second->getNickname(), word, channel, "");
			}
		}
		return ;
	}
}