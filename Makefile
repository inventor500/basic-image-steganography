main: encode-message decode-message
encode-message: message.o encode-message.cpp
	g++ -Wall -o encode-message message.o encode-message.cpp `libpng-config --ldflags`
decode-message: message.o decode-message.cpp
	g++ -Wall -o decode-message message.o decode-message.cpp `libpng-config --ldflags`
message.o: message.h message.cpp
	g++ -Wall -o message.o -c `libpng-config --cflags` message.cpp
