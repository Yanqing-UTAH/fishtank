#include "env.h"
#include "fish.h"

#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <iostream>
#include <iomanip>

using namespace std;

env& env::GetInstance()
{
    static env instance;
    return instance;
}

env::env() : fishNum(0), reviveNum(0), phase(INIT_PHASE)
{
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j)
            setMap(i, j, EMPTY);
    srand(time(NULL));
}

env::~env()
{
    for (int i = 1; i <= MAX_PLAYER; ++i)
        delete player[i];
}

bool env::addAI(fish* newAI)
{
    if ((fishNum < MAX_PLAYER) && (newAI))
    {
        player[++fishNum] = newAI;
        return true;
    }
    return false;
}

void env::foodRefresh()
{
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j)
            if (askWhat(i, j) == FOOD)
                setMap(i, j, EMPTY);
    for (int i = 1; i <= MAX_FOOD; ++i)
    {
        int x = 0, y = 0;
        randXY(x, y);
        setMap(x, y, FOOD);
    }
}

void env::fishInit()
{
    phase = INIT_PHASE;
    for (int i = 1; i <= fishNum; ++i)
    {
        int x = 0, y = 0;
        randXY(x, y);
        setMap(x, y, i);
        setPoint(i, 10);
        setLevel(i, 1);
        setExp(i, 0);
        setXY(i, x, y);
        setHP(i, 0);
        setMaxHP(i, 0);
        setAtt(i, 0);
        setSp(i, 0);
        fishBonus[i] = 0;
        fishKill[i] = 0;
        fishDie[i] = 0;
    }
    for (int i = 1; i <= fishNum; ++i)
    {
        current = i;
        player[i]->init();
    }
}

void env::calcPriority()
{
    for (int i = 1; i <= fishNum; ++i)
        sequence[i] = i;
    sort(1, fishNum);
}

void env::fishRevive()
{
    for (int i = 1; i <= reviveNum; ++i)
    {
        phase = REVIVE_PHASE;
        int x = 0, y = 0, target = reviveList[i];
        current = reviveList[i];
        player[target]->revive(x, y);
        phase = REVIVE_PHASE_2;
        if (!validCor(x, y) || (askWhat(x, y) != EMPTY))
            randXY(x, y);
        setMap(x, y, target);
        setXY(target, x, y);
        setHP(target, larger(fishMaxHP[target] / 10, 1));
    }
    reviveNum = 0;
    for (int i = 1; i <= fishNum; ++i)
        if (askHP(i) <= 0)
            reviveList[++reviveNum] = i;
}

void env::fishPlay()
{
    for (int i = 1; i <= fishNum; ++i)
    {
        phase = MAIN_PHASE;
        current = sequence[i];
        if (getHP() > 0)
            player[current]->play();
    }
}

void env::play()
{
    fishInit();
    for (int round = 1; round <= GAME_ROUND; ++round)
    {
        cout << "Round " << round << endl;
        if (round % 5 == 1)
            foodRefresh();
        fishRevive();
        calcPriority();
        fishPlay();
    }
    printResult();
}

bool env::move(int x, int y)
{
    if (phase != MAIN_PHASE)
    {
        return false;
    }
    if (!validCor(x, y))
    {
        return false;
    }
    if (askWhat(x, y) != EMPTY)
    {
        return false;
    }
    if (dis(x, y) > getSp())
    {
        return false;
    }
    setMap(getX(), getY(), EMPTY);
    setXY(current, x, y);
    setMap(x, y, current);
    phase = MOVE_PHASE;
    return true;
}

bool env::attack(int x, int y)
{
    if ((phase != MAIN_PHASE) && (phase != MOVE_PHASE))
    {
        return false;
    }
    if (!validCor(x, y))
    {
        return false;
    }
    if (dis(x, y) != 1)
    {
        return false;
    }
    if (askWhat(x, y) == EMPTY)
    {
        return false;
    }
    if (askWhat(x, y) == FOOD)
    {
        setMap(x, y, EMPTY);
        increaseHP(larger(getMaxHP() / 10, 2));
        increaseExp(1);
    }
    else
    {
        int target = askWhat(x, y);
        decreaseHP(target, getAtt());
        if (!askHP(target))
        {
            setMap(x, y, EMPTY);
            if (fishLevel[target] > getLevel())
                fishBonus[current] += 2 * (fishLevel[target] - getLevel());
            increaseExp(larger(1, fishLevel[target] / 2));
            ++fishKill[current];
            ++fishDie[target];
        }
    }
    phase = ATTACK_PHASE;
    return true;
}

