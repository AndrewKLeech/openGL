#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#pragma comment(lib, "glew32.lib")
GLfloat clr[4] = { 0.5F, 0.5F, 0.5F, 1.0F };
void myinit(void) {
	char *strVersion = (char*)glGetString(GL_VERSION);
	printf("version = %s\n", strVersion);
	fflush(stdout);
}
void mydisplay(void) {
	glClearColor(clr[0], clr[1], clr[2], clr[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}
void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q': clr[0] += 0.01F; if (clr[0] > 1.0F) clr[0] = 1.0F; break;
	case 'w': clr[1] += 0.01F; if (clr[1] > 1.0F) clr[1] = 1.0F; break;
	case 'e': clr[2] += 0.01F; if (clr[2] > 1.0F) clr[2] = 1.0F; break;
	case 'a': clr[0] -= 0.01F; if (clr[0] < 0.0F) clr[0] = 0.0F; break;
	case 's': clr[1] -= 0.01F; if (clr[1] < 0.0F) clr[1] = 0.0F; break;
	case 'd': clr[2] -= 0.01F; if (clr[2] < 0.0F) clr[2] = 0.0F; break;
	case 27: exit(0); break;
	default: return;
	}
	glutPostRedisplay();
}


int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitWindowSize(200, 200);
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);
	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}