#include <iostream>
#include <filesystem>
#include <fstream>
#include "message.h"

int main(int argc, char* argv[]) {
    try {
        if (argc == 1) {
            std::cerr << "Usage: decode-message <image>\n";
            return 1;
        }
        else if (argc != 2) {
            throw std::runtime_error("Invalud number of arguments. Expected 1, got " + std::to_string(argc - 1));
        }
        std::string message_image_filename = argv[1];
        if (!std::filesystem::exists(std::filesystem::path(message_image_filename))) {
            throw std::runtime_error("Image does not exist!");
        }
        std::string message = get_message(message_image_filename);
        
        std::cout << message << '\n';
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
