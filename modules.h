#ifndef MODULES_H_INCLUDED
#define MODULES_H_INCLUDED

#endif // MODULES_H_INCLUDED

#include <iostream>
#include <math.h>
#include <bits/stdc++.h>
#define PI 3.141592

using namespace std;

// prototypes

class Player;
class Weapon;
class Sprite;
class Map;
class Ray;
class Hud;

class Weapon
{
public:
    GLint name;
    bool attack;

    Weapon() {}
    Weapon(GLint name)
    {
        this->name = name;
        this->attack = false;
    }

    void show(GLfloat bounds, GLfloat height);
};

void Weapon::show(GLfloat bounds, GLfloat height)
{
    if (name == 0)
    {
        if (attack)
        {
            glBegin(GL_QUADS);
            glColor3f(0.8, 0.8, 0.8);
            glVertex2f(1.5 * bounds, bounds / 2);
            glVertex2f(1.5 * bounds + 30, bounds / 2 - 20);
            glVertex2f(1.7 * bounds, bounds / 2 - height / 2);
            glVertex2f(1.64 * bounds, bounds / 2 - height / 2);
            glColor3f(0.6, 0.6, 0.6);
            glVertex2f(1.5 * bounds, bounds / 2 - 50);
            glVertex2f(1.5 * bounds, bounds / 2);
            glVertex2f(1.64 * bounds, bounds / 2 - height / 2);
            glVertex2f(1.6 * bounds, bounds / 2 - height / 2);
            glEnd();
            attack = false;
        }
        else
        {
            glBegin(GL_QUADS);
            glColor3f(0.8, 0.8, 0.8);
            glVertex2f(1.6 * bounds, bounds / 2);
            glVertex2f(1.6 * bounds + 30, bounds / 2 - 20);
            glVertex2f(1.8 * bounds, bounds / 2 - height / 2);
            glVertex2f(1.74 * bounds, bounds / 2 - height / 2);
            glColor3f(0.6, 0.6, 0.6);
            glVertex2f(1.6 * bounds, bounds / 2 - 50);
            glVertex2f(1.6 * bounds, bounds / 2);
            glVertex2f(1.74 * bounds, bounds / 2 - height / 2);
            glVertex2f(1.7 * bounds, bounds / 2 - height / 2);
            glEnd();
        }
    }
    if (name == 1)
    {
        if (attack)
        {

            glColor3f(0.8, 0.8, 0);
            glBegin(GL_POLYGON);
            glVertex2f(1.7 * bounds - 45, bounds / 2 - height / 2 + 60);
            glVertex2f(1.7 * bounds - 75, bounds / 2 - height / 2 + 90);
            glVertex2f(1.7 * bounds - 105, bounds / 2 - height / 2 + 90);
            glVertex2f(1.7 * bounds - 90, bounds / 2 - height / 2 + 55);
            glVertex2f(1.7 * bounds - 50, bounds / 2 - height / 2 + 40);
            glEnd();
            glColor3f(0.1, 0.1, 0.1);
            glBegin(GL_POLYGON);
            glVertex2f(1.7 * bounds, bounds / 2 - height / 2);
            glVertex2f(1.7 * bounds, bounds / 2 - height / 2 + 30);
            glVertex2f(1.7 * bounds - 5, bounds / 2 - height / 2 + 40);
            glVertex2f(1.7 * bounds - 20, bounds / 2 - height / 2 + 40);
            glVertex2f(1.7 * bounds - 25, bounds / 2 - height / 2 + 30);
            glVertex2f(1.7 * bounds - 25, bounds / 2 - height / 2);
            glEnd();
            glColor3f(0.1, 0.1, 0.1);
            glBegin(GL_POLYGON);
            glVertex2f(1.7 * bounds - 5, bounds / 2 - height / 2 + 40);
            glVertex2f(1.7 * bounds - 45, bounds / 2 - height / 2 + 60);
            glVertex2f(1.7 * bounds - 50, bounds / 2 - height / 2 + 40);
            glVertex2f(1.7 * bounds - 25, bounds / 2 - height / 2 + 20);
            glEnd();
            attack = false;
        }
        else
        {
            glColor3f(0.1, 0.1, 0.1);
            glBegin(GL_POLYGON);
            glVertex2f(1.7 * bounds, bounds / 2 - height / 2);
            glVertex2f(1.7 * bounds, bounds / 2 - height / 2 + 30);
            glVertex2f(1.7 * bounds - 5, bounds / 2 - height / 2 + 40);
            glVertex2f(1.7 * bounds - 20, bounds / 2 - height / 2 + 40);
            glVertex2f(1.7 * bounds - 25, bounds / 2 - height / 2 + 30);
            glVertex2f(1.7 * bounds - 25, bounds / 2 - height / 2);
            glEnd();
            glColor3f(0.1, 0.1, 0.1);
            glBegin(GL_POLYGON);
            glVertex2f(1.7 * bounds - 5, bounds / 2 - height / 2 + 40);
            glVertex2f(1.7 * bounds - 45, bounds / 2 - height / 2 + 60);
            glVertex2f(1.7 * bounds - 50, bounds / 2 - height / 2 + 40);
            glVertex2f(1.7 * bounds - 25, bounds / 2 - height / 2 + 20);
            glEnd();
        }
    }
}

