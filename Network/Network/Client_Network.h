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
public:
	Client_Network();
	Client_Network(std::string serverIP_, int port_, int id_, std::string name_);

	void change_port(int port_ = 8001);
	void change_server(std::string serverIP_ = "localhost");
	void change_id(int id_ = 1);
	void change_name(std::string name_ = "UnknownPlayer");

	int get_port();
	std::string get_serverIP();
	int get_id();
	std::string get_name();

	void send(std::string data);
	std::string receive();
private:
	char buffer[2048];
	std::string serverIP = "localhost";
	int port = 8001;
	sf::UdpSocket socket;
	int id = 1;
	std::string name = "UnknownPlayer";
	int PrevTime = 0;

	int getMilliCount();
};