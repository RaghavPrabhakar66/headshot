#ifndef MODULES_H_INCLUDED
#define MODULES_H_INCLUDED

#endif // MODULES_H_INCLUDED
#include "textures.h"
#include <iostream>
#include <math.h>
#include <string>

using namespace std;

#define PI 3.141592

// Functions
// Render sprite in 2d space
void drawSprite(GLfloat x, GLfloat y, GLfloat w, GLfloat h, vector<GLfloat> texture, vector<GLfloat> texture_alpha = vector<GLfloat>{69, 69, 69}, GLfloat texture_size=128)
{
    glPointSize(1);
    vector<GLfloat> t{0, 0}, tstep{texture_size / (w + 1), texture_size / (h + 1)};
    GLfloat c, r, g, b;

    for(int i = x; i < x + w; i++)
    {
        t[1] = 0;
        for(int j = y + h; j > y; j--)
        {
            c = (int(t[1]) * texture_size + int(t[0])) * 3;
            r = texture[c];
            g = texture[c + 1];
            b = texture[c + 2];
            if(!(r == texture_alpha[0] && g == texture_alpha[1] && b == texture_alpha[2]))
            {
                glBegin(GL_POINTS);
                glColor3ub(r, g, b);
                glVertex2d(i, j);
                glEnd();
            }
            t[1] += tstep[1];
        }
        t[0] += tstep[0];
    }
}

// Classes
// prototypes
class Player;
class Weapon;
class Sprite;
class Map;
class Ray;
class Hud;


