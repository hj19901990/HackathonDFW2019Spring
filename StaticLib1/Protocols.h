#pragma once

#include <string>

typedef struct {

	int id;

	int name_len;

	char name[48];

	int age;   

	int gender;      //1 male   2 female

	int emotion;    //happy 1  normal 2  upset 3

}Individual_Info;

#define PACKET_TYPE_PIC 0xA0
#define PACKET_TYPE_INDIVIDUAL_INFO 0xA1
typedef struct {

	int cmdType;

	int timestamp;

}Common_Header;

typedef struct {

	Common_Header header;

	int width;

	int height;


}Pic_RGB_Header;

typedef struct {

	Common_Header header;

	/*
	0 Not detected
	1 Face Detected! And Recognized
	2 Face Detected! But Not Recognized
	*/
	int flag;

	Individual_Info individual_info;
}Individual_Info_Header;

class Pic {

public:

	int timestamp;

	int width;

	int height;

	char *buffer;

public:
	Pic(Pic_RGB_Header* header) {

		timestamp = header->header.timestamp;

		width = header->width;

		height = header->height;

		int pic_size = width * height * 3;

		buffer = new char[pic_size];

		std::memcpy(buffer, (char *)header + sizeof(Pic_RGB_Header), pic_size);

	}
	~Pic() {

		delete buffer;
	}
};

class Individual {

public:
	int id;

	std::string name;

	int age;

	int gender;

	int emotion;        

public:

	Individual() :
		id(-1),
		name(""),
		age(-1),
		gender(-1),
		emotion(-1) {

	}

	Individual(Individual_Info_Header *header) :
		id(header->individual_info.id),
		name(header->individual_info.name, header->individual_info.name_len),
		age(header->individual_info.age),
		gender(header->individual_info.gender),
		emotion(header->individual_info.emotion)
	{}

	~Individual() {

	}

};