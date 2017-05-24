#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include<iostream>
#include<fstream>
#include <string>
#include <sstream>
#pragma comment(lib, "glew32.lib")
#define _USE_MATH_DEFINES
#include <math.h>

#define WINDSIZEX 500
#define WINDSIZEY 500


using namespace std;



static char* vsSource = "#version 120 \n\
attribute vec4 aPosition; \n\
attribute vec4 aColor; \n\
varying vec4 vColor; \n\
uniform mat4 uModel; \n\
uniform mat4 uRot; \n\
void main(void) { \n\
  gl_Position = uRot * uModel * aPosition; \n\
  vColor = aColor; \n\
}";

static char* fsSource = "#version 120 \n\
varying vec4 vColor; \n\
void main(void) { \n\
  gl_FragColor = vColor; \n\
}";

GLuint vs = 0;
GLuint fs = 0;
GLuint prog = 0;

GLfloat vertices[] = {
	-0.5, -0.5, -0.5, 1.0, // 0
	-0.5, -0.5, +0.5, 1.0, // 1
	-0.5, +0.5, -0.5, 1.0, // 2
	-0.5, +0.5, +0.5, 1.0, // 3
	+0.5, -0.5, -0.5, 1.0, // 4
	+0.5, -0.5, +0.5, 1.0, // 5
	+0.5, +0.5, -0.5, 1.0, // 6
	+0.5, +0.5, +0.5, 1.0, // 7
};

GLfloat colors[] = {
	0.5, 0.5, 0.5, 1.0, // black
	0.5, 0.5, 1.0, 1.0, // blue
	0.5, 1.0, 0.5, 1.0, // green
	0.5, 1.0, 1.0, 1.0, // cyan
	1.0, 0.5, 0.5, 1.0, // red
	1.0, 0.5, 1.0, 1.0, // magenta
	1.0, 1.0, 0.5, 1.0, // yellow
	1.0, 1.0, 1.0, 1.0, // white
};

string indicesStr[] = {
	"0.5", "0.5", "0.5", "1.0", // black
	"0.5", "0.5", "0.5", "1.0", // black
	"0.5", "0.5", "0.5", "1.0", // black
	"0.5", "0.5", "0.5", "1.0", // black
	"0.5", "0.5", "0.5", "1.0", // black
	"0.5", "0.5", "0.5", "1.0", // black
	"0.5", "0.5", "0.5", "1.0", // black
	"0.5", "0.5", "0.5", "1.0", // black
};

GLushort indices[] = { // 36 points, 12 triangles
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
};

GLuint vbo[1];

char buf[1024];

GLfloat motion_start[3]; // start vector, (x, y, z)
GLfloat motion_end[3]; // end vector, (x, y, z)

GLfloat matModel[16] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0,
};
GLfloat matRot[16] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0,
};
GLfloat matA[16];
GLfloat matB[16];

void matCopy(GLfloat a[16], GLfloat b[16]) { // a = b;
	a[0] = b[0]; a[4] = b[4]; a[8] = b[8]; a[12] = b[12];
	a[1] = b[1]; a[5] = b[5]; a[9] = b[9]; a[13] = b[13];
	a[2] = b[2]; a[6] = b[6]; a[10] = b[10]; a[14] = b[14];
	a[3] = b[3]; a[7] = b[7]; a[11] = b[11]; a[15] = b[15];
}

void matIden(GLfloat a[16]) { // a = I
	a[0] = 1.0; a[4] = 0.0; a[8] = 0.0; a[12] = 0.0;
	a[1] = 0.0; a[5] = 1.0; a[9] = 0.0; a[13] = 0.0;
	a[2] = 0.0; a[6] = 0.0; a[10] = 1.0; a[14] = 0.0;
	a[3] = 0.0; a[7] = 0.0; a[11] = 0.0; a[15] = 1.0;
}

