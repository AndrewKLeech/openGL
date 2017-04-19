#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
//Vertex Shader
static char* vsSource = "#version 120\n\
attribute vec4 vertex;\n\
void main(void) {\n\
		gl_Position = vertex;\n\
}";
//Fragment Shader
static char* fsSource = "#version 120\n\
void main(void) {\n\
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n\
	}";

GLfloat plane0[] = { // initial position
	-0.2F, -0.8F, 0.0F, 1.0F,
	+0.2F, -0.8F, 0.0F, 1.0F,
	+0.0F, -0.5F, 0.0F, 1.0F,
};
GLfloat plane1[] = { // current position
	-0.2F, -0.8F, 0.0F, 1.0F,
	+0.2F, -0.8F, 0.0F, 1.0F,
	+0.0F, -0.5F, 0.0F, 1.0F,
};
GLfloat stone0[] = { // initial obstacle
	-0.5F, +0.9F, 0.0F, 1.0F,
	-0.5F, +0.8F, 0.0F, 1.0F,
	-0.4F, +0.9F, 0.0F, 1.0F,
	//TODO
};
GLfloat stone1[] = { // dropping obstacle
	-0.5F, +0.9F, 0.0F, 1.0F,
	-0.5F, +0.8F, 0.0F, 1.0F,
	-0.4F, +0.9F, 0.0F, 1.0F,
	//TODO
};

GLuint vs = 0;
GLuint fs = 0;
GLuint prog = 0;
const float planeStep = 0.04F; // may be tuned for your PC
const float stoneStep = 0.001F;

void myidle(void) {
	// moving stone ! : you can add AI logic here!
	stone1[1] -= stoneStep; // vertex#0.y
	stone1[5] -= stoneStep; // vertex#1.y
	stone1[9] -= stoneStep; // vertex#2.y
	if (stone1[9] < -1.0F) {
		memcpy(stone1, stone0, sizeof(stone0));
	}
	// redisplay
	glutPostRedisplay();
}

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: exit(0); break; // ESCAPE
	case 'r': // reset
		memcpy(plane1, plane0, sizeof(plane0));
		glutPostRedisplay();
		break;
		// user interface: simple joystick!
	case 'a': // left move
		plane1[0] -= planeStep;
		plane1[4] -= planeStep;
		plane1[8] -= planeStep;
		glutPostRedisplay();
		break;
	case 'd': // right move
		//TODO
		plane1[0] += planeStep;
		plane1[4] += planeStep;
		plane1[8] += planeStep;
		glutPostRedisplay();
		break;
	}
}

void myinit(void) {
	// vs: vertex shader
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSource, NULL);
	glCompileShader(vs); // compile to get .OBJ
						 // fs: fragment shader
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSource, NULL);
	glCompileShader(fs); // compile to get .OBJ
						 // prog: program
	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog); // link to get .EXE
	glUseProgram(prog); // execute it !
}

void mydisplay(void) {
	GLuint loc;
	// clear in black color
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	// provide the vertex attributes
	loc = glGetAttribLocation(prog, "vertex");
	glEnableVertexAttribArray(loc);
	// draw the plane
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, plane1);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// draw the stone
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, stone1);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// flush all
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("game");
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);
	glutIdleFunc(myidle);
	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}