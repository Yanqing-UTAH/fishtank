#ifndef FISHINFO_H
#define FISHINFO_H

#include "utildef.h"

class FishInfo
{
public:
    FishInfo();
    ~FishInfo();
    void SetHandler(wxEvtHandler* handler);
    int getPoint(int, bool = true) const;
    int getLevel(int, bool = true) const;
    int getExp(int, bool = true) const;
    int getX(int, bool = true) const;
    int getY(int, bool = true) const;
    int getHP(int, bool = true) const;
    int getMaxHP(int, bool = true) const;
    int getAtt(int, bool = true) const;
    int getSp(int, bool = true) const;
    int getScore(int, bool = true) const;
    int getFishCount(bool = true) const;
    int askWhat(int, int, bool = true) const;

    void setMap(int, int, int, bool = true);
    void setPoint(int, int, bool = true);
    void setLevel(int, int, bool = true);
    void setExp(int, int, bool = true);
    void setXY(int, int, int, bool = true);
    void setHP(int, int, bool = true);
    void setMaxHP(int, int, bool = true);
    void setAtt(int, int, bool = true);
    void setSp(int, int, bool = true);
    void setScore(int, int, bool = true);
    void addFish(bool = true);
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
    wxEvtHandler* evtHandler;
};

#endif // FISHINFO_H
