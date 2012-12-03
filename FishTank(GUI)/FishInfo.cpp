#include "FishInfo.h"
#include "Events.h"
#include "FishTankApp.h"

int FishInfo::getPoint(int target) const
{
    return fishPoint[target];
}

int FishInfo::getLevel(int target) const
{
    return fishLevel[target];
}

int FishInfo::getExp(int target) const
{
    return fishExp[target];
}

int FishInfo::getX(int target) const
{
    return fishX[target];
}

int FishInfo::getY(int target) const
{
    return fishY[target];
}

int FishInfo::getHP(int target) const
{
    return fishHP[target];
}

int FishInfo::getMaxHP(int target) const
{
    return fishMaxHP[target];
}

int FishInfo::getAtt(int target) const
{
    return fishAtt[target];
}

int FishInfo::getSp(int target) const
{
    return fishSp[target];
}

int FishInfo::askWhat(int x, int y) const
{
    return map[x][y];
}

void FishInfo::setMap(int x, int y, int target)
{
    if (validCor(x, y))
    {
        map[x][y] = target;
        wxSetMapEvent* event = new wxSetMapEvent(x, y, target, wxEVT_SET_MAP, SET_MAP_ID);
        ((FishTankApp*)wxTheApp)->frame->GetEventHandler()->QueueEvent(event);
    }
}

void FishInfo::setXY(int target, int x, int y)
{
    fishX[target] = x;
    fishY[target] = y;
}

void FishInfo::setHP(int target, int value)
{
    fishHP[target] = value;
}

void FishInfo::setMaxHP(int target, int value)
{
    fishMaxHP[target] = value;
}

void FishInfo::setAtt(int target, int value)
{
    fishAtt[target] = value;
}

void FishInfo::setSp(int target, int value)
{
    fishSp[target] = value;
}

void FishInfo::setExp(int target, int value)
{
    fishExp[target] = value;
}

void FishInfo::setLevel(int target, int value)
{
    fishLevel[target] = value;
}

void FishInfo::setPoint(int target, int value)
{
    fishPoint[target] = value;
}
