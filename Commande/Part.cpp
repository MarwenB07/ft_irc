#include "../Server/Server.hpp"

void Server::PartOfChannel(User *user, std::map<std::string, Channel *> channel, std::string the_chan)
{
	std::cout << "PART used = " << _channel_class_list.size() << std::endl;

	for (std::vector<Channel *>::iterator it = _channel_class_list.begin(); it != _channel_class_list.end(); ++it)
	{
		if (ChannelAlreadyExists(the_chan, channel, 0) == true && AlreadyInChannel(user, (*it)) == true)
		{
			std::map<std::string, Channel *>::iterator chanPart = channel.find(the_chan);
			chanPart->second->PartChannel(user);
			send_msg(user->getClientSocket(), PART(user->getNickname(), chanPart->first));
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

	++w;
	word = *w;
	channelname = newSplit(word, ",");

	std::cout << "size[" << split.size() << "]" << std::endl;

	std::cout << "line = " << line << std::endl;

	if (line == "PART")
		return (send_msg(user->getClientSocket(), ERR_NEEDMOREPARAMS(user->getNickname(), line)));
	else if (split.size() == 3)
	{
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
				delete chan;
				continue;
			}
			PartOfChannel(user, channel, *it);
			delete chan;
		}
	}
	return ;
}