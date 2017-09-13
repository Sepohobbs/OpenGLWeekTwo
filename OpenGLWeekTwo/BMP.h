#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <string>
#include <cstring>

#include <iostream>
#include <fstream>

using namespace std;

class BMP {
public:
	int width, height;
	int bytes_per_pixel;
	uint8_t* image_data;

	int bmp_file_size;

	BMP();
	BMP(int, int, int);
	BMP(string);

	void save(string);
	void load(string);

	uint8_t* get_pixel(int, int);

	void set_pixel(int, int, uint8_t*);
};

#endif