// Map
class Map
{
public:
    GLint blockSize;
    vector<vector<GLint>> walls;
    vector<GLint> shape;
    vector<GLfloat> pos;
    Map(vector<GLfloat> pos, vector<vector<GLint>> walls, GLint blockSize)
    {
        this->pos = pos;
        this->blockSize = blockSize;
        this->walls = walls;
        this->shape = vector<GLint>{walls.size(), walls[0].size()};
    }
    void show();
};

void Map::show()
{
    for (int i = 0; i < shape[0]; i++)
    {
        for (int j = 0; j < shape[1]; j++)
        {
            if (walls[i][j] == 0)
            {
                glColor3f(0.1, 0.1, 0.1);
            }
            else if (walls[i][j] == 1)
            {
                glColor3f(1, 1, 1);
            }
            glBegin(GL_QUADS);
            glVertex2f(pos[0] + i * blockSize + 1, pos[1] + j * blockSize + 1);
            glVertex2f(pos[0] + (i + 1) * blockSize - 1, pos[1] + j * blockSize + 1);
            glVertex2f(pos[0] + (i + 1) * blockSize - 1, pos[1] + (j + 1) * blockSize - 1);
            glVertex2f(pos[0] + i * blockSize + 1, pos[1] + (j + 1) * blockSize - 1);
            glEnd();
        }
    }
}

// Individual raycasts
class Ray
{
public:
    vector<GLfloat> pos;
    vector<GLfloat> dir;
    GLfloat angle;

    Ray() {}
    Ray(vector<GLfloat> pos, vector<GLfloat> dir, GLfloat angle)
    {
        this->pos = pos;
        this->angle = angle;
        this->dir = dir;
    }
    void show();
    vector<GLfloat> cast(Map m);
};

