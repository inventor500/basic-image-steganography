#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "message.h"

int main(int argc, char* argv[]) {
    try {
        if (argc == 1) {
            std::cerr << "Usage: encode-message <message> <image>\n";
            return 1;
        }
        else if (argc != 3) {
            throw std::runtime_error("Invalid number of arguments. Expected 2, got " + std::to_string(argc - 1));
        }
        std::string image_filename = argv[2];
        std::string message_filename = argv[1];
        if (!std::filesystem::exists(std::filesystem::path(image_filename))) {
            throw std::runtime_error("Image does not exist!");
        }
        else if (!std::filesystem::exists(std::filesystem::path(message_filename))) {
            throw std::runtime_error("Message file does not exist!");
        }
        std::string message;
        {
            std::ifstream file(message_filename);
            std::ostringstream temp_stream;
            temp_stream << file.rdbuf();
            message = temp_stream.str();
        }
        png::image<png::rgb_pixel> image =  insert_message(message, image_filename);
        image.write(image_filename.substr(0, image_filename.length() - 4) + "_encoded.png");
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