// Map
class Map
/*
    This class is used to store the map data.
    It is used to render the map aka walls.
    params:
        - size: the size of the map
        - blockSize: the size of each block
*/
{
public:
    // 
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
    for (int i = shape[0] - 1; i >= 0; i--)
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
    /**/
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

class Weapon
{
public:
    GLint name;
    GLfloat damage;
    GLfloat range;
    GLint ammo;
    GLint cooldown;

    Weapon() {}
    Weapon(GLint name, GLfloat damage, GLfloat range, GLfloat ammo)
    {
        this->name = name;
        this->damage = damage;
        this->range = range;
        this->ammo = ammo;
        this->cooldown = 0;
    }

    void show(GLfloat bounds, GLfloat height);
    bool shoot();
};

bool Weapon::shoot()
{
    if(ammo && cooldown <= 0)
    {
        if(name == 0)
        {
            cooldown = 1;
        }
        else if(name == 1)
        {
            ammo--;
            cooldown = 5;
        }
        else if(name == 2)
        {
            ammo--;
            cooldown = 22;
        }

        return true;
    }

    return false;
}

void Weapon::show(GLfloat bounds, GLfloat height)
{
    if (name == 0)
    {
        //drawSprite(1.6 * bounds, bounds / 2 - height / 2, 128, 128, knife_textures[cooldown]);

    }
    else if (name == 1)
    {
        drawSprite(1.6 * bounds, bounds / 2 - height / 2, 128, 128, pistol_textures[cooldown]);
    }
    else if(name == 2)
    {
        if(0 < cooldown && cooldown < 21)
        {
            drawSprite(1.5 * bounds + 50, bounds / 2 - height / 2, 107, 118, shotgun_textures[cooldown / 2]);
        }
        else
        {
            drawSprite(1.5 * bounds, bounds / 2 - height / 2, 220, 150, shotgun_textures[cooldown / 2]);
        }
    }
    if(cooldown > 0)
    {
        cooldown--;
    }
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
    GLfloat health;
    GLfloat stamina;
    vector<Ray> rays;

    Player() {} // dummy constructor
    Player(vector<GLfloat> pos, GLfloat speed, const GLint rayCount, GLfloat FOV, GLfloat angle)
    {
        weapons = vector<Weapon>{Weapon(0, 75, 30, 1), Weapon(1, 50, 400, 18), Weapon(2, 30, 400, 10)};
        weapon = weapons[0];

        this->pos = pos;
        this->speed = speed;
        this->health = 100;
        this->stamina = 100;
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
    void actions(bool keybuffer[], bool specialkeybuffer[], GLint mousebuffer[], GLfloat bounds, Map m);
    vector<vector<GLfloat>> see(Map m);
};


void Player::actions(bool keybuffer[], bool specialkeybuffer[], GLint mousebuffer[], GLfloat bounds, Map m)
{
    // Use action
    if(specialkeybuffer[112] && stamina > 0)
    {
        speed = 1.6;
        stamina -= 1;
    }
    else
    {
        speed = 1;
        if (stamina < 100)
        {
            stamina+=0.5;
        }
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
    if (keybuffer['3'])
    {
        weapon = weapons[2];
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

class Hud
{
public:
    GLfloat bounds;
    GLfloat height;
    GLfloat rsod;

    Hud(GLfloat bounds, GLfloat height)
    {
        this->bounds = bounds;
        this->height = height;
        this->rsod = 0;
    }
    void show(Player p);
    void displayDialogue(string text, GLint maxLength, GLfloat offset);
};

void Hud::displayDialogue(string text, GLint maxLength = 60, GLfloat offset = 20)
{
    // textbox
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
}

void Hud::show(Player p)
{
    //Rsod
    if(rsod > 0)
    {
        glColor3f(1, 0, 0);
        glBegin(GL_QUADS);
        glVertex2f(bounds, bounds / 2 - height / 2);
        glVertex2f(bounds, bounds / 2 + height / 2);
        glVertex2f(2 * bounds, bounds / 2 + height / 2);
        glVertex2f(2 * bounds, bounds / 2 - height / 2);
        glEnd();
        rsod--;
    }

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

    // equipped weapon
    glBegin(GL_LINE_LOOP);
    glColor3ub(255, 255, 0);
    glVertex2f(bounds, bounds / 2 + height / 2);
    glVertex2f(1.5 * bounds, bounds / 2 + height / 2);
    glVertex2f(1.5 * bounds, bounds);
    glVertex2f(bounds, bounds);
    glEnd();

    // Stats
    // health bar
    glColor3f(1, 0, 0);
    glRasterPos2f(1.6 * bounds, bounds - 20);
    string health  = "HP";
    for(int i = 0; i < health.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, health[i]);
    }
    // health bar fill
    glBegin(GL_QUADS);
    glColor3f(1, 0, 0);
    glVertex2f(1.7 * bounds, bounds - 10);
    glVertex2f(1.7 * bounds, bounds - 20);
    glVertex2f(1.7 * bounds + (bounds * p.health / 500.0), bounds - 20);
    glVertex2f(1.7 * bounds + (bounds * p.health / 500.0), bounds - 10);
    glEnd();

    // health bar boundary
    glLineWidth(1);
    glColor3f(0.5, 0.5, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(1.7 * bounds, bounds - 10);
    glVertex2f(1.7 * bounds, bounds - 20);
    glVertex2f(1.9 * bounds, bounds - 20);
    glVertex2f(1.9 * bounds, bounds - 10);
    glEnd();

    // stamina
    glColor3f(0, 1, 0);
    glRasterPos2f(1.6 * bounds, bounds - 40);
    string stamina  = "SP";
    for(int i = 0; i < stamina.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, stamina[i]);
    }
    // stamina bar fill
    glColor3f(0, 1, 0);
    glBegin(GL_QUADS);
    glVertex2f(1.7 * bounds, bounds - 30);
    glVertex2f(1.7 * bounds, bounds - 40);
    glVertex2f(1.7 * bounds + (bounds * p.stamina / 500.0), bounds - 40);
    glVertex2f(1.7 * bounds + (bounds * p.stamina / 500.0), bounds - 30);
    glEnd();

    // stamina bar boundary
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0.5, 0);
    glVertex2f(1.7 * bounds, bounds - 10);
    glVertex2f(1.7 * bounds, bounds - 20);
    glVertex2f(1.9 * bounds, bounds - 20);
    glVertex2f(1.9 * bounds, bounds - 10);
    glEnd();

    // ammo
    glRasterPos2f(1.6 * bounds, 50);
    string ammo     = "Ammo     :   " + to_string(int(p.weapon.ammo));
    for(int i = 0; i < ammo.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ammo[i]);
    }

    // face
    // sexy design
    glColor3ub(255, 255, 255);
    glLineWidth(1);
}


class Sprite
{
public:
    vector<GLfloat> pos;
    vector<GLfloat> shape;
    vector<GLfloat> texture;
    vector<GLfloat> texture_alpha;
    string dialogue;

    GLfloat threshold;
    bool proximity;
    bool visible;

    Sprite() {}
    Sprite(vector<GLfloat> pos, vector<GLfloat> shape, vector<GLfloat> texture, vector<GLfloat> texture_alpha, GLfloat threshold, string dialogue)
    {
        this->pos = pos;
        this->shape = shape;
        this->texture = texture;
        this->texture_alpha = texture_alpha;
        this->threshold = threshold;
        this->proximity = false;
        this->dialogue = dialogue;
        this->visible = false;
    }
    void show(Player p, GLfloat bounds, GLfloat maxHeight, vector<vector<GLfloat>> distances, GLint texture_size);
    void actions(Hud &hud);
    void see(Player p);
};

void Sprite::show(Player p, GLfloat bounds, GLfloat maxHeight, vector<vector<GLfloat>> distances, GLint texture_size = 128)
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

    glPointSize(1);
    GLfloat width = shape[0] * 6 * 64 / dist, height = shape[1] * 6 * 64 / dist;
    GLfloat x = 1.5 * bounds - sin(theta * PI / 180) * bounds - width / 2, y = bounds / 2 - height / 2, c, r, g, b;
    vector<GLfloat> t{0, 0}, tstep{texture_size / (width + 1), texture_size / (height + 1)};
    visible = false;
    if (theta < p.FOV / 2 && theta > -p.FOV / 2 && dist > 5  && distances[0][(int)(p.rayCount * (p.FOV / 2 + theta) / p.FOV)] > dist)
    {
        visible = true;
        for(int i = x + width; i > x; i--)
        {
            t[1] = 0;
            for(int j = y + height; j > y; j--)
            {
                if(bounds < i && i < 2 * bounds && bounds / 2 -maxHeight / 2 < j && j < bounds / 2 + maxHeight / 2)
                {
                    c = (int(t[1]) * texture_size + int(t[0])) * 3;
                    r = texture[c];
                    g = texture[c + 1];
                    b = texture[c + 2];
                    if(!(r == texture_alpha[0] && g == texture_alpha[1] && b == texture_alpha[2]))
                    {
                        glBegin(GL_POINTS);
                        glColor3ub(r, g, b);
                        glVertex2d(i, j);
                        glEnd();
                    }
                }
                t[1] += tstep[1];
            }
            t[0] += tstep[0];
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
void Sprite::actions(Hud &hud)
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
    GLfloat health;
    GLfloat damage;
    GLint cooldown;
    Enemy(vector<GLfloat> pos, vector<GLfloat> shape, vector<GLfloat> texture, vector<GLfloat> texture_alpha, GLfloat health = 100, GLfloat damage = 20, GLfloat threshold = 120, GLfloat speed = 0.5)
    {
        this->pos = pos;
        this->shape = shape;
        this->texture = texture;
        this->texture_alpha = texture_alpha;
        this->health = health;
        this->damage = damage;
        this->threshold = threshold;
        this->speed = speed;
        this->proximity = false;
        this->cooldown = 0;
    }
    void actions(Player &p, Map m, Hud &hud);
};

void Enemy::actions(Player &p, Map m, Hud &hud)
{
    if(proximity && cooldown <= 0)
    {
        GLfloat dx = p.pos[0] - pos[0];
        GLfloat dy = p.pos[1] - pos[1];
        GLfloat dist = sqrt(dx* dx + dy * dy), offset = 10;
        dx /= dist;
        dy /= dist;
        if(dist > 20)
        {
            if(m.walls[(pos[0] + dx *offset) / m.blockSize][pos[1] / m.blockSize] == 0)
            {
                pos[0] += dx * speed;
            }
            if(m.walls[pos[0] / m.blockSize][(pos[1] + dy * offset) / m.blockSize] == 0)
            {
                pos[1] += dy * speed;
            }
        }
        else
        {
            p.health -= damage;
            cooldown = 50;
            hud.rsod = 2;
        }
    }
    if(cooldown > 0)
    {
        cooldown--;
    }
}
