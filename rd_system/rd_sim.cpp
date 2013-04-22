#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include "freeglut.h"
#include "bitmapUtil.h"
using std::vector;
#pragma comment( lib, "freeglut.lib" )

#include "rd_simulator.h"

const int Mapsize = 200;
const int Windowsize = 600;
rdmap rd(Mapsize);
static float *tex;
static int tickCount = 0;
char windowPixels[3*Windowsize*Windowsize];

void timertick(int value)
{
	for(int i = 0; i < 30; i++){
		rd.move();
	}
    glutPostRedisplay();
    glutTimerFunc(16, timertick, 0);
	std::stringstream ss;
	//glReadPixels(0, 0, Windowsize, Windowsize, GL_BGR_EXT, GL_UNSIGNED_BYTE, windowPixels);
	//ss << "bmp/case2/case_" << std::setw(10) << std::setfill('0') << tickCount << ".bmp";
	//saveBMP(ss.str(), Mapsize, Mapsize, windowPixels);
	tickCount++;
}

void mousedrug(int x, int y){
	rd.point_delete(x*Mapsize/Windowsize, y*Mapsize/Windowsize, 20*Mapsize/Windowsize);
}

void keydown(unsigned char key, int x, int y){
	switch(key){
	case 'i':
		rd.map_reset();
		break;
	case '1':
		rd.set_f(rd.get_f() + 0.001f);
		break;
	case '2':
		rd.set_f(rd.get_f() - 0.001f);
		break;
	case 'q':
		rd.set_k(rd.get_k() + 0.001f);
		break;
	case 'w':
		rd.set_k(rd.get_k() - 0.001f);
		break;
	case 'a':
		rd.set_ru(rd.get_ru() + 0.002f);
		break;
	case 's':
		rd.set_ru(rd.get_ru() - 0.002f);
		break;
	case 'z':
		rd.set_rv(rd.get_rv() + 0.002f);
		break;
	case 'x':
		rd.set_rv(rd.get_rv() - 0.002f);
		break;
	}
}

void display_string(float x, float y, std::string str){
	glDisable(GL_TEXTURE_2D);
	glColor4f(0.7f,0.8f,0.8f,1.0f);
	glRasterPos3f(x, y, -1.0f);
	const char* p = str.c_str();
	while (*p != '\0') glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p++);
 	glEnable(GL_TEXTURE_2D);
}

void display_params(){
	display_string(-0.9f, 0.9f, "reset: i");
	std::stringstream ss;
	ss.str("");
	ss << "1< >2 f: " << rd.get_f();
	display_string(-0.9f, 0.82f, ss.str());
	ss.str("");
	ss << "q< >w k: " << rd.get_k();
	display_string(-0.9f, 0.74f, ss.str());
	ss.str("");
	ss << "a< >s ru:" << rd.get_ru();
	display_string(-0.9f, 0.66f, ss.str());
	ss.str("");
	ss << "z< >x rv:" << rd.get_rv();
	display_string(-0.9f, 0.58f, ss.str());
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float* rd_v = *(rd.get_v());
	float* tex_current = tex;
	float* rd_v_current = rd_v;
	const float texture_mul = 2.0;
	for(int i = 0; i < Mapsize; i++){
		for(int j = 0; j < Mapsize; j++){
			*tex_current = 0.0;
			tex_current++;
			*tex_current = std::min(texture_mul*(*rd_v_current), 1.0f);
			tex_current++;
			*tex_current = std::min(texture_mul*(*rd_v_current), 1.0f);
			tex_current++;
			rd_v_current++;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Mapsize, Mapsize, 0, GL_RGB, GL_FLOAT, tex);

	glColor3f(1.0,1.0,1.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0,0);glVertex2f(-1.0f, 1.0f);
	glTexCoord2f(1,0);glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(1,1);glVertex2f(1.0f, -1.0f);
	glTexCoord2f(0,1);glVertex2f(1.0f, 1.0f);
	glEnd();

	display_params();

	glutSwapBuffers();
}
int main(int argc, char** argv){
	tex = static_cast<float*>(malloc(Mapsize*Mapsize*3*sizeof(float)));

    glutInitWindowPosition(100, 100);
	glutInitWindowSize(Windowsize, Windowsize);
 
    glutInit(&argc, argv);
 
    glutCreateWindow("Reaction-Diffusion System");
 
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
 
	glutDisplayFunc(display);
    glutTimerFunc(16, timertick, 0);
	glutMotionFunc(mousedrug);
	glutKeyboardFunc(keydown);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

 	glEnable(GL_TEXTURE_2D);
	GLuint texture;
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glutMainLoop();
	
	glDeleteTextures(1,&texture);
	delete tex;

	return 0;
}
