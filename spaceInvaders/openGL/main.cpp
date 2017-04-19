#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
//Vertex Shader
static char* vsSource = "#version 120\n\
attribute vec4 vertex;\n\
attribute vec4 aColor;\n\
varying vec4 vColor;\n\
void main(void) {\n\
		gl_Position = vertex;\n\
		vColor = aColor;\n\
}";
//Fragment Shader
static char* fsSource = "#version 120\n\
varying vec4 vColor;\n\
void main(void) {\n\
			gl_FragColor = vColor;\n\
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
	-0.7F, +0.9F, 0.0F, 1.0F,
	-0.6F, +0.8F, 0.0F, 1.0F,
};
GLfloat stone1[] = { // dropping obstacle
	-0.5F, +0.9F, 0.0F, 1.0F,
	-0.7F, +0.9F, 0.0F, 1.0F,
	-0.6F, +0.8F, 0.0F, 1.0F,
};
GLfloat bullet0[] = { // initial bullet
	 0.0F, -0.2F, 0.0F, 1.0F,
	+0.05F, -0.5F, 0.0F, 1.0F,
	-0.05F, -0.5F, 0.0F, 1.0F,
};
GLfloat bullet1[] = { // moving bullet
	 0.0F, -0.2F, 0.0F, 1.0F,
	+0.05F, -0.5F, 0.0F, 1.0F,
	-0.05F, -0.5F, 0.0F, 1.0F,
};

GLfloat planeColor[] = {
	1.0, 0.0, 0.0, 1.0, // red
	1.0, 0.0, 0.0, 1.0, // red
	1.0, 0.0, 0.0, 1.0, // red
};
GLfloat bulletColor[] = {
	0.0, 0.0, 1.0, 1.0, // blue
	0.0, 0.0, 1.0, 1.0, // blue
	0.0, 0.0, 1.0, 1.0, // blue
};

GLuint vs = 0;
GLuint fs = 0;
GLuint prog = 0;
const float planeStep = 0.04F; // may be tuned for your PC
const float stoneStep = 0.001F;
bool shoot = false;
void myidle(void) {
	// moving stone ! : you can add AI logic here!
	stone1[1] -= stoneStep; // vertex#0.y
	stone1[5] -= stoneStep; // vertex#1.y
	stone1[9] -= stoneStep; // vertex#2.y
	if (stone1[9] < -1.0F) {
		memcpy(stone1, stone0, sizeof(stone0));
	}
	if (shoot) {
		bullet1[1] += stoneStep; // vertex#0.y
		bullet1[5] += stoneStep; // vertex#1.y
		bullet1[9] += stoneStep; // vertex#2.y
		if (bullet1[9] > 1.0F) {
			shoot = false;
			memcpy(bullet1, bullet0, sizeof(stone0));
		}
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
		bullet0[0] -= planeStep;
		bullet0[4] -= planeStep;
		bullet0[8] -= planeStep;
		glutPostRedisplay();
		break;
	case 'd': // right move
		//TODO
		plane1[0] += planeStep;
		plane1[4] += planeStep;
		plane1[8] += planeStep;
		bullet0[0] += planeStep;
		bullet0[4] += planeStep;
		bullet0[8] += planeStep;
		glutPostRedisplay();
		break;
	case 32:
		shoot = true;
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
	GLuint locColor;
	// clear in black color
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	// provide the vertex attributes
	loc = glGetAttribLocation(prog, "vertex");
	glEnableVertexAttribArray(loc);
	// draw the plane
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, plane1);
	// provide the color attributes
	locColor = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(locColor);
	glVertexAttribPointer(locColor, 4, GL_FLOAT, GL_FALSE, 0, planeColor);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// draw the stone
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, stone1);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// draw the bullet
	if (shoot) {
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, bullet1);
		// provide the color attributes
		locColor = glGetAttribLocation(prog, "aColor");
		glEnableVertexAttribArray(locColor);
		glVertexAttribPointer(locColor, 4, GL_FLOAT, GL_FALSE, 0, bulletColor);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
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