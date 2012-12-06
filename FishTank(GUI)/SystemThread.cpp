#include "SystemThread.h"
#include "FishTankApp.h"
#include "fish.h"

#include <cstdlib>
#include <ctime>
#include "TAAI.h"

using namespace std;

wxThread::ExitCode SystemThread::Entry()
{
    play();
    return NULL;
}

SystemThread::SystemThread() : data(((FishTankApp*)wxTheApp)->data)
{
    init();
    for (int i = 1; i <= MAX_PLAYER; ++i)
    {
        fish* AI = new TAAI();
        AI->setHost(this);
        addAI(AI);
    }
}

void SystemThread::SetHandler(wxEvtHandler* handler)
{
    evtHandler = handler;
}

void SystemThread::init()
{
    data->init();
    reviveNum = 0;
    phase = INIT_PHASE;
    round = 0;
    srand(time(NULL));
}

SystemThread::~SystemThread()
{
    for (int i = 1; i <= data->getFishCount(); ++i)
        delete player[i];
}

void SystemThread::SendLog(const wxString& str)
{
    wxCommandEvent* event = new wxCommandEvent(wxEVT_SEND_MSG, SEND_MSG_ID);
    event->SetString(str);
    evtHandler->QueueEvent(event);
}

bool SystemThread::addAI(fish* newAI)
{
    if ((data->getFishCount() < MAX_PLAYER) && (newAI))
    {
        data->addFish();
        player[data->getFishCount()] = newAI;
        return true;
    }
    return false;
}

void SystemThread::foodRefresh()
{
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j)
            if (askWhat(i, j) == FOOD)
                data->setMap(i, j, EMPTY);
    for (int i = 1; i <= MAX_FOOD; ++i)
    {
        int x = 0, y = 0;
        randXY(x, y);
        data->setMap(x, y, FOOD);
    }
    SendLog("Food refreshed");
}

void SystemThread::fishInit()
{
    phase = INIT_PHASE;
    for (int i = 1; i <= data->getFishCount(); ++i)
    {
        int x = 0, y = 0;
        randXY(x, y);
        data->setMap(x, y, i);
        data->setPoint(i, 10);
        data->setLevel(i, 1);
        data->setExp(i, 0);
        data->setXY(i, x, y);
        data->setHP(i, 0);
        data->setMaxHP(i, 0);
        data->setAtt(i, 0);
        data->setSp(i, 0);
        fishKill[i] = 0;
        fishDie[i] = 0;
    }
    for (int i = 1; i <= data->getFishCount(); ++i)
    {
        current = i;
        player[i]->init();
        SendLog(wxString::Format("Fish %d borns at (%d, %d), MaxHP = %d, Sp = %d, Att = %d", i, getX(), getY(), getMaxHP(), getSp(), getAtt()));
    }
}

void SystemThread::calcPriority()
{
    for (int i = 1; i <= data->getFishCount(); ++i)
        sequence[i] = i;
    sort(1, data->getFishCount());
}

void SystemThread::fishRevive()
{
    for (int i = 1; i <= reviveNum; ++i)
    {
        phase = REVIVE_PHASE;
        int x = 0, y = 0, target = reviveList[i];
        current = reviveList[i];
        player[target]->revive(x, y);
        if (!validCor(x, y) || (askWhat(x, y) != EMPTY))
            randXY(x, y);
        data->setMap(x, y, target);
        data->setXY(target, x, y);
        data->setHP(target, larger(data->getMaxHP(target) / 10, 1));
        SendLog(wxString::Format("Fish %d revives at (%d, %d)", target, x, y));
    }
    reviveNum = 0;
    for (int i = 1; i <= data->getFishCount(); ++i)
        if (askHP(i) <= 0)
            reviveList[++reviveNum] = i;
}

void SystemThread::fishPlay()
{
    for (int i = 1; i <= data->getFishCount(); ++i)
    {
        phase = MAIN_PHASE;
        current = sequence[i];
        if (getHP() > 0)
        {
            SendLog(wxString::Format("Fish %d is in action now", current));
            wxCommandEvent* event = new wxCommandEvent(wxEVT_SEND_SB, SEND_SB_ID);
            event->SetString(wxString::Format("Round %d : Fish %d is in action now", round, current));
            ((FishTankApp*)wxTheApp)->frame->GetEventHandler()->QueueEvent(event);
            player[current]->play();
            ((FishTankApp*)wxTheApp)->watcher->Refresh();
        }
    }
}

