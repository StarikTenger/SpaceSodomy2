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
public:
	Server_Network();
	Server_Network(int port_);

	std::deque<std::string> get_messages();
	std::string get_last_message();
	void del_last_message();
	void clear_messages();
	std::set<sf::IpAddress>get_addresses();

	void receive();
	void send(std::string message);
private:
	sf::UdpSocket socket;
	char buffer[1024];
	std::size_t received = 0;
	sf::IpAddress sender;
	unsigned short port = 0;
	std::set<sf::IpAddress> addresses;
	std::map<sf::IpAddress, int> ports;
	std::deque<std::string> messages;
};