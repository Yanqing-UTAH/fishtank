#include "FishInfo.h"
#include "Events.h"
#include "FishTankApp.h"

FishInfo::FishInfo() : lock(new wxMutex) {}

FishInfo::~FishInfo()
{
    delete lock;
}

void FishInfo::init()
{
    fishCount = 0;
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j)
            setMap(i, j, EMPTY);
}

int FishInfo::getPoint(int target) const
{
    //wxMutexLocker locker(*lock);
    return fishPoint[target];
}

int FishInfo::getLevel(int target) const
{
    //wxMutexLocker locker(*lock);
    return fishLevel[target];
}

int FishInfo::getExp(int target) const
{
    //wxMutexLocker locker(*lock);
    return fishExp[target];
}

int FishInfo::getX(int target) const
{
    //wxMutexLocker locker(*lock);
    return fishX[target];
}

int FishInfo::getY(int target) const
{
    //wxMutexLocker locker(*lock);
    return fishY[target];
}

int FishInfo::getHP(int target) const
{
    //wxMutexLocker locker(*lock);
    return fishHP[target];
}

int FishInfo::getMaxHP(int target) const
{
    //wxMutexLocker locker(*lock);
    return fishMaxHP[target];
}

int FishInfo::getAtt(int target) const
{
    //wxMutexLocker locker(*lock);
    return fishAtt[target];
}

int FishInfo::getSp(int target) const
{
    //wxMutexLocker locker(*lock);
    return fishSp[target];
}

int FishInfo::getScore(int target) const
{
    //wxMutexLocker locker(*lock);
    return fishScore[target];
}

int FishInfo::getFishCount() const
{
    return fishCount;
}

int FishInfo::askWhat(int x, int y) const
{
    //wxMutexLocker locker(*lock);
    return map[x][y];
}

void FishInfo::setMap(int x, int y, int target)
{
    //wxMutexLocker locker(*lock);
    if (validCor(x, y))
    {
        map[x][y] = target;
        wxSetMapEvent* event = new wxSetMapEvent(x, y, wxEVT_SET_MAP, SET_MAP_ID);
        ((FishTankApp*)wxTheApp)->frame->GetEventHandler()->QueueEvent(event);
    }
}

void FishInfo::setXY(int target, int x, int y)
{
    //wxMutexLocker locker(*lock);
    fishX[target] = x;
    fishY[target] = y;
}

void FishInfo::setHP(int target, int value)
{
    //wxMutexLocker locker(*lock);
    fishHP[target] = value;
}

void FishInfo::setMaxHP(int target, int value)
{
    //wxMutexLocker locker(*lock);
    fishMaxHP[target] = value;
}

void FishInfo::setAtt(int target, int value)
{
    //wxMutexLocker locker(*lock);
    fishAtt[target] = value;
}

void FishInfo::setSp(int target, int value)
{
    //wxMutexLocker locker(*lock);
    fishSp[target] = value;
}

void FishInfo::setExp(int target, int value)
{
    //wxMutexLocker locker(*lock);
    fishExp[target] = value;
}

void FishInfo::setLevel(int target, int value)
{
    //wxMutexLocker locker(*lock);
    fishLevel[target] = value;
}

void FishInfo::setPoint(int target, int value)
{
    //wxMutexLocker locker(*lock);
    fishPoint[target] = value;
}

void FishInfo::setScore(int target, int value)
{
    //wxMutexLocker locker(*lock);
    fishScore[target] = value;
}

void FishInfo::addFish()
{
    ++fishCount;
}