void SystemThread::play()
{
    fishInit();
    while (round <= GAME_ROUND)
        turn();
}

void SystemThread::turn()
{
    if (++round <= GAME_ROUND)
    {
        SendLog(wxString::Format("Round: %d", round));
        if (round % 5 == 1)
            foodRefresh();
        fishRevive();
        calcPriority();
        fishPlay();
    }
}

bool SystemThread::move(int x, int y)
{
    if (phase != MAIN_PHASE)
    {
        SendLog(wxString::Format("Fish %d attempts to move from (%d, %d) to (%d, %d)", current, getX(), getY(), x, y));
        SendLog("Failed (Cannot move in this phase)");
        return false;
    }
    if (!validCor(x, y))
    {
        SendLog(wxString::Format("Fish %d attempts to move from (%d, %d) to (%d, %d)", current, getX(), getY(), x, y));
        SendLog("Failed (Invalid coordinate)");
        return false;
    }
    if (askWhat(x, y) != EMPTY)
    {
        SendLog(wxString::Format("Fish %d attempts to move from (%d, %d) to (%d, %d)", current, getX(), getY(), x, y));
        SendLog("Failed (Target area is not empty)");
        return false;
    }
    if (dis(getX(), getY(), x, y) > getSp())
    {
        SendLog(wxString::Format("Fish %d attempts to move from (%d, %d) to (%d, %d)", current, getX(), getY(), x, y));
        SendLog("Failed (Out of move range)");
        return false;
    }
    SendLog(wxString::Format("Fish %d moves from (%d, %d) to (%d, %d)", current, getX(), getY(), x, y));
    data->setMap(getX(), getY(), EMPTY);
    data->setXY(current, x, y);
    data->setMap(x, y, current);
    phase = MOVE_PHASE;
    return true;
}

bool SystemThread::attack(int x, int y)
{
    if ((phase != MAIN_PHASE) && (phase != MOVE_PHASE))
    {
        SendLog(wxString::Format("Fish %d attempts to attack (%d, %d)", current, x, y));
        SendLog("Failed (Cannot attack in this phase)");
        return false;
    }
    if (!validCor(x, y))
    {
        SendLog(wxString::Format("Fish %d attempts to attack (%d, %d)", current, x, y));
        SendLog("Failed (Invalid coordinate)");
        return false;
    }
    if (dis(getX(), getY(), x, y) != 1)
    {
        SendLog(wxString::Format("Fish %d attempts to attack (%d, %d)", current, x, y));
        SendLog("Failed (Out of attack range)");
        return false;
    }
    if (askWhat(x, y) == EMPTY)
    {
        SendLog(wxString::Format("Fish %d attempts to attack (%d, %d)", current, x, y));
        SendLog("Failed (Target area is empty)");
        return false;
    }
    if (askWhat(x, y) == FOOD)
    {
        SendLog(wxString::Format("Fish %d eats food at (%d, %d)", current, x, y));
        data->setMap(x, y, EMPTY);
        increaseHP(larger(getMaxHP() / 10, 2));
        increaseExp(1);
    }
    else
    {
        int target = askWhat(x, y);
        SendLog(wxString::Format("Fish %d attacks Fish %d at (%d, %d)", current, target, x, y));
        decreaseHP(target, getAtt());
        if (!askHP(target))
        {
            data->setMap(x, y, EMPTY);
            int targetLevel = data->getLevel(target);
            if ((targetLevel) > getLevel())
                data->setScore(current, data->getScore(current) + 2 * (targetLevel - getLevel()));
            increaseExp(larger(1, targetLevel / 2));
            ++fishKill[current];
            ++fishDie[target];
        }
    }
    phase = ATTACK_PHASE;
    return true;
}

void SystemThread::increaseHP(int value)
{
    if (value <= 0)
        return;
    int HP = getHP() + value;
    if (HP > getMaxHP())
        HP = getMaxHP();
    data->setHP(current, HP);
    if (phase != INIT_PHASE)
        SendLog(wxString::Format("Fish %d's HP increases to %d", current, HP));
}

void SystemThread::decreaseHP(int target, int value)
{
    if (value <= 0)
        return;
    int HP = askHP(target) - value;
    if (HP < 0)
        HP = 0;
    data->setHP(target, HP);
    SendLog(wxString::Format("Fish %d's HP decreases to %d", target, HP));
    if (!HP)
    {
        SendLog(wxString::Format("Fish %d is killed", target));
    }
}

