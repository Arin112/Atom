#include <GL/freeglut.h>
#include <chrono>
#include <cmath>
#include <thread>

void circle(float x, float y, float r) {
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for (double d = 0; d <= 6.5 /* 2*PI */; d += 0.1)
		glVertex2f(x + (r * cos(d)), y + (r * sin(d)));

	glEnd();
}

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(/*GLUT_DEPTH | GLUT_DOUBLE |*/ GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("ATOM simple. press anykey to exit");

	glutDisplayFunc([]() {}); // empty lambda, do nothing
	glutKeyboardFunc([](unsigned char, int, int) { exit(0); });

	double t = 0;
	while (1) {
		glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1, 1, 0);
		circle(0, 0, 0.1); // core
		glColor3f(0, 0, 1);
		for (int i = 2; i < 10; i++)
			circle(cos(t / i) * 0.1 * i, sin(t / i) * 0.1 * i, 0.05);
		glFlush();
		// glutSwapBuffers();
		t += 0.1;
		glutMainLoopEvent(); // process system events(window move, resize,
							 // redraw, etc)
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}
