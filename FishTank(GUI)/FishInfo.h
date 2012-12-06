#ifndef FISHINFO_H
#define FISHINFO_H

#include "utildef.h"

class FishInfo
{
public:
    FishInfo();
    ~FishInfo();
    int getPoint(int) const;
    int getLevel(int) const;
    int getExp(int) const;
    int getX(int) const;
    int getY(int) const;
    int getHP(int) const;
    int getMaxHP(int) const;
    int getAtt(int) const;
    int getSp(int) const;
    int getScore(int) const;
    int getFishCount() const;
    int askWhat(int, int) const;

    void setMap(int, int, int);
    void setPoint(int, int);
    void setLevel(int, int);
    void setExp(int, int);
    void setXY(int, int, int);
    void setHP(int, int);
    void setMaxHP(int, int);
    void setAtt(int, int);
    void setSp(int, int);
    void setScore(int, int);
    void addFish();
    void init();

private:
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
    int fishScore[MAX_PLAYER+1];
    int fishKill[MAX_PLAYER+1];
    int fishDie[MAX_PLAYER+1];
    int fishRank[MAX_PLAYER+1];
    int fishCount;

private:
    wxMutex* lock;
};

#endif // FISHINFO_H
