#ifndef MODULES_H_INCLUDED
#define MODULES_H_INCLUDED

#endif // MODULES_H_INCLUDED

#include <iostream>
#include <math.h>
#include<bits/stdc++.h>

using namespace std;


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
                glVertex2f(pos[0] + i * 64 + 1, pos[1] - j * 64 - 1);
                glVertex2f(pos[0] + (i + 1) * 64 - 1, pos[1] - j * 64 - 1);
                glVertex2f(pos[0] + (i + 1) * 64 - 1, pos[1] - (j + 1) * 64 + 1);
                glVertex2f(pos[0] + i * 64 + 1, pos[1] - (j + 1) * 64 + 1);
            glEnd();

        }
    }
}

// Walls
class Boundary
{
public:
	vector<GLfloat> A;
	vector<GLfloat> B;

	Boundary(){} //dummy constructor
	Boundary(vector<GLfloat> A, vector<GLfloat> B)
    {
        this->A = A;
        this->B = B;
    }

	void show();
};
void Boundary::show()
{
    glBegin(GL_LINES);
    glVertex2f(A[0], A[1]);
    glVertex2f(B[0], B[1]);
    glEnd();
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
    vector<GLfloat> cast(Boundary wall);
};

vector<GLfloat> Ray::cast(Boundary wall)
{
    vector<GLfloat> res(3);
    GLfloat x1 = wall.A[0];
    GLfloat y1 = wall.A[1];
    GLfloat x2 = wall.B[0];
    GLfloat y2 = wall.B[1];

    GLfloat x3 = pos[0];
    GLfloat y3 = pos[1];
    GLfloat x4 = pos[0] + dir[0];
    GLfloat y4 = pos[1] + dir[1];

    GLfloat denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if(denominator == 0)
    {
        res[2] = -1;
        return res;
    }
    GLfloat t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
    GLfloat u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denominator;

    if (0 < t && t < 1)
    {
        res[0] = float(x1 + t * (x2 - x1));
        res[1] = float(y1 + t * (y2 - y1));
        res[2] = u;
        return res;
    }
    else
    {
        res[2] = -1;
        return res;
    }
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
	GLfloat speed;
	GLfloat FOV;
	GLfloat angle;
	GLint rayCount;
    GLfloat step;
	vector<Ray> rays;

    Player(){} //dummy constructor
    Player(vector<GLfloat> pos, GLfloat speed, const GLint rayCount, GLfloat FOV, GLfloat angle)
    {
        this->pos = pos;
        this->speed = speed;
        this->rayCount = rayCount;
        this->FOV = FOV;
        this->step = this->FOV / (rayCount - 1);
        this->angle = angle;
	}
    void show();
    void actions(bool keybuffer[], GLfloat delMouse[], GLfloat bounds);
    vector<GLfloat> see(vector<Boundary> walls, GLint wallsize, bool render);
};
void Player::actions(bool keybuffer[], GLfloat delMouse[], GLfloat bounds)
{
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
    angle = angle - delMouse[0];
    delMouse[0] = 0;

	for(int i = 0; i < rayCount; i++)
    {
        rays[i].pos = pos;
        GLfloat theta = angle -(FOV / 2) + i * step;
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

vector<GLfloat> Player::see(vector<Boundary> walls, const GLint wallsize, bool render=true)
{
    vector<GLfloat> pt(2);
    GLfloat dmin;
    GLboolean flag;
    vector<GLfloat> distances;
    vector<GLfloat> d;

    for(int i = 0; i < this->rayCount; i++)
    {
        dmin = 1000000;
        flag = false;
        for(int j = 0; j < wallsize; j++)
        {
            d = rays[i].cast(walls[j]);
            if (d[2] > 0)
            {
                d[2] *= cos((rays[i].angle - angle) * (3.14159 / 180));
                if(d[2] < dmin)
                {
                    dmin = d[2];
                    flag = true;
                    pt[0] = d[0];
                    pt[1] = d[1];
                }
            }

        }
        if (flag && render)
        {
            glBegin(GL_LINES);
            glVertex2f(pos[0], pos[1]);
            glVertex2f(pt[0], pt[1]);
            glEnd();

//            glBegin(GL_POINTS);
//            glVertex2f(pt[0], pt[1]);
//            glEnd();
        }
        distances.push_back(dmin);
    }
    return distances;
}
