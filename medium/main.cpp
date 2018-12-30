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
	for (double d = 0; d <= 2 * M_PI + 0.1; d += 0.1)
		glVertex2f(x + (r * cos(d)), y + (r * sin(d)));
	glEnd();
}

void core(double r) {
	glPushMatrix();
	for (int i = 1; i < 6; i++) {
		adron(r * cos(i * 2 * M_PI / 6), r * sin(i * 2 * M_PI / 6), r, i % 2);
		glTranslated(0, 0, -0.00001f);
	}
	// tricks with rotation and translation are required for overlapping all
	// over all
	glPopMatrix();
	glPushMatrix();
	glRotatef(1, 1, 0, 0);
	adron(r, 0, r, 0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, -0.1f);
	adron(0, 0, r, 1);
	glPopMatrix();
}

void electron(double x, double y, double r, int n = 0) {
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(1, 1, 1, 1 - n / 100.);
	glVertex2f(x, y);
	glColor4f(0, 0, 1, 0.5 - n / 100.);
	for (double d = -0.1; d <= 2 * M_PI; d += 0.1)
		glVertex2f(x + (r * cos(d)), y + (r * sin(d)));
	glEnd();
}

void orbit(double x, double y, double r) {
	glBegin(GL_LINE_LOOP);
	glColor4f(1, 1, 1, 0.5);
	for (double d = 0; d <= 2 * M_PI; d += 0.1)
		glVertex2f(x + (r * sin(d + 1.15)), y + (r * cos(d - 1.15)));
	glEnd();
}

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("ATOM");

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc([]() {}); // empty lambda, do nothing
	glutKeyboardFunc(
		[](unsigned char, int, int) { exit(0); }); // anykey for exit

	double t = 0;
	while (1) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glRotatef(t * 30, 0, 0, 1);
		core(0.1);
		glRotatef(t * 30, 0, 0, -1);
		glRotatef(-t * 30, 0, 0, 1);

		for (int j = 0; j < 5; j++) {
			for (int i = 0; i < 50; i++) {
				electron(sin(t - i / 50. + 1.15 + j * (3.14 / 5)) * 0.5,
						 cos(t - i / 50. - 1.15 + j * (3.14 / 5)) * 0.5, 0.05,
						 i);
			}
			orbit(0, 0, 0.5);
			glRotatef(36 * 2, 0, 0, 1);
		}

		glFlush();
		t += 0.01;
		glutMainLoopEvent(); // process system events(window move, resize,
							 // redraw, etc)
		usleep(5000);
	}
	return 0;
}
