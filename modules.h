#ifndef MODULES_H_INCLUDED
#define MODULES_H_INCLUDED

#endif // MODULES_H_INCLUDED

#include <iostream>
#include <math.h>
#include<bits/stdc++.h>
#define PI 3.141592

using namespace std;


class Weapon
{
    public:
    GLint name;
    bool attack;

    Weapon(){}
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
        if(attack)
        {
            glColor3f(0.8, 0.8, 0.8);
            glBegin(GL_POLYGON);
                glVertex2f(1.5 * bounds, bounds / 2);
                glVertex2f(1.7 * bounds, bounds / 2 - height / 2);
                glVertex2f(1.6 * bounds, bounds / 2 - height / 2);
            glEnd();
        attack = false;
        }
        else
        {
            glColor3f(0.8, 0.8, 0.8);
            glBegin(GL_POLYGON);
                glVertex2f(1.6 * bounds, bounds / 2);
                glVertex2f(1.8 * bounds, bounds / 2 - height / 2);
                glVertex2f(1.7 * bounds, bounds / 2 - height / 2);
            glEnd();
        }
    }
    if (name == 1)
    {
        glColor3f(0.8, 0.8, 0.8);
        glBegin(GL_POLYGON);
            glVertex2f(1.6 * bounds, bounds / 3);
            glVertex2f(1.7 * bounds, bounds / 3);
            glVertex2f(1.8 * bounds, bounds / 2 - height / 2);
            glVertex2f(1.7 * bounds, bounds / 2 - height / 2);
        glEnd();
    }
}

//Map
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
        this->shape = vector<GLint> {walls.size(), walls[0].size()};
    }
    void show();
};

