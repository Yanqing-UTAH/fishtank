#ifndef ENV_H
#define ENV_H

#include "utildef.h"
#include <cstdlib>
#include <fstream>

class fish;

class env
{
public:
    ~env();
    bool addAI(fish*);
    void play();

private:
    void foodRefresh();
    bool sortCmp(int, int) const;
    void sort(int, int);
    void fishInit();
    void fishRevive();
    void fishPlay();
    void calcPriority();
    void randXY(int&, int&);
    void printResult();
    int larger(int, int) const;
    int dis(int, int, int, int) const;
    int dis(int, int) const;
    bool validCor(int, int) const;

private:
    int current;
    fish* player[MAX_PLAYER+1];
    int fishNum;

    int map[N+1][M+1];
    int fishPoint[MAX_PLAYER+1];
    int fishLevel[MAX_PLAYER+1];
    int fishExp[MAX_PLAYER+1];
    int fishX[MAX_PLAYER+1];
    int fishY[MAX_PLAYER+1];
    int fishHP[MAX_PLAYER+1];
    int fishMaxHP[MAX_PLAYER+1];
    int fishAtt[MAX_PLAYER+1];
    int fishSp[MAX_PLAYER+1];
    int fishBonus[MAX_PLAYER+1];
    int fishKill[MAX_PLAYER+1];
    int fishDie[MAX_PLAYER+1];
    int fishRank[MAX_PLAYER+1];

    int reviveList[MAX_PLAYER+1];
    int reviveNum;

    int phase;

    static const int INIT_PHASE = 3;
    static const int MAIN_PHASE = 0;
    static const int MOVE_PHASE = 1;
    static const int ATTACK_PHASE = 2;
    static const int REVIVE_PHASE = 4;
    static const int REVIVE_PHASE_2 = 5;

    int sequence[MAX_PLAYER+1];

public:
    bool move(int, int);
    bool attack(int, int);
    int getPoint() const;
    int getLevel() const;
    int getExp() const;
    int getX() const;
    int getY() const;
    int getHP() const;
    int getMaxHP() const;
    int getAtt() const;
    int getSp() const;
    int getID() const;
    int askWhat(int, int) const;
    int askHP(int) const;
    bool increaseHealth();
    bool increaseStrength();
    bool increaseSpeed();

private:
    void setMap(int, int, int);
    void increasePoint(int);
    void increaseLevel(int);
    void increaseExp(int);
    void increaseHP(int);
    void decreaseHP(int, int);
    void setPoint(int, int);
    void setLevel(int, int);
    void setExp(int, int);
    void setXY(int, int, int);
    void setHP(int, int);
    void setMaxHP(int, int);
    void setAtt(int, int);
    void setSp(int, int);

public:
    static env& GetInstance();

private:
    env();
    env(const env&);
    env& operator=(const env&);
};

inline int env::larger(int a, int b) const
{
    return a > b ? a : b;
}

inline int env::dis(int x1, int y1, int x2, int y2) const
{
    return (std::abs(x1 - x2) + std::abs(y1 - y2));
}

inline int env::dis(int x, int y) const
{
    return dis(x, y, getX(), getY());
}

inline bool env::validCor(int x, int y) const
{
    return ((x >= 1) && (x <= N) && (y >= 1) && (y <= M));
}

#endif // ENV_H
