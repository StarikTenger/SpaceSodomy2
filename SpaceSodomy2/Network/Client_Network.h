#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <SFML/Network.hpp>
#include <fstream>
#include <cstdio>
#include <windows.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sys/timeb.h>
#include <AuxLib/AuxLib.h>

class Client_Network {
private:
	char buffer[2048]; // receiving buffer
	std::string serverIP = "localhost"; // Server IP address
	int port = 8001; // sending port
	sf::UdpSocket socket;
	int id = 1; // player id
	std::string name = "UnknownPlayer";

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

	// Sends data in format: "PlayerId LocalTime PlayerName # data"
	void send(std::string data);
	// Retruns received string
	void receive(); 
	// Returns basic string converted to string
	std::string get_message();
};