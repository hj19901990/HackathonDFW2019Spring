#pragma once

#include <string>

typedef struct {

	int id;

	int name_len;

	char *name;

	int age;

	int gender;

	int note_len;

	char *note;

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

	char buf;

}Pic_RGB_Header;

typedef struct {

	Common_Header header;

	/*
	0 Not detected
	1 Face Detected! And Recognized
	2 Face Detected! But Not Recognized
	*/
	int flag;

	Individual_Info *individual_info;
}Individual_Info_Header;

class Pic {

public:
	int width;

	int height;

	char *buffer;

public:
	Pic(Pic_RGB_Header* header) {

		width = header->width;

		height = header->height;

		int pic_size = width * height * 3;

		buffer = new char[pic_size];

		std::copy(&header->buf, &header->buf + pic_size, buffer);

	}
	~Pic() {

		delete buffer;
	}
};

class Individual {

private:
	int id;

	std::string name;

	int age;

	int gender;

	std::string note;

public:

	Individual() :
		id(-1),
		name(""),
		age(-1),
		gender(-1),
		note("") {

	}

	Individual(Individual_Info_Header *header) :
		id(header->individual_info->id),
		name(header->individual_info->name, header->individual_info->name_len),
		age(header->individual_info->age),
		gender(header->individual_info->gender),
		note(header->individual_info->note, header->individual_info->note_len)
	{}

	~Individual() {

	}

};