void Map::show()
{
    for(int i = 0; i < shape[0]; i++)
    {
        for(int j = 0; j < shape[1]; j++)
        {
            if(walls[i][j] == 0)
            {
                glColor3f(0.1, 0.1, 0.1);
            }
            else if(walls[i][j] == 1)
            {
                glColor3f(1, 1 , 1);
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

    Ray(){}
    Ray(vector<GLfloat> pos,vector<GLfloat> dir,  GLfloat angle)
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
    GLfloat atan = -1/tan(angle * (PI / 180)), ntan = -tan(angle * (PI / 180)), distH, distV;
    vector<GLfloat> h(2), offsetH{0, 0}, v(2), offsetV{0, 0}, res(4);
    vector<GLint> mappos;
    GLint dofH = 0, dofV = 0;

    // Horizontal
    // Down
    if(angle > 180)
    {
        h[1] = (((int)pos[1] / m.blockSize) * m.blockSize)-0.0001;
        h[0] = pos[0] + (pos[1] - h[1]) * atan;
        offsetH = vector<GLfloat> {m.blockSize*atan, -m.blockSize};
    }
    // Up
    else if(angle < 180)
    {
        h[1] = (((int)pos[1] / m.blockSize) * m.blockSize) + m.blockSize;
        h[0] = pos[0] + (pos[1] - h[1]) * atan;
        offsetH = vector<GLfloat> {-m.blockSize*atan, m.blockSize};
    }
    else if(angle == 0 || angle == 180)
    {
        h = pos;
        dofH = m.shape[0];
    }

    // Vertical
    // Left
    if(270 > angle && angle > 90)
    {
        v[0] = (((int)pos[0] / m.blockSize) * m.blockSize)-0.0001;
        v[1] = pos[1] + (pos[0] - v[0]) * ntan;
        offsetV = vector<GLfloat> {-m.blockSize, m.blockSize*ntan};
    }
    // Right
    else if(270 < angle || angle < 90)
    {
        v[0] = (((int)pos[0] / m.blockSize) * m.blockSize) + m.blockSize;
        v[1] = pos[1] + (pos[0] - v[0]) * ntan;
        offsetV = vector<GLfloat> {m.blockSize, -m.blockSize*ntan};
    }
    else if(angle == 90 || angle == 270)
    {
        v = pos;
        dofV = m.shape[0];
    }

    // Horizontal
    while(dofH < m.shape[0])
    {
        mappos = vector<GLint> {(int)(h[0]) / m.blockSize, (int)(h[1]) / m.blockSize};
        GLint mloc = mappos[1] * m.shape[0] + mappos[0];
        mappos[0] = mloc % m.shape[0];
        mappos[1] = mloc / m.shape[0];
        if(0 < mloc && mloc < m.shape[0] * m.shape[1] && m.walls[mappos[0]][mappos[1]] == 1)
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
    while(dofV < m.shape[0])
    {
        mappos = vector<GLint> {(int)(v[0]) / m.blockSize, (int)(v[1]) / m.blockSize};
        GLint mloc = mappos[1] * m.shape[0] + mappos[0];
        mappos[0] = mloc % m.shape[0];
        mappos[1] = mloc / m.shape[0];
        if(0 < mloc && mloc < m.shape[0] * m.shape[1] && m.walls[mappos[0]][mappos[1]] == 1)
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
    if(distH < distV)
    {
        res[0] = h[0];
        res[1] = h[1];
        res[2] = sqrt(distH);
        if(angle > 180){res[3] = 0;}
        else{res[3] = 1;}
    }
    else
    {
        res[0] = v[0];
        res[1] = v[1];
        res[2] = sqrt(distV);
        if(270 > angle && angle > 90){res[3] = 2;}
        else{res[3] = 3;}
    }
    return res;
}
void Ray::show()
{
    glBegin(GL_LINES);
    glVertex2f(pos[0], pos[1]);
    glVertex2f(pos[0] + dir[0], pos[1] + dir[1]);
    glEnd();

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

    Player(){} //dummy constructor
    Player(vector<GLfloat> pos, GLfloat speed, const GLint rayCount, GLfloat FOV, GLfloat angle)
    {
        weapons = vector<Weapon>{Weapon(0), Weapon(1)};
        weapon = weapons[0];

        this->pos = pos;
        this->speed = speed;
        this->rayCount = rayCount;
        this->FOV = FOV;
        if(rayCount > 1)
        {
            this->step = this->FOV / (rayCount - 1);
        }
        else if(rayCount == 1)
        {
            this->step = 0;
        }
        this->angle = angle;

        for(int i = 0; i < rayCount; i++)
        {
            GLfloat theta = (angle - FOV/2 + i * step) * (PI / 180);
            if(theta < 0)
            {
                theta += 360;
            }

            if(theta >= 360)
            {
                theta -= 360;
            }
            rays.push_back(Ray(pos, vector<GLfloat>{cos(theta), sin(theta)}, theta));
        }
	}
    void show();
    void actions(bool keybuffer[], GLfloat mousebuffer[], GLfloat bounds);
    vector<vector<GLfloat>> see(Map m);
};
void Player::actions(bool keybuffer[], GLfloat mousebuffer[], GLfloat bounds)
{
    if (keybuffer[' '])
    {
        weapon.attack = true;;
    }
    if (keybuffer['1'])
    {
        weapon = weapons[0];
    }
    if (keybuffer['2'])
    {
        weapon = weapons[1];
    }
    if (keybuffer['w'])
    {
        pos[0] += speed * cos(angle  * (3.14159 / 180));
		pos[1] += speed * sin(angle  * (3.14159 / 180));
	}
	if (keybuffer['a'])
	{
		pos[0] -= speed * sin(angle  * (3.14159 / 180));
		pos[1] += speed * cos(angle  * (3.14159 / 180));
	}
	if (keybuffer['s'])
	{
	    pos[0] -= speed * cos(angle  * (3.14159 / 180));
		pos[1] -= speed * sin(angle  * (3.14159 / 180));
	}
	if (keybuffer['d'])
	{
		pos[0] += speed * sin(angle  * (3.14159 / 180));
		pos[1] -= speed * cos(angle  * (3.14159 / 180));
	}
	if (keybuffer['q'])
    {
        angle += speed ;
    }
    if(keybuffer['e'])
    {
        angle -= speed;
    }
    angle += mousebuffer[0];
    mousebuffer[0] = 0;
    if(angle >= 360)
    {
        angle -= 360;
    }

    else if(angle < 0)
    {
        angle += 360;
    }

	for(int i = 0; i < rayCount; i++)
    {
        rays[i].pos = pos;
        GLfloat theta = angle -(FOV / 2) + i * step;
        if(theta < 0)
        {
            theta += 360;
        }

        if(theta >= 360)
        {
            theta -= 360;
        }
        rays[i].dir[0] = float(cos(theta  * (3.14159 / 180)));
        rays[i].dir[1] = float(sin(theta  * (3.14159 / 180)));
        rays[i].angle = theta;
    }
}
void Player::show()
{
    glBegin(GL_POINTS);
    glVertex2f(pos[0], pos[1]);
    glEnd();
}

vector<vector<GLfloat>> Player::see(Map m)
{
    vector<vector<GLfloat>> distances(4);
    vector<GLfloat> res;

    for(int i = 0; i < this->rayCount; i++)
    {
        res = rays[i].cast(m);
        glBegin(GL_LINES);
        glVertex2f(pos[0], pos[1]);
        glVertex2f(res[0], res[1]);
        glEnd();
        glBegin(GL_POINTS);
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
    GLint type;
    GLint state;
    Sprite(vector<GLfloat> pos)
    {
        this->pos = pos;
        this->state = 1;
        this->type = 0;
    }
    void show(Player p, GLfloat bounds);
};

void Sprite::show(Player p, GLfloat bounds)
{
    if(state)
    {
        vector<GLfloat> relativePos{pos[0] - p.pos[0], pos[1] - p.pos[1]};
        vector<GLfloat> dir{cos(p.angle * PI/180), sin(p.angle * PI / 180)};
        GLfloat a = relativePos[1]*dir[0] + relativePos[0]*dir[1];
        GLfloat b = relativePos[0]*dir[0] - relativePos[1]*dir[1];
        cout<<a<<", "<<b<<endl;
        a = a / b + (3/2 * bounds);
        b = pos[2] / b + bounds / 2;
        glColor3f(1, 1, 0);
        glPointSize(20);
        glBegin(GL_POINTS);
        glVertex2d(a, b);
        glEnd();
        glPointSize(1);

    }
}
