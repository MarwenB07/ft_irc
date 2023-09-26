#ifndef IRC_MSG_HPP
#define IRC_MSG_HPP

/* 

Traduction :

a_u_m = <available user modes>
a_c_m = <available channel modes>
c_m_w_a_p = <channel modes with a parameter>
l_o_t_m = line of the motd

*/

/* all Irc Macro code  */

	/* Normal Macro in Irc */

// Join the server //


# define RPL_WELCOME(client, servername, user, host) (":localhost 001 " + client + " :Welcome to the " + servername + " Network, " + client + "!" + user + "@" + host + "\r\n") // 001
# define RPL_YOURHOST(client, servername, version) (":localhost 002 " + client + " :Your host is " + servername + ", running version " + version + "\r\n") // 002
# define RPL_CREATED(client, datetime) (":localhost 003 " + client + " :This server was created " + datetime + "\r\n") // 003
# define RPL_MYINFO(client, servername, version, a_u_m, a_c_m, c_m_w_a_p) (":localhost 004 " + client + " " + servername + " " + version + " " + a_u_m + " " + a_c_m + " " + c_m_w_a_p + "\r\n") // 004
# define RPL_ISUPPORT(client, token) (":localhost 005 " + client + " " + token + " :are supported by this server\r\n") // 005

# define RPL_LUSERCLIENT(client, entier) (":localhost 251 " + client + " :There are " + entier + " users and 0 invisible on 1 servers\r\n") // 251
# define RPL_LUSEROP(client) (":localhost 252 " + client + " 0 :operator(s) online\r\n") // 252
# define RPL_LUSERCHANNELS(client, entier) (":localhost 254 " + client + " :channels formed\r\n") // 254
# define RPL_LUSERME(client, entier) (":localhost 255 " + client + " :I have " + entier + " clients and 1 servers\r\n") // 255
# define RPL_LOCALUSERS(client, u, m) (":localhost 265 " + client + " " + u + " " + m + ":Current local users " + u + " max " + m + "\r\n") // 265
# define RPL_GLOBALUSERS(client, u, m) (":localhost 266 " + client + " " + u + " " + m + ":Current global users " + u + " max " + m + "\r\n") // 266

# define RPL_MOTD(client, l_o_t_m) ( ":localhost 372 " + client + " :" + l_o_t_m + "\r\n") // 372
# define RPL_ENDOFMOTD(client) ( ":localhost 376 " + client + " :End of /MOTD command.\r\n") // 376



# define RPL_CHANNELMODEIS(client, channel, modestring, mode_arguments) (":localhost 324 " + client + " #" + channel + " " + modestring + mode_arguments + "...\r\n") // 324
# define RPL_CREATIONTIME(client, channel, creationtime) (":localhost 329 " + client + " #" + channel + " " + creationtime + "\r\n") // 329
# define RPL_NOTOPIC(client, channel) (":localhost 331 " + client + " #" + channel + " :No topic is set\r\n") // 331
# define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " #" + channel + " :" + topic + "\r\n") // 332
# define RPL_TOPICWHOTIME(client, channel, nick, setat) (":localhost 333 " + client + " #" + channel + " " + nick + " " + setat + "\r\n") // 333
# define RPL_INVITELIST(client, channel) (":localhost 336 " + client + " " + channel + "\r\n") // 336
# define RPL_ENDOFINVITELIST(client) (":localhost 337 " + client + " :End of /INVITE list\r\n") // 337
# define RPL_INVITING(client, nick, channel) (":localhost 341 " + client + " " + nick + " #" + channel + "\r\n") // 341
# define RPL_NAMREPLY(client, symbol, channel, user) (":localhost 353 " + client + " " + symbol + " #" + channel + " :" + user + " \r\n") // 353

