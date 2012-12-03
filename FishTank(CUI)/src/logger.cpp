#include "env.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

env::logger::logger(const char* filename)
{
    log.open(filename);
}

env::logger::~logger()
{
    log.close();
}

void env::logger::pFishStatus(int id, int lv, int hp, int maxhp, int sp, int att, int x, int y, int exp)
{
    log << "-------Status of Fish " << setw(2) << id << "------------" << endl;
    log << "Level: " << lv << endl;
    log << "HP: " << hp << "/ " << maxhp << endl;
    log << "Sp: " << sp << endl;
    log << "Att: " << att << endl;
    log << "Location: (" << x << ", " << y << ")" << endl;
    log << "Exp: " << exp << endl;
    log << "------------------------------------" << endl;
}

void env::logger::pFishTurn(int id)
{
    log << "------------------------------------" << endl;
    log << "------------------------------------" << endl;
    log << "Fish " << id << " is in action now" << endl;
}

void env::logger::pFishRevive(int id, int x, int y)
{
    log << "Fish " << id << " revives at (" << x << ", " << y << ")" << endl;
}

void env::logger::pPriorityDecided()
{
    log << "Priority is decided" << endl;
}

void env::logger::pFoodRefreshed()
{
    log << "Food is refreshed" << endl;
}

void env::logger::pRound(int round)
{
    log << "Round " << round << endl;
}

void env::logger::pFishMove(int id, int x1, int y1, int x2, int y2, int result)
{
    log << "Fish " << id << " moves from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ") : ";
    if (result == 0)
        log << "Success";
    else if (result == 1)
        log << "Fail : Target area is out of your move range";
    else if (result == 2)
        log << "Fail : Target area is not Empty";
    else if (result == 3)
        log << "Fail : Invalid coordinate";
    else if (result == 4)
        log << "Fail : Cannot move in this phase";
    log << endl;
}

void env::logger::pFishKilled(int id)
{
    log << "Fish " << id << " is killed" << endl;
}

void env::logger::pFishDecHP(int id, int hp)
{
    log << "Fish " << id << "'s HP has decreased to " << hp << endl;
}

void env::logger::pFishEatFood(int id)
{
    log << "Fish " << id << " eats food" << endl;
}

void env::logger::pFishIncExp(int id, int dExp)
{
    log << "Fish " << id << "'s Exp is increased by " << dExp << endl;
}

void env::logger::pFishLevelUp(int id, int lv)
{
    log << "Fish " << id << "'s level is upgraded to " << lv << endl;
}

void env::logger::pFishAttack(int id, int x, int y, int result)
{
    log << "Fish " << id << " attacks (" << x << ", " << y << ") : ";
    if (result == 0)
        log << "Success";
    else if (result == 1)
        log << "Fail : Attempted to attack an empty area";
    else if (result == 2)
        log << "Fail : Target area is out of your attack range";
    else if (result == 3)
        log << "Fail : Invalid coordinate";
    else if (result == 4)
        log << "Fail : Cannot attack in this phase";
    log << endl;
}

void env::logger::pFishIncHP(int id, int hp)
{
    log << "Fish " << id << "'s HP is " << hp << " now" << endl;
}

void env::logger::pFishIncMaxHP(int id, int maxhp)
{
    log << "Fish " << id << "'s MaxHP is " << maxhp << " now" << endl;
}

void env::logger::pFishIncSp(int id, int sp)
{
    log << "Fish " << id << "'s Sp is " << sp << " now" << endl;
}

void env::logger::pFishIncAtt(int id, int att)
{
    log << "Fish " << id << "'s Att is " << att << " now" << endl;
}