void SystemThread::increaseExp(int value)
{

    data->setExp(current, getExp() + value);
    data->setScore(current, data->getScore(current) + value);
    SendLog(wxString::Format("Fish %d's Exp increases to %d", current, getExp()));
    while (getExp() >= (2 + getLevel() + 1) * getLevel() / 2)
    {
        data->setLevel(current, getLevel() + 1);
        data->setPoint(current, getPoint() + 3);
        SendLog(wxString::Format("Fish %d's Level is upgraded to %d", current, getLevel()));
    }
}

bool SystemThread::increaseHealth()
{
    if (getPoint())
    {
        data->setMaxHP(current, getMaxHP() + 2);
        increaseHP(2);
        data->setPoint(current, getPoint() - 1);
        if (phase != INIT_PHASE)
            SendLog(wxString::Format("Fish %d's MaxHP increases to %d", current, getMaxHP()));
        return true;
    }
    return false;
}

bool SystemThread::increaseStrength()
{
    if (getPoint())
    {
        data->setAtt(current, getAtt() + 1);
        data->setPoint(current, getPoint() - 1);
        if (phase != INIT_PHASE)
            SendLog(wxString::Format("Fish %d's Strength increases to %d", current, getAtt()));
        return true;
    }
    return false;
}

bool SystemThread::increaseSpeed()
{
    if (getPoint())
    {
        data->setSp(current, getSp() + 1);
        data->setPoint(current, getPoint() - 1);
        if (phase != INIT_PHASE)
            SendLog(wxString::Format("Fish %d's Speed increases to %d", current, getSp()));
        return true;
    }
    return false;
}

bool SystemThread::sortCmp(int t1, int t2) const
{
    int x1 = data->getSp(t1);
    int x2 = data->getSp(t2);
    if (x1 < x2) return true;
    if (x1 > x2) return false;
    double y1 = data->getMaxHP(t1) ? data->getHP(t1) / data->getMaxHP(t1) : 0;
    double y2 = data->getMaxHP(t2) ? data->getHP(t2) / data->getMaxHP(t2) : 0;
    if (y1 < y2) return true;
    if (y1 > y2) return false;
    int z1 = data->getExp(t1) + fishBonus[t1];
    int z2 = data->getExp(t2) + fishBonus[t2];
    if (z1 > z2) return true;
    if (z1 < z2) return false;
    return false;
}

void SystemThread::sort(int l, int r)
{
    int i = l;
    int j = r;
    int m = sequence[(l + r) >> 1];
    while (i < j)
    {
        while (sortCmp(sequence[j], m)) --j;
        while (sortCmp(m, sequence[i])) ++i;
        if (i <= j)
        {
            int t = sequence[i];
            sequence[i] = sequence[j];
            sequence[j] = t;
            ++i;
            --j;
        }
    }
    if (l < j) sort(l, j);
    if (i < r) sort(i, r);
}

void SystemThread::randXY(int& x, int& y)
{
    int x0 = rand() % N + 1;
    int y0 = rand() % M + 1;
    while (askWhat(x0, y0) != EMPTY)
    {
        x0 = rand() % N + 1;
        y0 = rand() % M + 1;
    }
    x = x0;
    y = y0;
}

int SystemThread::getPoint() const
{
    return data->getPoint(current);
}
int SystemThread::getLevel() const
{
    return data->getLevel(current);
}
int SystemThread::getExp() const
{
    return data->getExp(current);
}
int SystemThread::getX() const
{
    return data->getX(current);
}
int SystemThread::getY() const
{
    return data->getY(current);
}
int SystemThread::getHP() const
{
    return data->getHP(current);
}
int SystemThread::getMaxHP() const
{
    return data->getMaxHP(current);
}
int SystemThread::getAtt() const
{
    return data->getAtt(current);
}
int SystemThread::getSp() const
{
    return data->getSp(current);
}
int SystemThread::getID() const
{
    return current;
}
int SystemThread::askWhat(int x, int y) const
{
    if (phase != REVIVE_PHASE)
    {
        if (validCor(x, y))
            return data->askWhat(x, y);
        return EMPTY;
    }
    return EMPTY;
}
int SystemThread::askHP(int id) const
{
    if ((id >= 1) && (id <= data->getFishCount()))
        return data->getHP(id);
    return 0;
}
