#include <GL/freeglut.h>
#include <chrono>
#include <cmath>
#include <thread>

using namespace std;

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_MULTISAMPLE);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Atom 3D easy. press any key to exit");

  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);
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

  GLfloat light0_diffuse[] = {1., 1., 1.};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);

  GLfloat light0_position[] = {4.0, 4.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  glEnable(GL_LIGHT0);

  double t = 0;
  while (1) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0, 0, -10);
    glRotatef(10 * t, 0, 1, 0); // slowly rotate atom
    glRotatef(5 * t, 1, 0, 0);

    glutSolidSphere(1, 20, 20); // draw core

    for (int i = 0; i < 10; i++) {
      glPushMatrix();
      glRotatef(18 * i, 1, 0, 0);
      glutSolidTorus(0.02, 3, 5, 40); // draw electron orbit
      glTranslatef(sin(t + i) * 3, cos(t + i) * 3, 0);
      glutSolidSphere(0.3, 10, 10); // draw electron
      glPopMatrix();
    }

    t += 0.01;
    glFlush();
    glutMainLoopEvent();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  return 0;
}
