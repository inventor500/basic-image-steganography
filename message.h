#include <png++/png.hpp>
#include <vector>
#include <bitset>
#include <string>

#pragma once

typedef std::bitset<sizeof(char) * 8> CHAR_BITSET;

// Create a dynamic bitset containing the message with bit stuffing
// Ends with 5 1s
std::vector<bool> encode_message(std::string message);

// Insert a message into a png. Returns the png with the message. Calls encode_message
png::image<png::rgb_pixel> insert_message(std::string message, std::string filename);

std::string decode_message(std::vector<bool>);

std::string get_message(std::string filename);
