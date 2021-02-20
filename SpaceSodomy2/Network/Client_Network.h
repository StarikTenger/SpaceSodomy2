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
#include <string>
#include <sys/timeb.h>

class Client_Network
{
private:
	char buffer[2048]; // receiving buffer
	std::string serverIP = "localhost"; // Server IP address
	int port = 8001; // sending port
	sf::UdpSocket socket;
	int id = 1; // player id
	std::string name = "UnknownPlayer";

	int get_milli_count(); // local time (required to send it with messages)
public:
	Client_Network();
	Client_Network(std::string serverIP_, int port_, int id_, std::string name_);

	void set_port(int port_ = 8001); // setting a new sending port
	void set_server(std::string serverIP_ = "localhost"); // setting a new server IP address
	void set_id(int id_ = 1); // setting a new id for player
	void set_name(std::string name_ = "UnknownPlayer"); // setting a new name for player

	int get_port(); // getting sending port
	std::string get_serverIP(); // getting server IP address
	int get_id(); // getting player id
	std::string get_name(); // getting player name

	void send(std::string data); // seding data in format: "PlayerId LocalTime PlayerName #data"
	std::string receive(); // retrun received string
};