vector<GLfloat> Ray::cast(Map m)
{
    GLfloat atan = -1 / tan(angle * (PI / 180)), ntan = -tan(angle * (PI / 180)), distH, distV;
    vector<GLfloat> h(2), offsetH{0, 0}, v(2), offsetV{0, 0}, res(4);
    vector<GLint> mappos;
    GLint dofH = 0, dofV = 0;

    // Horizontal
    // Down
    if (angle > 180)
    {
        h[1] = (((int)pos[1] / m.blockSize) * m.blockSize) - 0.0001;
        h[0] = pos[0] + (pos[1] - h[1]) * atan;
        offsetH = vector<GLfloat>{float(m.blockSize * atan), float(-m.blockSize)};
    }
    // Up
    else if (angle < 180)
    {
        h[1] = (((int)pos[1] / m.blockSize) * m.blockSize) + m.blockSize;
        h[0] = pos[0] + (pos[1] - h[1]) * atan;
        offsetH = vector<GLfloat>{float(-m.blockSize * atan), float(m.blockSize)};
    }
    else if (angle == 0 || angle == 180)
    {
        h = pos;
        dofH = m.shape[0];
    }

    // Vertical
    // Left
    if (270 > angle && angle > 90)
    {
        v[0] = (((int)pos[0] / m.blockSize) * m.blockSize) - 0.0001;
        v[1] = pos[1] + (pos[0] - v[0]) * ntan;
        offsetV = vector<GLfloat>{float(-m.blockSize), float(m.blockSize * ntan)};
    }
    // Right
    else if (270 < angle || angle < 90)
    {
        v[0] = (((int)pos[0] / m.blockSize) * m.blockSize) + m.blockSize;
        v[1] = pos[1] + (pos[0] - v[0]) * ntan;
        offsetV = vector<GLfloat>{float(m.blockSize), float(-m.blockSize * ntan)};
    }
    else if (angle == 90 || angle == 270)
    {
        v = pos;
        dofV = m.shape[0];
    }

    // Horizontal
    while (dofH < m.shape[0])
    {
        mappos = vector<GLint>{(int)(h[0]) / m.blockSize, (int)(h[1]) / m.blockSize};
        GLint mloc = mappos[1] * m.shape[0] + mappos[0];
        mappos[0] = mloc % m.shape[0];
        mappos[1] = mloc / m.shape[0];
        if (0 < mloc && mloc < m.shape[0] * m.shape[1] && m.walls[mappos[0]][mappos[1]] == 1)
        {
            dofH = m.shape[0];
        }
        else
        {
            h[0] += offsetH[0];
            h[1] += offsetH[1];
            dofH++;
        }
    }

    // Vertical
    while (dofV < m.shape[0])
    {
        mappos = vector<GLint>{(int)(v[0]) / m.blockSize, (int)(v[1]) / m.blockSize};
        GLint mloc = mappos[1] * m.shape[0] + mappos[0];
        mappos[0] = mloc % m.shape[0];
        mappos[1] = mloc / m.shape[0];
        if (0 < mloc && mloc < m.shape[0] * m.shape[1] && m.walls[mappos[0]][mappos[1]] == 1)
        {
            dofV = m.shape[0];
        }
        else
        {
            v[0] += offsetV[0];
            v[1] += offsetV[1];
            dofV++;
        }
    }

    distH = (h[0] - pos[0]) * (h[0] - pos[0]) + (h[1] - pos[1]) * (h[1] - pos[1]);
    distV = (v[0] - pos[0]) * (v[0] - pos[0]) + (v[1] - pos[1]) * (v[1] - pos[1]);
    if (distH < distV)
    {
        res[0] = h[0];
        res[1] = h[1];
        res[2] = sqrt(distH);
        if (angle > 180)
        {
            res[3] = 0;
        }
        else
        {
            res[3] = 1;
        }
    }
    else
    {
        res[0] = v[0];
        res[1] = v[1];
        res[2] = sqrt(distV);
        if (270 > angle && angle > 90)
        {
            res[3] = 2;
        }
        else
        {
            res[3] = 3;
        }
    }
    return res;
}
void Ray::show()
{
    glBegin(GL_LINES);
    glColor3b(255, 255, 0);
    glVertex2f(pos[0], pos[1]);
    glVertex2f(pos[0] + dir[0], pos[1] + dir[1]);
    glEnd();
}

class Hud
{
public:
    GLfloat bounds;
    GLfloat height;
    GLint timer;
    string text;

    Hud(GLfloat bounds, GLfloat height)
    {
        this->bounds = bounds;
        this->height = height;
        this->text = "";
        this->timer = 0;
    }
    void show(GLint maxLength, GLfloat offset);
    void displayDialogue(string text);
};

void Hud::displayDialogue(string text)
{
    this->text = text;
    this->timer = 50;
}

void Hud::show(GLint maxLength = 60, GLfloat offset = 20)
{
    // height = bounds, width = 2 *bounds
    // crosshair
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

//    cout<<textbox<<endl;

    if(timer)
    {
        // textbox
        cout<<"why have you forsaken me";
        glBegin(GL_QUADS);
        glColor3ub(223, 54, 45);
        glVertex2f(bounds, 0);
        glVertex2f(bounds * 2, 0);
        glVertex2f(bounds * 2, bounds / 2 - height / 2);
        glVertex2f(bounds, bounds / 2 - height / 2);
        glEnd();

        // textbox boundary
        glLineWidth(4);
        glBegin(GL_LINE_LOOP);
        glColor3ub(0, 0, 0);
        glVertex2f(bounds + 2, 2);
        glVertex2f(bounds * 2 - 2, 2);
        glVertex2f(bounds * 2 - 2, bounds / 2 - height / 2 - 2);
        glVertex2f(bounds + 2, bounds / 2 - height / 2 - 2);
        glEnd();

        //text
        glColor3ub(0, 0, 0);
        for (int j = 0; j < (text.length() / maxLength) + 1; j++)
        {
            glRasterPos2f(bounds + offset, ((bounds / 2 - height / 2) - (j + 1) * 20));
            for (int i = 0; i < maxLength; i++)
            {
                if (text[(j * maxLength) + i] == '\0')
                {
                    break;
                }
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[(j * maxLength) + i]);
            }
        }
        timer--;
    }

    // equipped weapon
    glBegin(GL_LINE_LOOP);
    glColor3ub(255, 255, 0);
    glVertex2f(bounds, bounds / 2 + height / 2);
    glVertex2f(1.5 * bounds, bounds / 2 + height / 2);
    glVertex2f(1.5 * bounds, bounds);
    glVertex2f(bounds, bounds);
    glEnd();

    // stats
    // face
    // sexy design
    glColor3ub(255, 255, 255);
    glLineWidth(1);
}