# define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + client + " #" + channel + " :End of /NAMES list\r\n") // 366
# define ERR_NOSUCHNICK(client, nickname) (":localhost 401 " + client + " " + nickname + " :No such nick/channel\r\n") // 401
# define ERR_NOSUCHCHANNEL(client, channel) (":localhost 403 " + client + " " + channel + " :No such channel\r\n") // 403
# define ERR_NOTEXTTOSEND(client) (":localhost 412 " + client +  " :No text to send\r\n")
# define ERR_NONICKNAMEGIVEN(client) (":localhost 431 " + client + " :No nickname given\r\n") // 431
# define ERR_ERRONEUSNICKNAME(client, nick) (":localhost 432 " + client + " " + nick + " :Erroneus nickname\r\n") // 432
# define ERR_NICKNAMEINUSE(client, nick) (":localhost 433 " + client + " " + nick + " :Nickname is already in use\r\n") // 433
# define ERR_USERNOTINCHANNEL(client, nick, channel) (":localhost 441 " + client + " " + nick + " #" + channel + " :They aren't on that channel\r\n") // 441
# define ERR_NOTONCHANNEL(client, channel) (":localhost 442 " + client + " " + channel + " :You're not on that channel\r\n") // 442
# define ERR_USERONCHANNEL(client, nick, channel) (":localhost 442 " + client + " " + nick + " " + channel + " :is already on channel\r\n") // 443
# define ERR_NEEDMOREPARAMS(client, commande) (":localhost 461 " + client + " " + commande + " :Not enough parameters\r\n") // 461
# define ERR_ALREADYREGISTERED(client) (":localhost 462 " + client + " :You may not reregister\r\n") // 462
# define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect\r\n") // 464
# define ERR_CHANNELISFULL(client, channel) (":localhost 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n") // 471
# define ERR_UNKNOWNMODE(client, modechar) (":localhost 472 " + client + " " + modechar + " :is unknown mode char to me\r\n") // 472
# define ERR_INVITEONLYCHAN(client, channel) (":localhost 473 " + client + " #" + channel + " :Cannot join channel (+i)\r\n") // 473
# define ERR_BADCHANNELKEY(client, channel) (":localhost 475 " + client + " #" + channel + " :Cannot join channel (+k)\r\n") // 475
# define ERR_CHANOPRIVSNEEDED(client, channel) (":localhost 482 " + client + " " + channel + " :You're not channel operator\r\n") // 482

					/* Error Macro in Irc */

// 448
					/* Other default message */

# define NB_CLIENTS 100

# define KICKED(sender, channel, nick, reason) (":" + sender + " KICK #" + channel + " " + nick + " :" + reason + "\r\n")
# define PART(nick, channel, reason) (":" + nick + " PART #" + channel + " :" + reason + "\r\n")
# define INVITE(client, nick, channel) (":" + client + " INVITE " + nick + " #" + channel + "\r\n")
# define MODE(client, channel, mode) (":" + client + " MODE #" + channel + " " + mode + "\r\n")
# define JOIN(client, channel) (":" + client + " JOIN #" + channel + "\r\n")
# define PONG(server, token) (server + token) // a voir + tard
# define QUIT(client, reason) (":" + client + " QUIT :" + reason + "\r\n") // a voir + tard
# define PRIVMSG(nick, ClientUser, message) (":" + nick + " PRIVMSG " + ClientUser + " " + message + "\r\n")
# define HELPER "\nBot : BotFeur\r\n"
# define HELP_MESSAGE "\nIn order :\n\n\
PASS <password>\n\
NICK <nickname>\n\
USER <username> 0 * :<realname> Exemple : USER guest 0 * :Bart Simpson\n\r\n"

/* other */

# define IRC_WELCOME "--- [ Welcome to IRC Serve ] ---\n\r\n"
# define START	  "\033[1;35m[IRC SERVER]\033[0;0m Mbouaza & Mdoumi : Waiting connexion ..."
# define WELCOME  "Welcome to ft_irc server\r\n"

// Colors //

# define GREY   "\033[1;30m"
# define RED    "\033[1;31m"
# define GREEN  "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE   "\033[1;36m"
# define PURPLE "\033[1;35m"
# define WHITE  "\033[1;37m"
# define END	"\033[0;0m"

// list

/*

connect - 

privmsg user to user -

privmsg channel -

TOPIC -

KICK -

*/

// function //

std::string takeMessage(std::string message);

void send_msg(int socket, std::string message);

#endif