#include "BMP.h"

BMP::BMP() {

}

BMP::BMP(int width, int height, int bytes_per_pixel = 3) {
	this->width = width;
	this->height = height;

	this->bytes_per_pixel = bytes_per_pixel;

	this->bmp_file_size = 0;

	this->image_data = new uint8_t[width * height * this->bytes_per_pixel];
	memset(this->image_data, 0x00, width * height * this->bytes_per_pixel);
}

BMP::BMP(string path) {
	load(path);
}

void BMP::save(string path) {
	int bmp_padding = (4 - (width * bytes_per_pixel) % 4) % 4;
	bmp_file_size = 122 + (((width * bytes_per_pixel) + bmp_padding) * height);

	uint8_t bmp_file_header[14];
	memset(bmp_file_header, 0x00, 14);
	bmp_file_header[0] = 66;
	bmp_file_header[1] = 77;
	bmp_file_header[2] = (bmp_file_size >> 0) & 0xff;
	bmp_file_header[3] = (bmp_file_size >> 8) & 0xff;
	bmp_file_header[4] = (bmp_file_size >> 16) & 0xff;
	bmp_file_header[5] = (bmp_file_size >> 24) & 0xff;
	bmp_file_header[10] = 122;

	uint8_t bmp_image_header[108];
	memset(bmp_image_header, 0x00, 108);
	bmp_image_header[0] = 108;
	bmp_image_header[4] = (width >> 0) & 0xff;
	bmp_image_header[5] = (width >> 8) & 0xff;
	bmp_image_header[6] = (width >> 16) & 0xff;
	bmp_image_header[7] = (width >> 24) & 0xff;
	bmp_image_header[8] = (height >> 0) & 0xff;
	bmp_image_header[9] = (height >> 8) & 0xff;
	bmp_image_header[10] = (height >> 16) & 0xff;
	bmp_image_header[11] = (height >> 24) & 0xff;
	bmp_image_header[12] = 1;
	bmp_image_header[14] = bytes_per_pixel * 8;

	if (bytes_per_pixel == 4) {
		bmp_image_header[16] = 3;

		// red bitmask
		bmp_image_header[40] = 0;
		bmp_image_header[41] = 0;
		bmp_image_header[42] = 0;
		bmp_image_header[43] = 0xff;

		// green bitmask
		bmp_image_header[44] = 0;
		bmp_image_header[45] = 0;
		bmp_image_header[46] = 0xff;
		bmp_image_header[47] = 0;

		// blue bitmask
		bmp_image_header[48] = 0;
		bmp_image_header[49] = 0xff;
		bmp_image_header[50] = 0;
		bmp_image_header[51] = 0;

		// alpha bitmask
		bmp_image_header[52] = 0xff;
		bmp_image_header[53] = 0;
		bmp_image_header[54] = 0;
		bmp_image_header[55] = 0;
	}


	fstream output(path.c_str(), ios::out | ios::binary | ios::trunc);
	output.write((char*)bmp_file_header, 14);
	output.write((char*)bmp_image_header, 108);

	for (int i = 0; i < height; i++) {
		int line_offset = width * (height - i - 1) * bytes_per_pixel;
		for (int x = 0; x < width; x++) {
			for (int c = (bytes_per_pixel - 1); c >= 0; c--) {
				output.write((char*)&image_data[line_offset + (x * bytes_per_pixel) + c], 1);
			}
		}

		output.write(0x00, bmp_padding);
	}

	output.close();
}

void BMP::load(string path) {
	fstream input(path.c_str(), ios::in | ios::binary);

	uint8_t bmp_file_header[14];
	input.seekg(0, ios::beg);
	input.read((char*)&bmp_file_header, 14);

	bmp_file_size = 0;
	uint32_t image_data_offset = 0;
	for (int i = 0; i < 4; i++) {
		bmp_file_size += (bmp_file_header[2 + i] << (i * 8));
		image_data_offset += (bmp_file_header[10 + i] << (i * 8));
	}

	uint8_t bmp_image_header[40];
	input.seekg(14, ios::beg);
	input.read((char*)&bmp_image_header, 40);

	width = 0; height = 0;
	for (int i = 0; i < 4; i++) {
		width += (bmp_image_header[4 + i] << (i * 8));
		height += (bmp_image_header[8 + i] << (i * 8));
	}

	bytes_per_pixel = bmp_image_header[14] / 8;

	this->image_data = new uint8_t[width * height * bytes_per_pixel];
	memset(this->image_data, 0x00, width * height * bytes_per_pixel);

	int bmp_padding = (4 - (width * bytes_per_pixel) % 4) % 4;

	for (int i = 0; i < height; i++) {
		int file_offset = (((width * bytes_per_pixel) + bmp_padding) * i) + image_data_offset;
		input.seekg(file_offset, ios::beg);

		int line_offset = width * (height - i - 1) * bytes_per_pixel;
		for (int x = 0; x < width; x++) {
			for (int c = (bytes_per_pixel - 1); c >= 0; c--) {
				input.read((char*)&image_data[line_offset + (x * bytes_per_pixel) + c], 1);
			}
		}
	}

	input.close();
}

uint8_t* BMP::get_pixel(int x, int y) {
	if ((x >= 0 && x < width) && (y >= 0 && y < height)) {
		int offset = (y * this->width * bytes_per_pixel) + (x * bytes_per_pixel);
		return &this->image_data[offset];
	}
	else {
		return new uint8_t(bytes_per_pixel);
	}
}

void BMP::set_pixel(int x, int y, uint8_t* pixel) {
	if ((x >= 0 && x < width) && (y >= 0 && y < height)) {
		int offset = (y * this->width * bytes_per_pixel) + (x * bytes_per_pixel);
		memcpy(&this->image_data[offset], pixel, bytes_per_pixel);
	}
}