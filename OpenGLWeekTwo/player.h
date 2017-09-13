#ifndef PLAYER_H
#define PLAYER_H

#include <glew.h>
#include <glut.h>

#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <cmath>

#include "BMP.h"


enum PLAYER_STATE {
	PLAYER_STANDING = 0, 
	PLAYER_WALKING = 1,
	PLAYER_JUMPING = 2,
};


class Player {
private:
	int frame_skip, current_frame, current_skip;

	int move_dist;

	int jump_height;
	int jump_frames;
	int current_jump_frame;
public:
	PLAYER_STATE current_state;
	
	uint16_t x, y, width, height;

	GLuint texture_id;

	bool facing_right;

	Player();

	void load(string texture_path);
		
	void draw();

	void draw_frame(int frame);

	void player_stop();
	void move_right();
	void move_left();
	void jump();


};



#endif