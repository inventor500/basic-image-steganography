#include "message.h"


std::vector<bool> encode_message(std::string message) {
    std::vector<bool> encoded_message; // vector<bool> is a dynamic bitset
    // Reserge message bits, plus five for ending flag
    encoded_message.reserve((message.size() * 8) + 8); // Due to bit stuffing, will be slightly under the total amount
    // insert bits from message
    for (char i : message) {
        CHAR_BITSET temp = i;
        for (int j = 7; j >= 0; j--) { // size_t is unsigned
            encoded_message.push_back(temp[j]); // Space was already allocated
        }
    }
    std::cout << "Beginning bit stuffing...\r";
    // Bit stuffing
    int temp_counter = 0;
    for (size_t i = 0; i < encoded_message.size(); i++) {
        if (encoded_message.at(i) == true) {
            temp_counter++;
        }
        else {
            temp_counter = 0;
        }
        // Insert a 0 to break up 5 1s in a row
        if (temp_counter == 8) {
            auto iter = encoded_message.begin() + i;
            encoded_message.insert(iter,false);
            temp_counter = 0;
        }
    }
    // Insert ending bits
    for (size_t i = 0; i < 8; i++) {
        encoded_message.push_back(true);
    }
    return encoded_message;
}


png::image<png::rgb_pixel> insert_message(std::string message, std::string filename) {
    png::image<png::rgb_pixel> image(filename);
    std::vector<bool> encoded_message = encode_message(message);
    if ((encoded_message.size() * 3) > (image.get_height() * image.get_width())) {
        throw std::runtime_error("Message is too long for image");
    }
    size_t counter = 0;
    std::cout << "Inserting message into image...\r";
    while (true) {
        for (size_t y = 0; y < image.get_height(); y++) {
            for (size_t x = 0; x < image.get_width(); x++) {
                auto pixel = image[y][x];
                pixel.red &= 0xE;
                pixel.red |= encoded_message[counter];
                counter++;
                if (counter >= encoded_message.size()) {
                    image[y][x] = pixel;
                    return image;
                }
                pixel.green &= 0xE;
                pixel.green |= encoded_message[counter];
                counter++;
                if (counter >= encoded_message.size()) {
                    image[y][x] = pixel;
                    return image;
                }
                pixel.blue &= 0xE;
                pixel.blue |= encoded_message[counter];
                counter++;
                if (counter >= encoded_message.size()) {
                    image[y][x] = pixel;
                    return image;
                }
                image[y][x] = pixel;
            }
        }
    }
}

std::string decode_message(std::vector<bool> encoded_message) {
    std::string decoded_message;
    // Verify that bit stuffing was used
    for (size_t i = 0; i < 8; i++) {
        if (encoded_message.at(encoded_message.size() - 1 - i) != true) {
            throw std::runtime_error("Invalid ending for encoded message");
        }
    }
    // Remove end flag
    auto iter = encoded_message.end() - 8;
    encoded_message.erase(iter,encoded_message.end());
    // Remove stuffed bits
    size_t true_counter = 0; // number of 1s in a row
    for (size_t i = 0; i < encoded_message.size(); i++) {
        if (encoded_message.at(i) == true) {
            true_counter++;
        }
        else if (true_counter == 7 && encoded_message.at(i) == false) {
            iter = encoded_message.begin() + i;
            encoded_message.erase(iter);
            i--;
            true_counter = 0;
        }
        else {
            true_counter = 0;
        }
    }
    // Convert to string
    if (encoded_message.size() % 8 != 0) {
        throw std::runtime_error("Invalid message length of " + std::to_string(encoded_message.size()) + " bits after removing stuffed bits");
    }

    for (size_t i = 0; i < encoded_message.size() / 8; i++) {
        char temp_char = 0;
        for (size_t j = 0; j < 8; j++) {
            temp_char <<= 1;
            temp_char |= encoded_message.at((i * 8) + j);
        }
        decoded_message += temp_char;
    }
    return decoded_message;
}

std::string get_message(std::string filename) {
    int counter = 0;
    png::image<png::rgb_pixel> image(filename);
    std::vector<bool> encoded_message;
    while (true) {
        for (size_t y = 0; y < image.get_height(); y++) {
            for (size_t x = 0; x < image.get_width(); x++) {
                auto pixel = image[y][x];
                bool temp;
                temp = pixel.red & 1;
                if (temp == true) {
                    counter++;
                }
                else {
                    counter = 0;
                }
                encoded_message.push_back(temp);
                if (counter == 8) {
                    return decode_message(encoded_message);
                }
                temp = pixel.green & 1;
                if (temp == true) {
                    counter++;
                }
                else {
                    counter = 0;
                }
                encoded_message.push_back(temp);
                if (counter == 8) {
                    return decode_message(encoded_message);
                }
                temp = pixel.blue & 1;
                if (temp == true) {
                    counter++;
                }
                else {
                    counter = 0;
                }
                encoded_message.push_back(temp);
                if (counter == 8) {
                    return decode_message(encoded_message);
                }
            }
        }
    }
}
