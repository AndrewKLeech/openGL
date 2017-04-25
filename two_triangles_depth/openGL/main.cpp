#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#pragma comment(lib, "glew32.lib")

static char* vsSource = "#version 120 \n\
attribute vec4 aPosition;; \n\
void main(void) { \n\
  gl_Position = aPosition;; \n\
}";

static char* fsSource = "#version 120 \n\
uniform vec4 uColor;\n\
void main(void) { \n\
  gl_FragColor =  uColor;\n\
}";

GLuint vs = 0;
GLuint fs = 0;
GLuint prog = 0;

void myinit(void) {
	GLuint status;
	// depth test on
	glEnable(GL_DEPTH_TEST);
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

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESCAPE
		exit(0);
		break;
	}
}

GLfloat first[] = {
	+0.2, +0.2, -0.5, 1.0,
	-0.8, +0.2, -0.5, 1.0,
	+0.2, -0.8, -0.5, 1.0,
};
GLfloat second[] = {
	-0.2, -0.2, -0.2, 1.0,
	+0.8, -0.2, -0.2, 1.0,
	-0.2, +0.8, -0.2, 1.0,
};

void mydisplay(void) {
	GLuint locPosition;
	GLuint locColor;
	// clear in gray color
	glClearColor(0.3F, 0.3F, 0.3F, 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// provide the vertex attributes
	locPosition = glGetAttribLocation(prog, "aPosition");
	glEnableVertexAttribArray(locPosition);
	locColor = glGetUniformLocation(prog, "uColor");
	// draw first triangle
	glVertexAttribPointer(locPosition, 4, GL_FLOAT, GL_FALSE, 0, first);
	glUniform4f(locColor, 1.0F, 0.0F, 0.0F, 1.0F); // red
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// draw second triangle
	glVertexAttribPointer(locPosition, 4, GL_FLOAT, GL_FALSE, 0, second);
	glUniform4f(locColor, 0.0F, 0.0F, 1.0F, 1.0F); // blue
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// flush all
	glFlush();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);
	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}
