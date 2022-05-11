#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include "modules.h"
#include "textures.h"

// Parameters
GLfloat bounds = 512;
GLfloat rayCount = 512;
GLfloat sliceWidth = bounds / rayCount;
GLfloat maxHeight = 320;
vector<GLfloat> playerpos{100, 100};
GLfloat playerSpeed = 1;
GLfloat FOV = 60;
vector<GLfloat> mappos{0, 0};
GLfloat blockSize = 64;
string text = "Hello There! Didn't expect to make it out here alive. This  place is crawling with monsters. No one can make it past...";

// Game Objects
vector<vector<GLint>> walls{
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 0, 1, 0, 1, 0, 1},
    {1, 1, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
};
Player p(playerpos, playerSpeed, rayCount, FOV, 90);
Map m(mappos, walls, blockSize);
Sprite s1(vector<GLfloat>{100, 300}, vector<GLfloat>{12, 16}, mage_texture, vector<GLfloat>{69, 69, 69}, 30, text);
Enemy e1(vector<GLfloat>{300, 100}, vector<GLfloat>{24, 24}, swole_texture, vector<GLfloat>{0, 0, 0});
Enemy e2(vector<GLfloat>{400, 100}, vector<GLfloat>{24, 24}, swole_texture, vector<GLfloat>{0, 0, 0});
Hud hud(bounds, maxHeight);
vector<Sprite> sprites {s1};
vector<Enemy> enemies{e1, e2};

// Input buffer
bool keybuffer[256] = {0}, specialkeybuffer[256];
GLint mousebuffer[] = {0, 0, 1, 1, 1, 1, 1}, mouseLoc[] = {bounds, bounds / 2};


// 2.5D rendering
void drawScene(vector<vector<GLfloat>> d, GLint texture_size = 32)
{
    GLfloat sliceHeight, offset, shade;
    glColor3ub(31, 103, 255);
    glBegin(GL_QUADS);
    glVertex2f(bounds, bounds / 2);
    glVertex2f(bounds, bounds / 2 + maxHeight / 2);
    glVertex2f(2 * bounds, bounds / 2 + maxHeight / 2);
    glVertex2f(2 * bounds, bounds / 2);
    glColor3ub(139, 71, 19);
    glBegin(GL_QUADS);
    glVertex2f(bounds, bounds / 2);
    glVertex2f(bounds, bounds / 2 - maxHeight / 2);
    glVertex2f(2 * bounds, bounds / 2 - maxHeight / 2);
    glVertex2f(2 * bounds, bounds / 2);
    glEnd();

    glPointSize(sliceWidth);
    glBegin(GL_POINTS);
    for (int i = 0; i < d[0].size(); i++)
    {
        GLfloat index = d[0].size() - i - 1;

        sliceHeight = 0.5 * m.blockSize * maxHeight / (d[0][index] * cos((p.angle - p.rays[index].angle) * (PI / 180)));
        vector<GLfloat> t{0, 0}, texture_offset{0, 0}, texture_step{0, texture_size / (sliceHeight + 1)};
        if (sliceHeight > maxHeight)
        {
            texture_offset[1] = (sliceHeight - maxHeight) / 2;
            sliceHeight = maxHeight;
        }

        offset = bounds / 2 - sliceHeight / 2;
        t[1] = texture_offset[1] * texture_step[1];
        shade = 1;
        if (d[1][index] <= 1)
        {
            t[0] = (int)(d[2][index] / 2.0) % texture_size;
            if (d[1][index] == 0)
            {
                t[0] = 31 - t[0];
                shade = 0.7;
            }
        }
        else
        {
            shade = 0.7;
            t[0] = (int)(d[3][index] / 2.0) % texture_size;
            if (d[1][index] == 3)
            {
                t[0] = 31 - t[0];
                shade = 0.6;
            }
        }
        for (int j = 1; j <= sliceHeight; j++)
        {
            GLfloat c = texture[(int)t[1] * texture_size + (int)t[0]];
            glColor3f(!c * shade, 0.05 * shade, 0.05 * shade);
            glVertex2f(bounds + i * sliceWidth + sliceWidth / 2, offset - sliceWidth / 2);
            glVertex2f(bounds + i * sliceWidth + sliceWidth / 2, j + offset - sliceWidth / 2);
            t[1] += texture_step[1];
        }
    }
    glEnd();
    glPointSize(1);

    for (int i = 0; i < sprites.size(); i++)
    {
        sprites[i].see(p);
        sprites[i].show(p, bounds, maxHeight, d, 128);
        sprites[i].actions(hud);
    }
    for (int i = 0; i < enemies.size(); i++)
    {
        enemies[i].see(p);
        enemies[i].show(p, bounds, maxHeight, d, 128);
        enemies[i].actions(p, m);
    }
}

// Event loop
void display()
{
    p.actions(keybuffer, specialkeybuffer, mousebuffer, bounds, m);
    glClear(GL_COLOR_BUFFER_BIT);
    m.show();
    drawScene(p.see(m));
    p.show();
    hud.show(p);
    p.weapon.show(bounds, 320);
    glutSwapBuffers();
    cout<<p.health<<endl;
}

// Mechanical functions
void keyUp(unsigned char c, int x, int y)
{
    keybuffer[c] = false;
}
void keyDown(unsigned char c, int x, int y)
{
    keybuffer[c] = true;
}
void specialDown(int c, int x, int y)
{
    specialkeybuffer[c] = true;
}
void specialUp(int c, int x, int y)
{
    specialkeybuffer[c] = false;
}
void mouse(int x, int y)
{
    mousebuffer[5] = mouseLoc[0] - x;
    mousebuffer[6] = mouseLoc[1] - y;
    mouseLoc[0] = x;
    mouseLoc[1] = y;
}
void mouseKeys(int button, int state, int x, int y)
{
    mousebuffer[button] = state;
}

void timer(GLint lassi)
{
    glutPostRedisplay();
    glutTimerFunc(20, timer, 0);
}

void reshape(int w, int h)
{
    if(h == 0)
    {
		h = 1;
    }
	glLoadIdentity();
	GLfloat scale = float(h) / bounds;
    bounds = h;
	sliceWidth = bounds / p.rayCount;
	m.blockSize = bounds / 8;
	hud.bounds = bounds;
	p.pos[0] *= scale;
	p.pos[1] *= scale;

	for(int i = 0; i < sprites.size(); i++)
    {
        sprites[i].pos[0] *= scale;
        sprites[i].pos[1] *= scale;
    }
    for(int i = 0; i < enemies.size(); i++)
    {
        enemies[i].pos[0] *= scale;
        enemies[i].pos[1] *= scale;
    }

    glutReshapeWindow(2 * bounds, bounds);
	glViewport(0, 0, bounds * 2, bounds);
	gluOrtho2D(0, 2 * bounds, 0, bounds);
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
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);    // 2 buffer therefore, smoother
    glutInitWindowSize(2 * bounds, bounds);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Headshot");
    glutDisplayFunc(display);                       // render the graphics
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);                      //this functions handle if a key is pressed or not
    glutKeyboardUpFunc(keyUp);                      //this functions handle if a key is pressed or not
    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);
    glutPassiveMotionFunc(mouse);                   //mouse movement
    glutMouseFunc(mouseKeys);                       //records mouse keys
    glutSetCursor(GLUT_CURSOR_DESTROY);             //cursor design
    init();
    timer(0);

    glutMainLoop();
    return 0;
}
