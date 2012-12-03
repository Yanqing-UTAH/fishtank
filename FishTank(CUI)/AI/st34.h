#ifndef ST34_H
#define ST34_H

#include "fish.h"
#include <cstdlib>

class st34 : public fish
{
public:

    void init();
    void play();
    void revive(int&, int&);

private:

    int calcSafe(int, int);
    int evaluate(int, int, int&);
    int eNow(int&, int&, int&);
    int ePoint(int, int);
    void increasePoint();
    void scan();
    inline int dis(int x1, int y1, int x2, int y2)
    {
        return (std::abs(x1 - x2) + std::abs(y1 - y2));
    }
    inline int dis(int x, int y)
    {
        return dis(x, y, getX(), getY());
    }
    inline int nextLevelExp()
    {
        return (2 + getLevel() + 1) * getLevel() / 2;
    }
    inline bool isEnemy(int target)
    {
        return ((target != EMPTY) && (target != FOOD) && (target != getID()));
    }
    static const int MAX = 10000;
    int d[4][2];
    int sx;
    int sy;
    int safe[N+1][M+1];

    int fX[MAX_PLAYER+1];
    int fY[MAX_PLAYER+1];
    int fMaxHP[MAX_PLAYER+1];
    int fSp[MAX_PLAYER+1];
    int found[MAX_PLAYER+1];
    int fDie[2000];
};

//可自行增加所需函数所需函数及变量，但需保证上面每个函数的完整与可调用

//不要使用全局变量！！

#endif
