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

class ClientNetwork {
private:
	char buffer[2048]; // receiving buffer
	std::string serverIP = "localhost"; // Server IP address
	int port = 8001; // sending port
	sf::UdpSocket socket;
	sf::UdpSocket socket_receiving;
	int id = 1; // player id
	std::string name = "UnknownPlayer"; // player name
	std::string team_name_hint = "any"; // hint to the server (may be rejected)
	int token = 0;

public:
	ClientNetwork();
	ClientNetwork(std::string serverIP_, int port_, int id_, std::string name_, int token_);

	void set_port(int port_ = 8001); // setting a new sending port
	void set_server(std::string serverIP_ = "localhost"); // setting a new server IP address
	void set_id(int id_ = 1); // setting a new id for player
	void set_name(std::string name_ = "UnknownPlayer"); // setting a new name for player
	void set_team_name_hint(std::string name_ = "any"); // setting a new team name hint for player
	void set_token(int token_);

	int get_port(); // getting sending port
	std::string get_serverIP(); // getting server IP address
	int get_id(); // getting player id
	std::string get_name(); // getting player name
	std::string get_team_name_hint(); // getting player team name hint
	int get_token(); // getting player token

	// Sends data in format: "PlayerId LocalTime PlayerName TeamNameHint # data"
	void send(std::string data);
	// Retruns received string
	void receive(); 
	// Returns basic string converted to string
	std::string get_message();
};