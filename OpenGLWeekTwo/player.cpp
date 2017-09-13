#include "player.h"

Player::Player() {
	x = 0;
	y = 0;

	width = 64;
	height = 128;

	facing_right = true;

	current_state = PLAYER_STANDING; 

	frame_skip = 5;
	current_frame = 4;
	current_skip = 0;

	move_dist = 2;

	jump_height = 50;
	jump_frames = 8;
	current_jump_frame = 0; 
}

void Player::load(string texture_path) {
	BMP player_bmp(texture_path);

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, player_bmp.width, player_bmp.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, player_bmp.image_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Player::draw() {
	/*
	if (current_state == PLAYER_STANDING) {
		draw_frame(4);
	}
	else if (current_state == PLAYER_WALKING) {

	}
	*/

	draw_frame(current_frame);

	current_skip += 1;
	if (current_skip >= frame_skip) {
		current_skip = 0;
		current_frame += 1;
		// make current_frame not go over 5-ish
		if (current_state == PLAYER_STANDING) {
			current_frame = 4;
		}
		else if (current_state == PLAYER_WALKING) {
			if (current_frame > 3) {
				current_frame = 0;
			}
		}
		else if (current_state == PLAYER_JUMPING) {
			current_frame = 1;
			current_jump_frame += 1;

			if (current_jump_frame >= jump_frames) {
				current_state = PLAYER_STANDING;
				current_frame = 4;
				current_jump_frame = 0;
				y = 0;
			}
			else {
				float sin_pos = ((float)current_jump_frame / (float)jump_frames) * 3.14159265359f;
				y = sin(sin_pos) * jump_height;
			}
		}
	}
}

void Player::draw_frame(int frame) {

	float texture_offset = (float)frame / 5.0f;
	float text_frame_width = 0.2f;


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glBegin(GL_QUADS);

	if (facing_right) {
		glTexCoord2f(texture_offset, 1.0f); glVertex2f(x, y);
		glTexCoord2f(texture_offset + text_frame_width, 1.0f); glVertex2f(x + width, y);
		glTexCoord2f(texture_offset + text_frame_width, 0.0f); glVertex2f(x + width, y + height);
		glTexCoord2f(texture_offset, 0.0f); glVertex2f(x, y + height);
	}
	else {
		glTexCoord2f(texture_offset + text_frame_width, 1.0f); glVertex2f(x, y);
		glTexCoord2f(texture_offset , 1.0f); glVertex2f(x + width, y);
		glTexCoord2f(texture_offset , 0.0f); glVertex2f(x + width, y + height);
		glTexCoord2f(texture_offset + text_frame_width, 0.0f); glVertex2f(x, y + height);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Player::player_stop() {
	if (current_state != PLAYER_JUMPING) {
		current_frame = 4;
		current_state = PLAYER_STANDING;
	}
}

void Player::move_right() {
	x += move_dist;
	if (current_state != PLAYER_JUMPING) current_state = PLAYER_WALKING;
	facing_right = true;
}

void Player::move_left() {
	x -= move_dist;
	if (current_state != PLAYER_JUMPING) current_state = PLAYER_WALKING;
	facing_right = false;
}

void Player::jump() {
	
	if (current_state != PLAYER_JUMPING) {
		current_state = PLAYER_JUMPING;
		current_frame = 1;
	}

}
