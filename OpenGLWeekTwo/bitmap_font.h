#ifndef BITMAP_FONT_H
#define BITMAP_FONT_H

#include <glew.h>
#include <glut.h>

#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>

#include "BMP.h"


class BitmapFontChar {
public:
	uint32_t id;
	uint16_t x, y, width, height;
	int16_t xoffset, yoffset, xadvance;
	uint8_t page, channel;

	BitmapFontChar();
	BitmapFontChar(uint8_t *data);
};

class BitmapFont {
public:
	GLuint font_texid;

	int texture_w;
	int texture_h;

	vector<BitmapFontChar> chars;

	BitmapFont();

	void Load(string font_bin_file, string font_bmp_file);

	int draw_char(int x, int y, char c);

	int draw_string(int x, int y, string s);

	float texture_position_x(int x);

	float texture_position_y(int y);

};

#endif