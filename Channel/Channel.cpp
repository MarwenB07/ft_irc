#include "Channel.hpp"

Channel::Channel(std::string ChannelName, int socket) : _name(ChannelName)
{
	_actif_password = false;
	_invitation = false;
	_topic = "";
	_sizeofClient = 1;
	_clients_limits = 20;
	_operator_list.push_back(socket);
	_authorized.push_back(socket);
}

// Add user into channel //

void Channel::AddToChannel(int socket, std::string name)
{
	int n_socket;
	std::string welcome_to_channel = name;
	welcome_to_channel.append(" JOIN #");
	welcome_to_channel.append(getChannelName());
	welcome_to_channel.append("\r\n");
	std::cout << "authorized" << std::endl;

	_authorized.push_back(socket);
	for (std::vector<int>::iterator it = _authorized.begin(); it != _authorized.end(); ++it)
	{
		n_socket = *it;
		if (socket != n_socket)
			send(n_socket, welcome_to_channel.c_str(), welcome_to_channel.length(), 0);
		else if (socket == n_socket)
		{
			send(socket, SUCCESS_JOIN(name).c_str(), SUCCESS_JOIN(name).length(), 0);
			send(socket, getChannelTopic().c_str(), getChannelTopic().length(), 0);
		}
	}

	for (std::vector<int>::iterator it = _authorized.begin(); it != _authorized.end(); ++it)
	{
		n_socket = *it;
		std::cout << "authorized == socket[" << n_socket << "]" << std::endl; 
	}
}

// geter //

std::string Channel::getChannelName(void)
{
	return(_name);
}

std::string Channel::getChannelPass(void)
{
	if (getChannelActifPass() == true)
		return(_password);
	return ("false");
}	

std::string Channel::getChannelTopic(void)
{
	return(_topic);
}

std::vector<int> Channel::getChannelAuthorized(void)
{
	return(_authorized);
}

std::vector<int> Channel::getChannelOperator(void)
{
	return(_operator_list);
}

std::vector<int> Channel::getInvitedList(void)
{
	return (_invited);
}

bool Channel::getChannelInvitation(void)
{
	return(_invitation);
}

bool Channel::getChannelActifPass(void)
{
	return(_actif_password);
}

int  Channel::getChannelClientsLimits(void)
{
	return(_clients_limits);
}

int  Channel::getChannelSizeofClients(void)
{
	return(_sizeofClient);
}

Channel::~Channel()
{

}