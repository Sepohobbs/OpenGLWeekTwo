#include <glew.h>
#include <glut.h>

#include "bitmap_font.h"
#include "player.h"

int SCREEN_W = 320;
int SCREEN_H = 240;
float ASPECT = (float)SCREEN_W / (float)SCREEN_H;

bool w_down = false;
bool a_down = false;
bool s_down = false;
bool d_down = false;

BitmapFont font;
Player player;

void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	/*
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, test.font_texid);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 1); glVertex2f(280, 200);
	glTexCoord2i(1, 1); glVertex2f(360, 200);
	glTexCoord2i(1, 0); glVertex2f(360, 280);
	glTexCoord2i(0, 0); glVertex2f(280, 280);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	*/
	
	//test.draw_string(100, 100, "test");

	player.draw();
	font.draw_string(player.x, player.y + player.height, "DAN");

	glutSwapBuffers();
}

void keyboard_down_special(int key, int x, int y) {

}

void keyboard_up_special(int key, int x, int y) {

}

void keyboard_down(unsigned char key, int x, int y) {

	if (key == 'w') w_down = true;
	if (key == 'a') a_down = true;
	if (key == 's') s_down = true;
	if (key == 'd') d_down = true;
}

void keyboard_up(unsigned char key, int x, int y) {
	if (key == 'w') w_down = false;
	if (key == 'a') a_down = false;
	if (key == 's') s_down = false;
	if (key == 'd') d_down = false;
}

void force_redraw(int value) {
	glutPostRedisplay();

	if (a_down) player.move_left();
	if (d_down) player.move_right();
	if (w_down) player.jump();

	if (a_down == false && d_down == false) player.player_stop();

	glutTimerFunc(20, force_redraw, 0);
}

void reshape(int width, int height) {
	SCREEN_W = width;
	SCREEN_H = height;
	ASPECT = (float)SCREEN_W / (float)SCREEN_H;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_W, 0, SCREEN_H);
}

int main(int argc, const char * argv[]) {
	glutInit(&argc, const_cast<char**>(argv));

	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA);
	glutInitWindowSize(SCREEN_W, SCREEN_H);
	glutCreateWindow("OpenGLWeekTwo");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLuint error = glewInit();

	glutDisplayFunc(render);
	glutTimerFunc(20, force_redraw, 0);
	glutKeyboardFunc(keyboard_down);
	glutKeyboardUpFunc(keyboard_up);
	glutSpecialFunc(keyboard_down_special);
	glutSpecialUpFunc(keyboard_up_special);
	glutReshapeFunc(reshape);

	glMatrixMode(GL_PROJECTION);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	font.Load("axis.fnt", "axis.bmp");

	player.load("walk.bmp");

	glutMainLoop();

	return 0;
}
