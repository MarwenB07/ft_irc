#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(std::string ChannelName, User *user) : _name(ChannelName)
{
	_channel_creator = user->getNickname();
	_actif_password = false;
	_invitation = false;
	_topic = "\r\n";
	_channel_mode_list = "\r\n";
	_sizeofClient = 1;
	_clients_limits = 20;
	_operator_list.push_back(user);
	_authorized.push_back(user);
}

Channel::Channel( Channel const & src )
{
	*this = src;

	return ;
}

Channel& Channel::operator=(const Channel& rhs)
{
    if (this == &rhs) {
        return *this;
    }

    // Copie des membres simples
    _name = rhs.getChannelName();
    _password = rhs.getChannelPass();
    _topic = rhs.getChannelTopic();
	_authorized = rhs.getChannelAuthorized();
    _invitation = rhs.getChannelInvitation();
	_operator_list = rhs.getChannelOperator();
    _actif_password = rhs.getChannelActifPass();
    _clients_limits = rhs.getChannelClientsLimits();
    _sizeofClient = rhs.getChannelSizeofClients();

    return *this;
}
// Add user into channel //

void Channel::AddToChannel(User *user, std::string name)
{
	// check client limit //
	// supp les invited //

	for (std::vector<User *>::iterator it = _authorized.begin(); it != _authorized.end(); ++it)
	{
		User *users = CpyUser(*it);
		if (user->getClientSocket() != users->getClientSocket())
			send_msg(users->getClientSocket(), JOIN_CHANNEL(user->getNickname(), name));
		delete users;
	}
	send_msg(user->getClientSocket(), JOIN_CHANNEL(user->getNickname(), _name));
	SendTopic(user);
	setChannelSizeofClients(getChannelSizeofClients() + 1);
}

void Channel::SendMsgToChannel(std::string message, int socket)
{
	for (std::vector<User *>::iterator socke = _authorized.begin(); socke != _authorized.end(); ++socke)
	{
		User *use = CpyUser(*socke);
		if (socket != use->getClientSocket())
			send_msg(use->getClientSocket(), message);
		delete use;
	}
}

void Channel::SendTopic(User *user)
{
	if (getChannelTopic() == "\r\n")
	{
		std::cout << "topic" << std::endl;
		send_msg(user->getClientSocket(), RPL_NOTOPIC(user->getNickname(), getChannelName()));
	}
	else
		send_msg(user->getClientSocket(), RPL_TOPIC(user->getNickname(), getChannelName(), getChannelTopic()));
}

void Channel::KickUser(User *user, std::string reason, int c)
{
	// reason = 1
	// just kick == 0 and don't care of reason string
	if (VerifVector(_operator_list, user) == true)
		_operator_list.erase(std::remove(_operator_list.begin(), _operator_list.end(), user), __operator_list.end());
	_authorized.erase(std::remove(_authorized.begin(), _authorized.end(), user), _authorized.end());
	if (c == 0)
		send_msg(user->getClientSocket(), KICKED(getChannelName()));
	else
		send_msg(user->getClientSocket(), KICKED_MESSAGE(getChannelName(), reason));

}

bool Channel::VerifVector(std::vector<User *> all_users, User *user)
{
	for (std::vector<User *>::iterator it = all_users.begin(); it != all_users.end(); ++it)
	{
		User *use = CpyUser(*it);
		if (user->getNickname() == use->getNickname())
		{
			delete use;
			return (true);
		}
		delete use;
	}
	return (false)
}

// geter //

std::string Channel::getChannelName(void) const
{
	return(_name);
}

std::string Channel::getChannelPass(void) const
{
	if (getChannelActifPass() == true)
		return(_password);
	return ("false");
}	

std::string Channel::getChannelTopic(void) const
{
	return(_topic);
}

std::string Channel::getChannelMode(void) const
{
	return(_channel_mode_list);
}

std::string Channel::getChannelCreator(void) const
{
	return (_channel_creator);
}

std::vector<User *> Channel::getChannelAuthorized(void) const
{
	return(_authorized);
}

std::vector<User *> Channel::getChannelOperator(void) const
{
	return(_operator_list);
}

std::vector<User *> Channel::getInvitedList(void) const
{
	return (_invited);
}

bool Channel::getChannelInvitation(void) const
{
	return(_invitation);
}

bool Channel::getChannelActifPass(void) const
{
	return(_actif_password);
}

int  Channel::getChannelClientsLimits(void) const
{
	return(_clients_limits);
}

int  Channel::getChannelSizeofClients(void) const
{
	return(_sizeofClient);
}

// set //

void Channel::setChannelName(std::string set)
{
	this->_name = set;
}

void Channel::setChannelPass(std::string set)
{
	this->_password = set;
}

void Channel::setChannelTopic(std::string set)
{
	this->_topic = set;
}

void  Channel::setChannelClientsLimits(int set)
{
	this->_clients_limits = set;
}

void  Channel::setChannelSizeofClients(int set)
{
	this->_sizeofClient = set;
}

void Channel::setChannelInvitation(bool set)
{
	this->_invitation = set;
}

void Channel::setChannelActifPass(bool set)
{
	this->_actif_password = set;
}

void Channel::AddChannelAuthorized(User *user)
{
	std::cout << "added" << std::endl;
	this->_authorized.push_back(user);
}

void Channel::AddChannelOperator(User *user)
{
	this->_operator_list.push_back(user);
}

void Channel::AddInvitedList(User *user)
{
	this->_invited.push_back(user);
}

void Channel::setChannelMode(std::string set)
{
	// check if mode is always here //
	_channel_mode_list.append(set);
}

Channel::~Channel()
{

}