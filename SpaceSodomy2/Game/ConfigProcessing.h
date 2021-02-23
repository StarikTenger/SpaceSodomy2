#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

namespace Config_Processing {
	std::stringstream comment(std::ifstream& file) {
		std::stringstream newFile; // file without comments
		std::string elem; // current input

		while (file >> elem) {
			if (elem == "#") { // Comment
				std::string input = "";
				std::getline(file, input);
				//while (input != "#")
				//	file >> input;
			}
			else {
				newFile << elem << " ";
			}
		}

		return newFile;
	}
}