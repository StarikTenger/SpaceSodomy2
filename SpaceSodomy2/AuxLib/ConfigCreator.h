#pragma once
#include <string>

class ConfigCreator {
protected:
	void write_config(std::string filename, std::string source);
public:
	virtual void run() = 0;
};