void matMult(GLfloat c[16], GLfloat a[16], GLfloat b[16]) { // C = A * B
	c[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
	c[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];
	c[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
	c[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];
	//
	c[4] = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7];
	c[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7];
	c[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
	c[7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7];
	//
	c[8] = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11];
	c[9] = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11];
	c[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];
	c[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11];
	//
	c[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];
	c[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];
	c[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
	c[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];
}

void setVector(int xwin, int ywin, GLfloat vec[3]) {
	GLfloat rad = sqrtf((WINDSIZEX / 2) * (WINDSIZEX / 2) + (WINDSIZEY / 2) * (WINDSIZEY / 2));
	vec[0] = (xwin - WINDSIZEX / 2) / rad;
	vec[1] = (WINDSIZEY / 2 - ywin) / rad;
	vec[2] = sqrtf(1.0F - vec[0] * vec[0] - vec[1] * vec[1]);
	printf("vec = %f,%f,%f\n", vec[0], vec[1], vec[2]);
}


void trackball(GLfloat va[3], GLfloat vb[3]) {
	GLfloat vc[3];
	GLfloat costheta, sintheta, theta;
	GLfloat q[4]; // quaternion (x, y, z, w)
	vc[0] = va[1] * vb[2] - va[2] * vb[1];
	vc[1] = va[2] * vb[0] - va[0] * vb[2];
	vc[2] = va[0] * vb[1] - va[1] * vb[0];
	costheta = va[0] * vb[0] + va[1] * vb[1] + va[2] * vb[2];
	sintheta = sqrtf(vc[0] * vc[0] + vc[1] * vc[1] + vc[2] * vc[2]);
	theta = atan2f(sintheta, costheta);
	printf("theta = %f rad, vec = %f,%f,%f\n", theta, vc[0], vc[1], vc[2]);
	q[0] = vc[0] * sinf(theta / 2.0F);
	q[1] = vc[1] * sinf(theta / 2.0F);
	q[2] = vc[2] * sinf(theta / 2.0F);
	q[3] = cosf(theta / 2.0F);
	matA[0] = q[3]; matA[4] = q[2]; matA[8] = -q[1]; matA[12] = q[0];
	matA[1] = -q[2]; matA[5] = q[3]; matA[9] = q[0]; matA[13] = q[1];
	matA[2] = q[1]; matA[6] = -q[0]; matA[10] = q[3]; matA[14] = q[2];
	matA[3] = -q[0]; matA[7] = -q[1]; matA[11] = -q[2]; matA[15] = q[3];
	matB[0] = q[3]; matB[4] = q[2]; matB[8] = -q[1]; matB[12] = -q[0];
	matB[1] = -q[2]; matB[5] = q[3]; matB[9] = q[0]; matB[13] = -q[1];
	matB[2] = q[1]; matB[6] = -q[0]; matB[10] = q[3]; matB[14] = -q[2];
	matB[3] = q[0]; matB[7] = q[1]; matB[11] = q[2]; matB[15] = q[3];
	matMult(matRot, matA, matB);
}

void mymotion(int xwin, int ywin) {
	setVector(xwin, ywin, motion_end);
	trackball(motion_start, motion_end);
	glutPostRedisplay();
}

void mymouse(int button, int state, int xwin, int ywin) {
	switch (state) {
	case GLUT_DOWN:
		setVector(xwin, ywin, motion_start);
		break;
	case GLUT_UP:
		setVector(xwin, ywin, motion_end);
		trackball(motion_start, motion_end);
		matMult(matA, matRot, matModel); // matA = matRot * matModel
		matCopy(matModel, matA); // matModel updated
		matIden(matRot); // matRot = I
		glutPostRedisplay();
		break;
	}
}

void myinit(void) {
	GLint status;
	GLuint loc;
	// vs: vertex shader
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSource, NULL);
	glCompileShader(vs); // compile to get .OBJ
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	printf("vs compile status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetShaderInfoLog(vs, sizeof(buf), NULL, buf);
	printf("vs log = [%s]\n", buf);
	// fs: fragment shader
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSource, NULL);
	glCompileShader(fs); // compile to get .OBJ
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	printf("fs compile status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetShaderInfoLog(fs, sizeof(buf), NULL, buf);
	printf("fs log = [%s]\n", buf);
	// prog: program
	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog); // link to get .EXE
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	printf("program link status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetProgramInfoLog(prog, sizeof(buf), NULL, buf);
	printf("link log = [%s]\n", buf);
	glValidateProgram(prog);
	glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
	printf("program validate status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetProgramInfoLog(prog, sizeof(buf), NULL, buf);
	printf("validate log = [%s]\n", buf);
	glUseProgram(prog); // execute it !
						// VBO setting
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 2 * 8 * 4 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * 4 * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 8 * 4 * sizeof(GLfloat), 8 * 4 * sizeof(GLfloat), colors);
	// provide the vertex attributes
	loc = glGetAttribLocation(prog, "aPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	// provide the color attributes
	loc = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(8 * 4 * sizeof(GLfloat)));
	// depth buffer enabled
	glEnable(GL_DEPTH_TEST);
}

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESCAPE
		exit(0);
		break;
	}
}

void myidle(void) {
	// nothing
}


void mydisplay(void) {
	GLuint loc;
	// clear 
	glClearColor(0.3F, 0.3F, 0.3F, 1.0F); // gray
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// provide the matrices
	loc = glGetUniformLocation(prog, "uModel");
	glUniformMatrix4fv(loc, 1, GL_FALSE, matModel);
	loc = glGetUniformLocation(prog, "uRot");
	glUniformMatrix4fv(loc, 1, GL_FALSE, matRot);
	// draw a triangle
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_SHORT, indices);
	// flush all
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	std::ifstream File("box.obj");
	string Line;
	string Name;
	int n = 0;
	int o = 0;
	float tempx = 0;
	float tempy = 0;
	float tempz = 0;
	char stempx[5] = { 0 };
	char stempy[5] = { 0 };
	char stempz[5] = { 0 };
	int itempx = 0;
	int itempy = 0;
	int itempz = 0;

	while (std::getline(File, Line)) {
		if (Line == "" || Line[0] == '#')
			continue;

		std::istringstream LineStream(Line);
		LineStream >> Name;

		if (Name == "v") {
			sscanf(Line.c_str(), "%*s %f %f %f", &tempx, &tempy, &tempz);
			//devide by 2 for now so that object fits in screen
			vertices[n] = tempx/2;
			vertices[n + 1] = tempy/2;
			vertices[n + 2] = tempz/2;
			//go to start of next vertex in array
			n = n + 4;
		}
		if (Name == "f") {
			sscanf(Line.c_str(), "%*s %s %s %s", stempx, stempy, stempz);
			//get first char of array and turn to int
			itempx = stempx[0] - '0';
			itempy = stempy[0] - '0';
			itempz = stempz[0] - '0';
			//minus one to start indeces at 0 insted of 1
			indices[o] = itempx - 1;
			indices[o + 1] = itempy - 1;
			indices[o + 2] = itempz - 1;
			//go to start of next indice line
			o = o + 3;
		}
	};
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WINDSIZEX, WINDSIZEY);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);
	glutMouseFunc(mymouse);
	glutMotionFunc(mymotion);
	glutIdleFunc(myidle);
	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}