int env::getPoint() const
{
    return fishPoint[current];
}

int env::getLevel() const
{
    return fishLevel[current];
}

int env::getExp() const
{
    return fishExp[current];
}

int env::getX() const
{
    return fishX[current];
}

int env::getY() const
{
    return fishY[current];
}

int env::getHP() const
{
    return fishHP[current];
}

int env::getMaxHP() const
{
    return fishMaxHP[current];
}

int env::getAtt() const
{
    return fishAtt[current];
}

int env::getSp() const
{
    return fishSp[current];
}

int env::getID() const
{
    return current;
}

int env::askWhat(int x, int y) const
{
    if (phase != REVIVE_PHASE)
    {
        if (validCor(x, y))
            return map[x][y];
        return EMPTY;
    }
    return EMPTY;
}

int env::askHP(int id) const
{
    if ((id >= 1) && (id <= fishNum))
        return fishHP[id];
    return 0;
}

void env::increaseHP(int value)
{
    if (value <= 0)
        return;
    int HP = getHP() + value;
    if (HP > getMaxHP())
        HP = getMaxHP();
    setHP(current, HP);
}

void env::decreaseHP(int target, int value)
{
    if (value <= 0)
        return;
    int HP = askHP(target) - value;
    if (HP < 0)
        HP = 0;
    setHP(target, HP);
}

void env::increaseExp(int value)
{
    setExp(current, getExp() + value);
    while (getExp() >= (2 + getLevel() + 1) * getLevel() / 2)
    {
        setLevel(current, getLevel() + 1);
        setPoint(current, getPoint() + 3);
    }
}

bool env::increaseHealth()
{
    if (getPoint())
    {
        setHP(current, getHP() + 2);
        setMaxHP(current, getMaxHP() + 2);
        setPoint(current, getPoint() - 1);
        return true;
    }
    return false;
}

bool env::increaseStrength()
{
    if (getPoint())
    {
        setAtt(current, getAtt() + 1);
        setPoint(current, getPoint() - 1);
        return true;
    }
    return false;
}

bool env::increaseSpeed()
{
    if (getPoint())
    {
        setSp(current, getSp() + 1);
        setPoint(current, getPoint() - 1);
        return true;
    }
    return false;
}

bool env::sortCmp(int t1, int t2) const
{
    int x1 = fishSp[t1];
    int x2 = fishSp[t2];
    if (x1 < x2) return true;
    if (x1 > x2) return false;
    double y1 = fishMaxHP[t1] == 0 ? 0 : fishHP[t1] / fishMaxHP[t1];
    double y2 = fishMaxHP[t2] == 0 ? 0 : fishHP[t2] / fishMaxHP[t2];
    if (y1 < y2) return true;
    if (y1 > y2) return false;
    int z1 = fishExp[t1] + fishBonus[t1];
    int z2 = fishExp[t2] + fishBonus[t2];
    if (z1 > z2) return true;
    if (z1 < z2) return false;
    return false;
}

void env::sort(int l, int r)
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

void env::randXY(int& x, int& y)
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

void env::setMap(int x, int y, int target)
{
    if (validCor(x, y))
        map[x][y] = target;
}

void env::setXY(int target, int x, int y)
{
    fishX[target] = x;
    fishY[target] = y;
}

void env::setHP(int target, int value)
{
    fishHP[target] = value;
}

void env::setMaxHP(int target, int value)
{
    fishMaxHP[target] = value;
}

void env::setAtt(int target, int value)
{
    fishAtt[target] = value;
}

void env::setSp(int target, int value)
{
    fishSp[target] = value;
}

void env::setExp(int target, int value)
{
    fishExp[target] = value;
}

void env::setLevel(int target, int value)
{
    fishLevel[target] = value;
}

void env::setPoint(int target, int value)
{
    fishPoint[target] = value;
}

void env::printResult()
{
    for (int i = 1; i <= fishNum; ++i)
        fishRank[i] = i;
    cout << "ID Score  Lv   HP MaxHP  Sp Att Kill Die" << endl;
    for (int j = 1; j <= fishNum; ++j)
    {
        int i = fishRank[j];
        cout << setw(3) << i << setw(5) << fishExp[i] + fishBonus[i] << setw(4) << fishLevel[i] << setw(5) << fishHP[i] << setw(6) << fishMaxHP[i] << setw(4) << fishSp[i] << setw(4) << fishAtt[i] << setw(5) << fishKill[i] << setw(4) << fishDie[i] << endl;
    }
    system("pause");
}
