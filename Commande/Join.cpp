#include "../Server/Server.hpp"

/*

JOIN #foobar ; accède au canal #foobar.

JOIN &foo fubar ; accède au canal &foo en utilisant la clé "fubar".

JOIN #foo,&bar fubar ; accède au canal #foo en utilisant la clé "fubar", et &bar en n'utilisant pas de clé.

JOIN #foo,#bar fubar,foobar ; accède au canal #foo en utilisant la clé "fubar", et au canal #bar en utilisant la clé "foobar".

JOIN #foo,#bar ; accède au canaux #foo and #bar.

:WiZ JOIN #Twilight_zone ; message JOIN de WiZ *fait*

*/

void Server::CreateChannel(User *user, std::string name)
{
	if (checkNameOfChannel(name) == false)
		send_msg(user->getClientSocket(), CREATE_ERROR);
	else
	{
		name.erase(0, 1);
		send_msg(user->getClientSocket(), JOIN_CHANNEL(user->getNickname(), name));
		_channel.insert(std::make_pair(name, new Channel(name, user)));
	}
}

void Server::JoinChannel(int socket, std::string nickname, std::string name, std::map<std::string, Channel *> channel)
{
	std::map<std::string, Channel *>::iterator canal = channel.find(name);
	std::map<int, User *>::iterator user = _users.find(socket);
	if (canal->second->getChannelInvitation() == true)
	{
		if (checkIsInvited(user->second, canal->second) == false)
			return (send_msg(socket, NOT_INVITED));
	}
		//send_msg(socket, )
	if (AlreadyInChannel(user->second, canal->second) == true)
		send_msg(socket, ALREADY_IN_CHANNEL(name));
	else
	{
		canal->second->AddToChannel(user->second, nickname);
		canal->second->AddChannelAuthorized(user->second);
		if (checkIsInvited(user->second, canal->second) == true)
			canal->second->DeleteInvited(user->second);
	}
}

void Server::Join(int socket, std::vector<std::string> split, std::map<std::string, Channel *> channel)
{
	std::string word;
	std::vector<std::string> channelname;
	std::vector<std::string>::iterator w = split.begin();
	std::map<int, User *>::iterator user = _users.find(socket);

	++w;
	word = *w;
	channelname = newSplit(word, ",");
	if (split.size() == 2 && word == "0\r\n")
	{
		// PART avec tous les channel
	}
	else if (split.size() == 2)
	{
		for (std::vector<std::string>::iterator it = channelname.begin(); it != channelname.end(); ++it)
		{
			word = *it;
			word.erase(0, 1);
			if (ChannelAlreadyExists(word, _channel, 1) == true)
			{
				std::cout << "bahahaha sale con t dans le caca" << std::endl;
				JoinChannel(socket, user->second->getNickname(), word, channel);
			}
			else
			{
				std::cout << "force ..." << std::endl;
				CreateChannel(user->second, *it);
			}
		}
	}
	else if (split.size() == 3)
	{
		std::cout << "jejeje" << std::endl;
		return ;
	}
}