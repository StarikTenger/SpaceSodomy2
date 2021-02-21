#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <fstream>
#include <cstdio>
#include <windows.h>
#include <vector>
#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>
#include <set>
#include <map>
#include <string>
#include <sys/timeb.h>
#include <deque>


class Server_Network
{
private:
	sf::UdpSocket socket;
	char buffer[1024]; // receiving buffer
	std::size_t received = 0; // receiving message size
	sf::IpAddress sender; // player IP address
	unsigned short port = 0; // receiving message port
	std::set<sf::IpAddress> addresses; // all players IP addresses
	std::map<sf::IpAddress, int> ports; // getting port by player IP address
	std::map<std::string, sf::IpAddress> IPconvert; // getting IP address by it's string form
	std::deque<std::string> messages; // deque of received messages
public:
	Server_Network();
	Server_Network(int port_);

	std::deque<std::string> get_messages(); // return deque of received messages
	std::string get_last_message(); // return message on the top of deque (last message)
	void del_last_message(); // delete message on the top of deque (last message)
	void clear_messages(); // delete all messages from deque
	void del_address(std::string address_); // delete address from set

	void receive(); // push new message to the top of deque
	void send(std::string message); // sending message to all players
};