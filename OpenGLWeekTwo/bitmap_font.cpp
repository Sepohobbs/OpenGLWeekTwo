#include "bitmap_font.h"

BitmapFont::BitmapFont() {

}

void BitmapFont::Load(string font_bin_file, string font_bmp_file) {
	// load font texture
	
	BMP font_bmp(font_bmp_file);
	
	texture_w = font_bmp.width;
	texture_h = font_bmp.height;

	glGenTextures(1, &font_texid);
	glBindTexture(GL_TEXTURE_2D, font_texid);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, font_bmp.width, font_bmp.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, font_bmp.image_data);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	ifstream font_bin(font_bin_file);
	font_bin.seekg(4, font_bin.beg);
	
	uint8_t block_info[5];
	while (!font_bin.eof()) {
		int current_pos = font_bin.tellg();
		font_bin.read((char*)block_info, 5);
		uint8_t block_id = block_info[0];
		int32_t block_size = 0;
		for (int i = 0; i < 4; i++) {
			block_size += (block_info[1 + i] << (i * 8));
		}
		
		if (block_id == 4) {
			int charblock_offset = 0;

			uint8_t char_data[20];
			while (charblock_offset < block_size) {
				font_bin.read((char*)char_data, 20);

				BitmapFontChar current_char(char_data);

				chars.push_back(current_char);

				charblock_offset += 20;
			}
			
		}

		font_bin.seekg(current_pos + block_size + 4 + 1, font_bin.beg);
	}

	font_bin.close();
}

int BitmapFont::draw_char(int x, int y, char c) {
	for (int i = 0; i < chars.size(); i++) {
		if (chars[i].id == c) {
			float text_x = texture_position_x(chars[i].x);
			float text_y = texture_position_y(chars[i].y);
			float text_w = texture_position_x(chars[i].width);
			float text_h = texture_position_y(chars[i].height);

			x += chars[i].xoffset;
			y += chars[i].yoffset;

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, font_texid);
			glBegin(GL_QUADS);
			glTexCoord2f(text_x, text_y + text_h); glVertex2f(x, y);
			glTexCoord2f(text_x + text_w, text_y + text_h); glVertex2f(x + chars[i].width, y);
			glTexCoord2f(text_x + text_w, text_y); glVertex2f(x + chars[i].width, y + chars[i].height);
			glTexCoord2f(text_x, text_y); glVertex2f(x, y + chars[i].height);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			return chars[i].xadvance;
		}
	}
	return 0; 
}

int BitmapFont::draw_string(int x, int y, string s) {
	int offset = 0;

	for (int i = 0; i < s.length(); i++) {
		offset += draw_char(x + offset, y, s[i]);
	}
	return offset;
}

float BitmapFont::texture_position_x(int x) {
	return  (float)x / (float)texture_w;
}

float BitmapFont::texture_position_y(int y) {
	return  (float)y / (float)texture_h;
}

BitmapFontChar::BitmapFontChar() {
	
}

BitmapFontChar::BitmapFontChar(uint8_t * data) {
	id = 0;

	for (int i = 0; i < 4; i++) {
		id += (data[i + 0] << (i * 8));
	}

	x = 0;
	y = 0;

	width = 0;
	height = 0;
	
	xoffset = 0;
	yoffset = 0;

	xadvance = 0;

	for (int i = 0; i < 2; i++) {
		x += (data[i + 4] << (i * 8));
		y += (data[i + 6] << (i * 8));

		width += (data[i + 8] << (i * 8));
		height += (data[i + 10] << (i * 8));

		xoffset += (data[i + 12] << (i * 8));
		yoffset += (data[i + 14] << (i * 8));

		xadvance += (data[i + 16] << (i * 8));
	}

	page = data[18];
	channel = data[19];
}
