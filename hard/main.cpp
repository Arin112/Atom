#include <GL/freeglut.h>
#include <algorithm>
#include <cmath>
#include <thread>

#define getOrbitCoord(t, n)                                                    \
	sin(t + n * (3.14 / 5)) * 3, cos(t + n * (3.14 / 5)) * 3,                  \
		sin(t + n * (3.14 / 5)) * 3

void adron(double r, int type) {
	if (type)
		glColor4ub(139, 0, 255, 255);
	else
		glColor4f(1, 0, 0, 1);
	glutSolidSphere(r, 15, 15);
}

void adronAngle(double r, int type) {
	glPushMatrix();
	glTranslatef(0, r, 0);
	adron(r, type);
	glPopMatrix();
}

void core(double r, int type) {
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	adronAngle(r, type);

	glPushMatrix();
	glRotatef(120, 0, 0, 1);
	adronAngle(r, type);
	glPopMatrix();

	glPushMatrix();
	glRotatef(120, 0, 1, 0);
	glRotatef(120, 0, 0, 1);
	adronAngle(r, type);
	glPopMatrix();

	glPushMatrix();
	glRotatef(240, 0, 1, 0);
	glRotatef(120, 0, 0, 1);
	adronAngle(r, type);
	glPopMatrix();
}

void electrons(double t, int z) { // for correct order of transparent render
	glPushMatrix();
	for (int i = 5; i < 15; i++) {
		for (int j = 0; j < 50; j++) {
			double k = t - j / 150.;
			glPushMatrix();
			glRotatef(k * 10 * i, float(i % 2), float((i + 1) % 2),
					  float(i % 3));
			if (j) {
				glEnable(GL_BLEND);
				glDisable(GL_DEPTH_TEST);
				glColor4f(0, 0, 1, 0.1);
			} else {
				glDisable(GL_BLEND);
				glEnable(GL_DEPTH_TEST);
				glColor4f(0, 0, 1, 1);
			}
			glTranslatef(getOrbitCoord(k, i));
			GLfloat matrixMV[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, matrixMV);
			if ((matrixMV[14] > -10 && z) || (matrixMV[14] < -10 && (!z)))
				glutSolidSphere(0.1 - j / 500., j ? 5 : 10, j ? 5 : 10);
			glPopMatrix();
		}
	}
	glPopMatrix();
}
int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(150, 10);
	glutInitWindowSize(700, 700);
	glutCreateWindow("ATOM. press anykey to exit");

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// glAlphaFunc(GL_GREATER, 0.5);
	// glEnable(GL_ALPHA_TEST);

	glutDisplayFunc([]() {}); // empty lambda, do nothing
	glutKeyboardFunc([](unsigned char, int, int) { exit(0); });
	glutReshapeFunc([](int w, int h) {
		if (h == 0)
			h = 1;
		float ratio = 1.0 * w / h;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, w, h);
		gluPerspective(60, ratio, 0.1, 50);
		glMatrixMode(GL_MODELVIEW);
	});

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	GLfloat material_diffuse[] = {1.0, 1.0, 1.0, 1.0};			   // as default
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse); // as default
	GLfloat light0_diffuse[] = {1., 1., 1.};					   // as default
	GLfloat light0_position[] = {0.0, 0.0, 0.0, 1.0};			   // as default
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);			   // as default
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);			   // as default

	glEnable(GL_LIGHT0);

	double t = 0;
	while (1) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0, 0, -10);

		electrons(t, 0);

		glPushMatrix();
		glRotatef(t * 300, 0, 1, 0);
		glRotatef(t * 100, 0, 0, 1);
		core(0.3, 1);
		glRotatef(180, 1, 0, 0);
		glRotatef(60, 0, 1, 0);
		core(0.3, 0);
		glPopMatrix();

		electrons(t, 1);

		glFlush();
		t += 0.01;
		glutMainLoopEvent(); // process system events(window move, resize,
							 // redraw, etc)
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
	return 0;
}
