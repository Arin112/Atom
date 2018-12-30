#include <GL/freeglut.h>
#include <cmath>
#include <unistd.h>

void adron(double x, double y, double r, int type) {
	glColor4f(1, 1, 1, 1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	if (type)
		glColor4ub(139, 0, 255, 200);
	else
		glColor4f(1, 0, 0, 200.f / 255);
	for (double d = 0; d <= 2 * M_PI + 0.01; d += 0.01)
		glVertex2f(x + (r * cos(d)), y + (r * sin(d)));
	glEnd();
}

void core(double r) {
	for (int i = 1; i < 6; i++) {
		adron(r * cos(i * 2 * M_PI / 6), r * sin(i * 2 * M_PI / 6), r, i % 2);
		glTranslated(0, 0, -0.00001f);
	}
	// tricks with rotation and translation are required for overlapping all
	// over all
	glLoadIdentity();
	glRotatef(1, 1, 0, 0);
	adron(r, 0, r, 0);
	glLoadIdentity();
	glTranslatef(0, 0, -0.1f);
	adron(0, 0, r, 1);
	glLoadIdentity();
}

void electron(double x, double y, double r) {
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(1, 1, 1, 1);
	glVertex2f(x, y);
	glColor4f(0, 0, 1, 0.5);
	for (double d = 0; d <= 2 * M_PI; d += 0.01)
		glVertex2f(x + (r * cos(d)), y + (r * sin(d)));
	glEnd();
}

void orbit(double x, double y, double r) {
	glBegin(GL_LINE_LOOP);
	glColor4f(1, 1, 1, 0.5);
	for (double d = 0; d <= 2 * M_PI; d += 0.1)
		glVertex2f(x + (r * cos(d)), y + (r * sin(d)));
	glEnd();
}

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("ATOM simple and nice");

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc([]() {}); // empty lambda, do nothing
	glutKeyboardFunc([](unsigned char, int, int) { exit(0); });

	double t = 0;
	while (1) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		core(0.1);
		for (int i = 4; i < 10; i++) {
			orbit(0, 0, 0.1 * i);
			electron(cos(t / i) * 0.1 * i, sin(t / i) * 0.1 * i, 0.05);
		}
		glFlush();
		t += 0.1;
		glutMainLoopEvent(); // process system events(window move, resize,
							 // redraw, etc)
		usleep(5000);
	}

	return 0;
}
