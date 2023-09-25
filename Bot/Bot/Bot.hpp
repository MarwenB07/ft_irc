#ifndef BOT_HPP
#define BOT_HPP

// include //

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <map>
#include <vector>
#include <thread>
#include <arpa/inet.h>
#include <sstream>

// Register //

# define PASS(password) ("PASS " + password + "\r\n")
# define NICK "NICK BotFeur\r\n"
# define USER "USER LeBotDeOuf 0 * :tunsinger raciste\r\n"

// Send cmd //

# define BOT_JOIN(channel) ("JOIN " + channel + "\r\n")
# define PRIVMSG_BOT(sended, message) ("PRIVMSG " + sended + " :" + message + "\r\n")

// Phrase //

# define PHRASE_1 "Quelqu'un a pas trouver mes lunette ?"
# define PHRASE_2 "C'est qui la qui pue la ?"
# define PHRASE_3 "Cavaleta a un 9 millimetre, il est fou il vas peut etre tirer ... *BAN !!!* ... Merde il a tirer ..."
# define PHRASE_4 "Tu vas faire quoi avec tes banques ?"
# define PHRASE_5 "Faut lire Berserk si jamais ..."
# define PHRASE_6 "Funfact 100 pourcent des gens qui ne respire pas pendant plus de 20 min meurt ... pas de probleme bg."
# define PHRASE_7 "Femme a lunette, femme a kek**et."

// Troll //

# define FEUR_BASE "Feur ..."
# define FEUR_P2 "surement feur ..."
# define FEUR_P3 "je pense feur surement ..."

# define RAGE_1 "Tes serieux de me kick sale ****"
# define RAGE_2 "De toute facon ton channel il pue ya rien de bien. sale ****"
# define RAGE_3 "Bahahahahahahaha cheh !"
# define RAGE_4 "Mince chui desoler de t'avoir deranger sale ****"
# define RAGE_5 "Alev Alev le channel"

// Help //

void clearBuffer(char *buffer, int len);
void send_msg(int socket, std::string message);
std::vector<std::string> newSplit(std::string s, std::string c);
std::vector<std::string> s_split(std::string str);
std::string takeMessage(std::string message);

// Commande //

void ReturnReponse(int sockfd, std::string line);
void InviteBot(int sockfd, std::string line);
void IfKickedBot(int sockfd, std::string line);

// TreatingMessage //

void TreatMessage(int sockfd, std::string line);

#endif