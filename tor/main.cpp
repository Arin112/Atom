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
  glutCreateWindow("Some OpenGl window");

  glEnable(GL_MULTISAMPLE);

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

  GLfloat light0_position[] = {0.0, 0.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  double t = 0;
  while (1) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0, 1, -10);
    glRotatef(90, 1, 0, 0);
    glColor4f(1, 1, 1, 1);
    // glutSolidTorus(0.5, 1, 20, 20);

    for (int i = 0; i < 5; i++) {
      glPushMatrix();
      glTranslatef(0, 0, (sin(t) * 2 * i / 2 + 4 + i * 2) * 0.3);
      glutSolidTorus(0.3, 0.7 * (sin(t) * i / 2 + 2), 20, 20);
      glPopMatrix();
    }
    glTranslatef(0, 0, 2.5);
    for (int i = 1; i < 5; i++) {
      glPushMatrix();
      glTranslatef(0, 0, -(sin(t) * 2 * i / 2 + 4 + i * 2) * 0.3);
      glutSolidTorus(0.3, 0.7 * (sin(t) * i / 2 + 2), 20, 20);
      glPopMatrix();
    }

    t += 0.01;
    glFlush();
    glutMainLoopEvent();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  return 0;
}
