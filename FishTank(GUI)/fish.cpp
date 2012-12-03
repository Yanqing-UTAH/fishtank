#include "fish.h"

bool fish::move(int x0, int y0)
{
    return host->move(x0, y0);
}

bool fish::attack(int x0, int y0)
{
    return host->attack(x0, y0);
}

int fish::getPoint()
{
    return host->getPoint();
}

int fish::getLevel()
{
    return host->getLevel();
}

int fish::getExp()
{
    return host->getExp();
}

int fish::getX()
{
    return host->getX();
}

int fish::getY()
{
    return host->getY();
}

int fish::getHP()
{
    return host->getHP();
}

int fish::getMaxHP()
{
    return host->getMaxHP();
}

int fish::getAtt()
{
    return host->getAtt();
}

int fish::getSp()
{
    return host->getSp();
}

int fish::getID()
{
    return host->getID();
}

int fish::askWhat(int x, int y)
{
    return host->askWhat(x, y);
}

int fish::askHP(int target)
{
    return host->askHP(target);
}

bool fish::increaseHealth()
{
    return host->increaseHealth();
}

bool fish::increaseStrength()
{
    return host->increaseStrength();
}

bool fish::increaseSpeed()
{
    return host->increaseSpeed();
}
