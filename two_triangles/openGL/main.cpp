#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#pragma comment(lib, "glew32.lib")
GLuint vs = 0; // vertex shader ID number
GLuint fs = 0; // fragment shader ID number
GLuint prog = 0; // shader program ID number
				 // vertex array data
GLfloat vertices[] = { // (x, y, z, w)
	-0.5, -0.5, 0.0, 1.0,
	+0.5, -0.5, 0.0, 1.0,
	-0.5, +0.5, 0.0, 1.0,
};
static char* vsSource = "#version 120 \n\
attribute vec4 vertex; \n\
void main(void) {\n\
		gl_Position = vertex;\n\
}";

static char* fsSource = "#version 120 \n\
void main(void) {\n\
gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n\
}";

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
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	// provide the vertex attributes
	loc = glGetAttribLocation(prog, "vertex");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, vertices);
	// draw a triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// flush all
	glFlush();
}

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESCAPE
		exit(0);
		break;
	}
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
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