// Player
class Player
{
public:
    vector<GLfloat> pos;
    vector<Weapon> weapons;
    Weapon weapon;
    GLfloat speed;
    GLfloat FOV;
    GLfloat angle;
    GLint rayCount;
    GLfloat step;
    vector<Ray> rays;

    Player() {} // dummy constructor
    Player(vector<GLfloat> pos, GLfloat speed, const GLint rayCount, GLfloat FOV, GLfloat angle)
    {
        weapons = vector<Weapon>{Weapon(0), Weapon(1)};
        weapon = weapons[0];

        this->pos = pos;
        this->speed = speed;
        this->rayCount = rayCount;
        this->FOV = FOV;

        if (rayCount > 1)
        {
            this->step = this->FOV / (rayCount - 1);
        }
        else if (rayCount == 1)
        {
            this->step = 0;
        }
        this->angle = angle;

        for (int i = 0; i < rayCount; i++)
        {
            GLfloat theta = (angle - FOV / 2 + i * step) * (PI / 180);
            if (theta < 0)
            {
                theta += 360;
            }

            if (theta >= 360)
            {
                theta -= 360;
            }
            rays.push_back(Ray(pos, vector<GLfloat>{cos(theta), sin(theta)}, theta));
        }
    }
    void show();
    void actions(bool keybuffer[], GLint mousebuffer[], GLfloat bounds, Map m);
    vector<vector<GLfloat>> see(Map m);
};


void Player::actions(bool keybuffer[], GLint mousebuffer[], GLfloat bounds, Map m)
{
    // Use action
//    if(keybuffer['f'])
//    {
//
//    }

    // Mouse Left Click
    if(mousebuffer[0] == 0)
    {
        weapon.attack = true;
        mousebuffer[0] = 1;
    }

    // Weapon switching
    if (keybuffer['1'])
    {
        weapon = weapons[0];
    }
    if (keybuffer['2'])
    {
        weapon = weapons[1];
    }

    // WASD movement
    GLfloat offset = 10;
    float dx = 0, dy = 0;

    if (keybuffer['w'])
    {
        dx += speed * cos(angle * (PI / 180));
        dy += speed * sin(angle * (PI / 180));
    }
    if (keybuffer['a'])
    {
        dx += -speed * sin(angle * (PI / 180));
        dy += speed * cos(angle * (PI / 180));
    }
    if (keybuffer['s'])
    {
        dx += -speed * cos(angle * (PI / 180));
        dy += -speed * sin(angle * (PI / 180));
    }
    if (keybuffer['d'])
    {
        dx += speed * sin(angle * (PI / 180));
        dy += -speed * cos(angle * (PI / 180));
    }
    if (m.walls[(pos[0] + dx * offset) / m.blockSize][pos[1] / m.blockSize] != 1)
    {
        pos[0] += dx;
    }
    if (m.walls[pos[0] / m.blockSize][(pos[1] + dy * offset) / m.blockSize] != 1)
    {
        pos[1] += dy;
    }

    // Rotation???
    if (keybuffer['q'])
    {
        angle += speed;
    }
    if (keybuffer['e'])
    {
        angle -= speed;
    }

    // Real rotation
    angle += mousebuffer[5];
    mousebuffer[5] = 0;
    if (angle >= 360)
    {
        angle -= 360;
    }
    else if (angle < 0)
    {
        angle += 360;
    }

    // Moving the rays
    for (int i = 0; i < rayCount; i++)
    {
        rays[i].pos = pos;
        GLfloat theta = angle - (FOV / 2) + i * step;
        if (theta < 0)
        {
            theta += 360;
        }

        if (theta >= 360)
        {
            theta -= 360;
        }
        rays[i].dir[0] = float(cos(theta * (3.14159 / 180)));
        rays[i].dir[1] = float(sin(theta * (3.14159 / 180)));
        rays[i].angle = theta;
    }
}
void Player::show()
{
    glPointSize(4);
    glColor3f(0.2, 0.2, 1);
    glBegin(GL_POINTS);
    glVertex2f(pos[0], pos[1]);
    glEnd();

}

