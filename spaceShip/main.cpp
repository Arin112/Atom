#include <GL/freeglut.h>
#include <chrono>
#include <cmath>
#include <thread>

using namespace std;

void circle(double x, double y, double r) {
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x, y);
  for (double d = -0.1; d <= 2 * M_PI; d += 0.1)
    glVertex2f(x + (r * cos(d)), y + (r * sin(d)));
  glEnd();
}

void ring(double x, double y, double r) {
  glBegin(GL_LINE_LOOP);
  for (double d = 0; d <= 2 * M_PI; d += 0.1)
    glVertex2f(x + r * sin(d), y + r * cos(d));
  glEnd();
}
int s = 2;

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_MULTISAMPLE);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Some OpenGl window");

  glEnable(GL_MULTISAMPLE);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glutDisplayFunc([]() {}); // empty lambda, do nothing

  glutKeyboardFunc([](unsigned char ch, int, int) {
    if (s == 2)
      s = 1;
    else if (s == 3)
      s = 4;
    if (ch == 'r')
      s = 2;
  });

  double t = 0;
  double x = sin(t * 1.5 + 0.5) * 0.7 + 0.07;
  double y = cos(t * 1.5 + 0.5) * 0.7 + 0.07;
  double vx = 0, vy = 0;

  while (1) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 0);
    circle(0, 0, 0.1);

    glColor3f(1, 1, 1);
    ring(0, 0, 0.4);
    glColor3f(0, 1, 0);
    circle(sin(t) * 0.4, cos(t) * 0.4, 0.09);

    glColor3f(1, 1, 1);
    ring(0, 0, 0.7);
    glColor3f(0, 0, 1);
    circle(sin(t * 1.5 + 0.5) * 0.7, cos(t * 1.5 + 0.5) * 0.7, 0.07);
    if (s == 1) {
      x += vx * 0.01;
      y += vy * 0.01;
      vx = 1.0 * vx - (x - sin(t) * 0.4) * 0.03;
      vy = 1.0 * vy - (y - cos(t) * 0.4) * 0.03;
      if ((x - sin(t) * 0.4) * (x - sin(t) * 0.4) +
              (y - cos(t) * 0.4) * (y - cos(t) * 0.4) <
          0.02 * 0.02 + 0.1 * 0.1) {
        s = 3;
      } else if (x * x + y * y < 0.1 * 0.1 + 0.02 * 0.02)
        s = 2;
    } else if (s == 2) {
      x = sin(t * 1.5 + 0.5) * 0.7 + 0.07;
      y = cos(t * 1.5 + 0.5) * 0.7 + 0.07;
      vx = 1.5 * cos(t * 1.5 + 0.5) * 0.7;
      vy = -1.5 * sin(t * 1.5 + 0.5) * 0.7;
    } else if (s == 3) {
      x = sin(t) * 0.4 + 0.09;
      y = cos(t) * 0.4 + 0.09;
      vx = cos(t) * 0.4;
      vy = -sin(t) * 0.4;
    } else if (s == 4) {
      x += vx * 0.01;
      y += vy * 0.01;
      vx = 1.0 * vx - (x - sin(t * 1.5 + 0.5) * 0.7) * 0.03;
      vy = 1.0 * vy - (y - cos(t * 1.5 + 0.5) * 0.7) * 0.03;
      if ((x - sin(t * 1.5 + 0.5) * 0.7) * (x - sin(t * 1.5 + 0.5) * 0.7) +
              (y - cos(t * 1.5 + 0.5) * 0.7) * (y - cos(t * 1.5 + 0.5) * 0.7) <
          0.02 * 0.02 + 0.1 * 0.1) {
        s = 2;
      } else if (x * x + y * y < 0.1 * 0.1 + 0.02 * 0.02)
        s = 1;
    }
    glColor3f(0, 1, 1);
    circle(x, y, 0.02);
    t += 0.01;
    glFlush();
    glutMainLoopEvent();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  return 0;
}
