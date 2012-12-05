#ifndef SYSTEMTHREAD_H
#define SYSTEMTHREAD_H

#include "utildef.h"
#include "FishInfo.h"
#include "Events.h"
#include "wx/button.h"

class SystemThread : public wxThread
{
public:
    SystemThread();
    void init();
    virtual ~SystemThread();
    virtual void* Entry();

    bool addAI(fish*);
    void play();
    void turn();

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
    void increaseHP(int);
    void decreaseHP(int, int);
    void increaseExp(int);

private:
    int current;
    fish* player[MAX_PLAYER+1];
    int fishNum;

    int reviveList[MAX_PLAYER+1];
    int reviveNum;

    int phase;
    int round;

    enum
    {
        INIT_PHASE,
        MAIN_PHASE,
        MOVE_PHASE,
        ATTACK_PHASE,
        REVIVE_PHASE
    };

    int sequence[MAX_PLAYER+1];
    int fishBonus[MAX_PLAYER+1];
    int fishKill[MAX_PLAYER+1];
    int fishDie[MAX_PLAYER+1];

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
    FishInfo* const data;
    wxEvtHandler* const evtHandler;
};

#endif // SystemThread_H
