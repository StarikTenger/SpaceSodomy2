#include "pch.h"
#include "ConfigCreator.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>

void ConfigCreator::write_config(std::string filename, std::string source) {
	if (std::filesystem::exists(filename))
		return;
	std::cout << "Creating file " << filename << "\n";
	std::ofstream config_file;
	config_file.open(filename);
	config_file << source;
}