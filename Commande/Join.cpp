#include "../Server/Server.hpp"

/*

JOIN #foobar ; accède au canal #foobar.

JOIN &foo fubar ; accède au canal &foo en utilisant la clé "fubar".

JOIN #foo,&bar fubar ; accède au canal #foo en utilisant la clé "fubar", et &bar en n'utilisant pas de clé.

JOIN #foo,#bar fubar,foobar ; accède au canal #foo en utilisant la clé "fubar", et au canal #bar en utilisant la clé "foobar".

JOIN #foo,#bar ; accède au canaux #foo and #bar.

:WiZ JOIN #Twilight_zone ; message JOIN de WiZ *fait*

*/

bool Server::AlreadyInChannel(int socket, std::string channelname)
{
	int dst_socket;
	std::map<std::string, Channel>::iterator channel = _channel.find(channelname);
	std::vector<int> canal = channel->second.getChannelAuthorized();
	for (std::vector<int>::iterator it = canal.begin(); it != canal.end(); ++it)
	{
		dst_socket = *it;
		if (dst_socket == socket)
			return (true);
	}
	return (false);
}

// check if my channel already 

bool Server::ChannelAlreadyExists(std::string channel, std::map<std::string, Channel> channel_list)
{
	for (std::map<std::string, Channel>::iterator it = channel_list.begin(); it != channel_list.end(); ++it)
	{
		std::cout << it->second.getChannelName() << " == " << channel << std::endl;
		if (it->second.getChannelName() == channel)
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

bool Server::checkInvitation(int socket, std::string name, std::map<std::string, Channel> canal)
{
	int dst_socket = -1;
	std::map<std::string, Channel>::iterator channel = canal.find(name);
	std::vector<int> list_invited = channel->second.getInvitedList();

	for (std::vector<int>::iterator it = list_invited.begin(); it != list_invited.end(); ++it)
	{
		dst_socket = *it;
		if (socket == dst_socket)
			return (true);
	}
	return (false);
}

void Server::CreateChannel(int socket, std::string name)
{
	if (checkNameOfChannel(name) == false)
		send(socket, Print(CREATE_ERROR).c_str(), Print(CREATE_ERROR).length(), 0);
	else
	{
		name.erase(0, 1);
		_channel.insert(std::make_pair(name, Channel(name, socket)));
		send(socket, Print(CREATE_CHANNEL(name)).c_str(), Print(CREATE_CHANNEL(name)).length(), 0);
	}
}

void Server::JoinChannel(int socket, std::string nickname, std::string name, std::map<std::string, Channel> channel)
{
	std::map<std::string, Channel>::iterator canal = channel.find(name);
	if (canal->second.getChannelInvitation() == true)
	{
		if (checkInvitation(socket, name, channel) == false)
		{
			send(socket, Print(NOT_INVITED).c_str(), Print(NOT_INVITED).length(), 0);
			return ;
		}
	}
	if (AlreadyInChannel(socket, canal->first) == true)
		send(socket, Print(ALREADY_IN_CHANNEL(name)).c_str(), Print(ALREADY_IN_CHANNEL(name)).length(), 0);
	else
		canal->second.AddToChannel(socket, nickname);
}

void Server::Join(int socket, std::vector<std::string> split, std::map<std::string, Channel> channel)
{
	int i = 0;
	std::string word;
	std::vector<std::string> channelname;
	std::vector<std::string>::iterator w = split.begin();
	std::map<int, User>::iterator user = _users.find(socket);

	for (std::vector<std::string>::iterator count = split.begin(); count != split.end(); ++count)
	{
		i++;
	}
	++w;
	word = *w;
	channelname = newSplit(word, ",");

	if (i == 2)
	{
		for (std::vector<std::string>::iterator it = channelname.begin(); it != channelname.end(); ++it)
		{
			word = *it;
			word.erase(0, 1);
			if (ChannelAlreadyExists(word, _channel) == true)
				JoinChannel(socket, user->second.getNickname(), word, channel);
			else
				CreateChannel(socket, *it);
		}
	}
	else if (i == 3)
	{
		std::cout << "jejeje" << std::endl;
		return ;
	}
}