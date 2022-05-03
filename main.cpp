#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include "modules.h"
#include "textures.h"

// Game Objects
vector<vector<GLint>> walls{
{1, 1, 1, 1, 1, 1, 1, 1},
{1, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 1, 1, 1, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 1},
{1, 0, 0, 1, 1, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 1},
{1, 1, 1, 1, 1, 1, 1, 1},
};
Player p(vector<GLfloat>{100, 100}, 1, 512, 60, 90);
Map m(vector<GLfloat>{0, 0}, walls, 64);

GLfloat bounds = 512, sliceWidth = bounds / p.rayCount;
bool keybuffer[256] = {0};
vector<GLfloat> mousebuffer{0, 0}, mouseLoc{bounds, bounds/2};

// 2.5D rendering
void drawScene(vector<vector<GLfloat>> d, GLfloat heightMax=320, GLint texture_size=32)
{
    GLfloat sliceHeight, offset, shade;
    glColor3ub(51, 153, 255);
    glBegin(GL_QUADS);
    glVertex2f(bounds, bounds / 2);
    glVertex2f(bounds, bounds / 2 + heightMax / 2);
    glVertex2f(2 * bounds, bounds / 2 + heightMax / 2);
    glVertex2f(2 * bounds, bounds / 2);
    glColor3ub(139, 71, 19);
    glBegin(GL_QUADS);
    glVertex2f(bounds, bounds / 2);
    glVertex2f(bounds, bounds / 2 - heightMax / 2);
    glVertex2f(2 * bounds, bounds / 2 - heightMax / 2);
    glVertex2f(2 * bounds, bounds / 2);
    glEnd();

    glPointSize(sliceWidth);
    glBegin(GL_POINTS);
    for(int i = 0; i < d[0].size() ; i++)
    {
        GLfloat index =  d[0].size() - i - 1;

        sliceHeight = m.blockSize * heightMax / (d[0][index] * cos((p.angle - p.rays[index].angle) * (PI / 180)));
        vector<GLfloat> t{0, 0}, texture_offset{0, 0}, texture_step{0, texture_size / (sliceHeight + 1)};
        if(sliceHeight > heightMax)
        {
            texture_offset[1] = (sliceHeight - heightMax) / 2;
            sliceHeight = heightMax;
        }

        offset = bounds/2 - sliceHeight / 2;
        t[1] = texture_offset[1] * texture_step[1];
        shade = 0.8;
        if(d[1][index] <= 1)
        {
            t[0] = (int)(d[2][index] / 2.0) % texture_size;
            if(d[1][index] == 0)
            {
                t[0] = 31 - t[0];
                shade = 1;
            }
        }
        else
        {
            shade = 0.8;
            t[0] = (int)(d[3][index] / 2.0) % texture_size;
            if(d[1][index] == 3)
            {
                t[0] = 31 - t[0];
                shade = 1;
            }
        }
        for(int j = 1; j <= sliceHeight; j++)
        {
            GLfloat c = texture[(int)t[1]*texture_size + (int)t[0]] * shade;
            glColor3f(c, c, c);
            glVertex2f(bounds + i * sliceWidth + sliceWidth/2, offset - sliceWidth/2);
            glVertex2f(bounds + i * sliceWidth + sliceWidth/2, j + offset - sliceWidth/2);
            t[1] += texture_step[1];
        }

    }
    glEnd();
    glPointSize(1);
}

// HUD
void HUD()
{
    // Crosshair
    glLineWidth(2);
    glBegin(GL_LINES);
        glColor3f(0, 1, 0);
        glVertex2f(1.5 * bounds - 10, bounds / 2);
        glVertex2f(1.5 * bounds + 10, bounds / 2);
        glColor3f(0, 1, 0);
        glVertex2f(1.5 * bounds, bounds / 2 - 10);
        glVertex2f(1.5 * bounds, bounds / 2 + 10);
    glEnd();
    glLineWidth(1);
    //p.weapon.show();
}

// Event loop
void display()
{
    p.actions(keybuffer, mousebuffer, bounds);
    glClear(GL_COLOR_BUFFER_BIT);

    m.show();
    p.show();
    drawScene(p.see(m));
    HUD();

    glutSwapBuffers();
}

// Mechanical functions
void keyUp(unsigned char c, int x, int y)
{
    keybuffer[c] = true;
}
void keyDown(unsigned char c, int x, int y)
{
    keybuffer[c] = false;
}
void mouse(int x, int y)
{
    mousebuffer[0] = mouseLoc[0] - x;
    mousebuffer[1] = mouseLoc[1] - y;
    mouseLoc[0] = x;
    mouseLoc[1] = y;
}

void timer(GLint lassi)
{
    glutPostRedisplay();
    glutTimerFunc(20, timer, 0);
}

// Boilerplate
void init()
{
    glClearColor(0, 0, 0, 1);
    glPointSize(4);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluOrtho2D(0, 2 * bounds, 0, bounds);
}

int main(GLint argc, char **argv)
{
    glutInit(&argc, argv );
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(2 * bounds, bounds);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Purgatory");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyUp);
    glutKeyboardUpFunc(keyDown);
    glutPassiveMotionFunc(mouse);
    init();
    timer(0);

    glutMainLoop();
    return 0;
}
