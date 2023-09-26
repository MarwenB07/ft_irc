#include "../Server/Server.hpp"

// probleme ne reconnais pas qu'il a quitter
// no reason
void Server::PartOfChannel(User *user, std::map<std::string, Channel *> channel, std::string the_chan, std::string reason)
{
	the_chan.erase(0, 1);
	for (std::vector<Channel *>::iterator it = _channel_class_list.begin(); it != _channel_class_list.end(); ++it)
	{
		if (ChannelAlreadyExists(the_chan, channel, 1) == true && AlreadyInChannel(user, (*it)) == true)
		{
			std::map<std::string, Channel *>::iterator chanPart = channel.find(the_chan);
			sends_msg(user->getClientSocket(), PART(user->getNickname(), chanPart->first, reason), chanPart->second->getChannelAuthorized(), 0);
			chanPart->second->PartChannel(user);
			chanPart->second->setChannelSizeofClients(chanPart->second->getChannelSizeofClients() - 1);
			if (chanPart->second->getChannelSizeofClients() == 0)
			{
				_channel_class_list.erase(std::remove(_channel_class_list.begin(), _channel_class_list.end(), chanPart->second), _channel_class_list.end());	
				eraseChanInMap(chanPart->first);
				return ;
			}
			return ;
		}
	}
	return ;
}

void Server::Part(User *user, std::map<std::string, Channel *> channel, std::string line)
{
	std::string word;
	std::vector<std::string> channelname;
	std::vector<std::string> split = s_split(line);
	std::vector<std::string>::iterator w = split.begin();
	std::map<std::string, Channel *>::iterator it;
	std::string reason = "";

	++w;
	word = *w;
	channelname = newSplit(word, ",");

	if (line == "PART")
		return (send_msg(user->getClientSocket(), ERR_NEEDMOREPARAMS(user->getNickname(), line)));
	else if ((split.size() == 3 && line.find(":") != std::string::npos) || split.size() == 2)
	{
		if (split.size() == 3)
		{
			reason = takeMessage(line + " ");
			reason.erase(0, 1);
		}
		for (std::vector<std::string>::iterator it = channelname.begin(); it != channelname.end(); ++it)
		{
			word = *it;
			word.erase(0, 1);
			if (ChannelAlreadyExists(word, channel, 1) == false)
			{
				send_msg(user->getClientSocket(), ERR_NOSUCHCHANNEL(user->getNickname(), *it));
				continue;
			}
			Channel *chan = FindChannel(word);
			if (AlreadyInChannel(user, chan) == false)
			{
				send_msg(user->getClientSocket(), ERR_NOTONCHANNEL(user->getNickname(), *it));
				continue;
			}
			PartOfChannel(user, channel, *it, reason);
		}
	}
	return ;
}