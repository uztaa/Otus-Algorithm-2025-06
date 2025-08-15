#include "FileReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

std::string FileReader::readFile(const std::string& filename) const {
    try {
        std::ifstream file(filename, std::ios::in);

        if (!file) {
            throw std::runtime_error("Cannot open file: " + filename + ". Check full path to in-, out-files.");
        }

        std::ostringstream content;
        content << file.rdbuf();  // read all data to string

        return content.str();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw e;
    }
}