vector<vector<GLfloat>> Player::see(Map m)
{
    vector<vector<GLfloat>> distances(4);
    vector<GLfloat> res;

    for (int i = 0; i < this->rayCount; i++)
    {
        res = rays[i].cast(m);
        glColor3f(1, 1, 0);
        glBegin(GL_LINES);
        glVertex2f(pos[0], pos[1]);
        glVertex2f(res[0], res[1]);
        glEnd();

        distances[0].push_back(res[2]);
        distances[1].push_back(res[3]);
        distances[2].push_back(res[0]);
        distances[3].push_back(res[1]);
    }
    return distances;
}

class Sprite
{
public:
    vector<GLfloat> pos;
    vector<GLfloat> shape;
    string dialogue;
    GLfloat threshold;
    bool proximity;
    bool visible;

    Sprite() {}
    Sprite(vector<GLfloat> pos, vector<GLfloat> shape, GLfloat threshold, string dialogue)
    {
        this->pos = pos;
        this->shape = shape;
        this->threshold = threshold;
        this->proximity = false;
        this->dialogue = dialogue;
        this->visible = false;
    }
    void show(Player p, GLfloat bounds, GLfloat maxHeight, vector<vector<GLfloat>> distances);
    void actions(Hud hud);
    void see(Player p);
};

void Sprite::show(Player p, GLfloat bounds, GLfloat maxHeight, vector<vector<GLfloat>> distances)
{
    // Render sprite on screen
    vector<GLfloat> relativePos{pos[0] - p.pos[0], pos[1] - p.pos[1]};
    GLfloat dist = sqrt(relativePos[0] * relativePos[0] + relativePos[1] * relativePos[1]);
    GLfloat theta = atan(relativePos[1] / relativePos[0]) / PI * 180;
    if (relativePos[0] < 0)
    {
        theta += 180;
    }
    theta -= p.angle;
    if (theta < -180)
    {
        theta += 360;
    }

    if (theta >= 180)
    {
        theta -= 360;
    }

    visible = false;
    glPointSize(1);
    GLfloat width = shape[0] * 6 * 64 / dist;
    GLfloat height = shape[1] * 6 * 64 / dist;
    GLfloat x = 1.5 * bounds - sin(theta * PI / 180) * bounds - width / 2, y = bounds / 2 - height / 2;
    for(int i = x; i < x + width; i++)
    {
        for(int j = y; j < y + height; j++)
        {
            if (theta < p.FOV / 2 && theta > -p.FOV / 2 && dist > 5  && distances[0][(int)(p.rayCount * (p.FOV / 2 + theta) / p.FOV)] > dist)
            {
                if(bounds < i && i < 2 * bounds && bounds / 2 -maxHeight / 2 < j && j < bounds / 2 + maxHeight / 2)
                {
                    glBegin(GL_POINTS);
                    glColor3f(0, 1, 0);
                    glVertex2d(i, j);
                    glEnd();
                    visible = true;
                }
            }
        }
    }

    // Draw sprite on the mini-map
    glColor3f(0, 1, 0);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex2i(pos[0], pos[1]);
    glEnd();
    glPointSize(1);
}
void Sprite::actions(Hud hud)
{
    if (proximity && visible)
    {
        hud.displayDialogue(dialogue);
    }
}
void Sprite::see(Player p)
{
    GLfloat dist = sqrt(pow(p.pos[0] - pos[0], 2) + pow(p.pos[1] - pos[1], 2));
    proximity = (dist < threshold);
}

class Enemy: public Sprite
{
    public:
    GLfloat speed;
    Enemy(vector<GLfloat> pos, vector<GLfloat> shape, GLfloat threshold, GLfloat speed=1)
    {
        this->pos = pos;
        this->shape = shape;
        this->threshold = threshold;
        this->speed = speed;
        this->proximity = false;
    }
    void actions(Player p, Map m);
};

void Enemy::actions(Player p, Map m)
{
    if(proximity)
    {
        GLfloat dx = p.pos[0] - pos[0];
        GLfloat dy = p.pos[1] - pos[1];
        GLfloat dist = sqrt(dx* dx + dy * dy), offset = 10;
        if(dist)
        {
            dx /= dist;
            dy /= dist;
//            cout<<dy<<" "<<m.walls[pos[0] / m.blockSize][(pos[1] + dy * offset) / m.blockSize]<<endl;
            if(m.walls[(pos[0] + dx *offset) / m.blockSize][pos[1] / m.blockSize] == 0)
            {
                pos[0] += dx * speed;
            }
            if(m.walls[pos[0] / m.blockSize][(pos[1] + dy * offset) / m.blockSize] == 0)
            {
                pos[1] += dy * speed;
            }
        }

    }
}
