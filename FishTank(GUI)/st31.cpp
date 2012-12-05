#include "st31.h"

#include <cstdlib>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

st31::st31()
{
    round = 0;
    myID = 0;
    HP = 0;
    maxHP = 0;
    atk = 0;
    spd = 0;
    x = 0;
    y = 0;
    memset(map,0,sizeof(map));
    for (int i = 1; i <= MAX_PLAYER; i++)
        enemies[i] = enemy();
}

st31::~st31()
{
}

void st31::init()
{
    int i;
    for (i = 0; i < INIT_HP; i++)
        increaseHealth();
    for (i = 0; i < INIT_ATK; i++)
        increaseStrength();
    for (i = 0;i < INIT_SPD; i++)
        increaseSpeed();
    update();
    myID = getID();
}

void st31::upgrade()
{
    update();

    int i,tot = 0,sum = 0;
    int spdList[MAX_PLAYER + 1];

    increaseHealth();
    /*while (!isSafe(x,y,100) && getPoint() > 0)
        increaseHealth();*/
    if (getPoint() == 0) return;
    for (i = 1; i <= MAX_PLAYER; i++)
        if (i != myID)
            spdList[++tot] = enemies[i].spd;
    for (i = 1; i <= tot; i++)
        if (spdList[i] > spd)
            sum ++;
    if (sum > 5) increaseSpeed();
    else increaseHealth();
    if (round <= 100) increaseStrength();
    else
    {
        tot = 0;
        for (i = 1; i <= MAX_PLAYER; i++)
            if (i != myID && enemies[i].deadTime >= 10 && atk < enemies[i].maxHP)
                tot ++;
        if (tot > 2) increaseStrength();
        else increaseHealth();
    }

}

void st31::update()
{
    HP = getHP();
    maxHP = getMaxHP();
    atk = getAtt();
    spd = getSp();
    x = getX();
    y = getY();
}

void st31::scan()
{
    int i,j;
    bool vis[200];

    memset(map,0,sizeof(map));
    memset(vis,0,sizeof(vis));

    for (i = 1; i <= N; i++)
        for (j = 1; j <= M; j++)
        {
            map[i][j] = askWhat(i,j);
            if (map[i][j] != FOOD && map[i][j] != EMPTY && map[i][j] != myID)
            {
                enemy& e = enemies[map[i][j]];
                e.setXY(i,j);
                e.setHP(askHP(map[i][j]));
                vis[map[i][j]] = true;
            }
        }

    int mLev = 0,ans = 0,eID,x1,y1,x2,y2;

    for (i = 1; i <= MAX_PLAYER; i++)
    if (i != myID)
    {
        if (vis[i] == false)
        {
            x1 = enemies[i].x;
            y1 = enemies[i].y;
            for (j = 1; j <= 4; j++)
            if (x1 + DX[j] >= 1 && x1 + DX[j] <= N && y1 + DY[j] >= 1 && y1 + DY[j] <= M)
            {
                x2 = x1 + DX[j];
                y2 = y1 + DY[j];
                if (map[x2][y2] > 0 && map[x2][y2] != myID)
                {
                    eID = map[x2][y2];
                    if (enemies[eID].level > mLev)
                    {
                        ans = eID;
                        mLev = enemies[eID].level;
                    }
                }
            }
            if (ans > 0) enemies[ans].setAtk(enemies[i].HP);
        }
        enemies[i].setLive(vis[i]);
        enemies[i].update(enemies,map);
    }
}

int st31::dist(int x1,int y1,int x2,int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

int st31::ceil(int a,int b)
{
    if (a % b == 0) return a / b;
    else return a / b + 1;
}

int st31::calValue(int x,int y)
{
    int i,j;
    int ans = 0;

    for (i = 1; i <= 4; i++)
    if (x + DX[i] >= 1 && x + DX[i] <= N && y + DY[i] >= 1 && y + DY[i] <= M
        && askWhat(x + DX[i],y + DY[i]) != EMPTY && askWhat(x + DX[i],y + DY[i]) != myID)
    {
        if (askWhat(x + DX[i],y + DY[i]) > 0)
        {
            enemy& eID = enemies[askWhat(x + DX[i],y + DY[i])];
            if (atk >= eID.HP)
                ans += ATK1_MOVE1 * eID.exp();
            else if (2 * atk - eID.atk >= 2 * eID.HP)
                ans += ATK2_MOVE1 * eID.exp();
            else if (3 * atk - 2 * eID.atk >= 3 * eID.HP)
                ans += ATK3_MOVE1 * eID.exp();
        }
        else if (askWhat(x + DX[i],y + DY[i]) == FOOD)
        {
            if (round <= INIT_FOOD_VALUE) ans += INF / 10;
            else ans += int (FOOD_VALUE * pow(maxHP * 1.0 / HP,2));
        }
    }

    int x1 = max(0,x - 2 * spd);
    int x2 = min(N,x + 2 * spd);
    int y1 = max(0,y - 2 * spd);
    int y2 = min(M,y + 2 * spd);

    for (i = x1; i <= x2; i++)
        for (j = y1; j <= y2; j++)
        if (dist(i,j,x,y) > 1 && askWhat(i,j) != EMPTY && askWhat(i,j)!= myID)
        {
            if (askWhat(i,j) > 0)
            {
                enemy& eID = enemies[askWhat(i,j)];
                if (2 * spd - eID.spd >= 2 * dist(i,j,x,y) && atk >= eID.HP)
                    ans += ATK1_MOVE2 * eID.exp();
                else if (3 * spd - 2 * eID.spd >= 3* dist(i,j,x,y) && atk >= eID.HP)
                    ans += ATK1_MOVE3 * eID.exp();
                else if (2 * spd - eID.spd >= 2 * dist(i,j,x,y) && 2 * atk - eID.atk >= 2 * eID.HP)
                    ans += ATK2_MOVE2 * eID.exp();
            }
            else
            {
                if (round <= INIT_FOOD_VALUE) ans += INF / 100;
                else ans += int (FOOD_VALUE * pow(maxHP * 1.0 / HP,2) / (ceil(dist(i,j,x,y),spd) * 5 + 5));
            }
        }
    return ans;
}

void st31::calSafe(int x,int y,int& dam1,int& dam2)
{
    int i,j,k;
    int damage = 0;
    bool eat = false;

    for (i = 1; i <= 4; i++)
    if (x + DX[i] >= 1 && x + DX[i] <= N && y + DY[i] >= 1 && y + DY[i] <= M)
    {
        if (askWhat(x + DX[i],y + DY[i]) > 0 && askWhat(x + DX[i],y + DY[i])!= myID)
        {
            enemy& eID = enemies[askWhat(x + DX[i],y + DY[i])];
            if (atk > askHP(askWhat(x + DX[i],y + DY[i]))) continue;
            damage += eID.atk;
        }
        else if (askWhat(x + DX[i],y + DY[i]) == FOOD && !eat)
        {
            damage -= max(2,HP / 10);
            eat = true;
        }
    }

    int x1 = max(1,x - spd);
    int x2 = min(N,x + spd);
    int y1 = max(1,y - spd);
    int y2 = min(M,y + spd);
    int damSum = 0;
    int sum = 0;
    int dam = 0;

    for (i = x1; i <= x2; i++)
        for (j = y1; j <= y2; j++)
        if (dist(x,y,i,j) <= spd && askWhat(i,j) == EMPTY)
        {
            dam = 0;
            eat = false;
            for (k = 1; k <= 4; k++)
            if (i + DX[k] >= 1 && i + DX[k] <= N && j + DY[k] >= 1 && j + DY[k] <= M)
            {
                if (askWhat(i + DX[k],j + DY[k]) > 0 && askWhat(i + DX[k],j + DY[k]) != myID)
                {
                    enemy& eID = enemies[askWhat(i + DX[i],j + DY[i])];
                    dam += eID.atk;
                }
                else if (askWhat(i + DX[k],j + DY[k]) == FOOD && !eat)
                {
                    eat = true;
                    dam -= max(2,HP / 10);
                }
            }
            damSum += dam;
            sum ++;
        }
    dam1 = damage;
    dam2 = damSum;
}

int st31::calSingleValue(int x,int y)
{
    int ans = 0;

    if (askWhat(x,y) > 0 && askWhat(x,y) != myID)
    {
        enemy& eID = enemies[askWhat(x,y)];
        if (atk >= eID.HP)
            ans = ATK1_MOVE1 * eID.exp();
        else if (2 * atk - eID.atk >= 2 * eID.HP)
            ans = ATK2_MOVE1 * eID.exp();
        else if (3 * atk - 2 * eID.atk >= 3 * eID.HP)
            ans = ATK3_MOVE1 * eID.exp();
    }
    else if (askWhat(x, y) == FOOD)
    {
        if (round <= INIT_FOOD_VALUE) ans += INF / 10;
        else ans += int (FOOD_VALUE * pow(maxHP * 1.0 / HP,2));
    }
    return ans;
}

void st31::decide(int& px,int& py,int& ax,int& ay)
{
    int i,j;
    int x1 = max(1,x - spd);
    int x2 = min(N,x + spd);
    int y1 = max(1,y - spd);
    int y2 = min(M,y + spd);
    int ans = 0;
    int ansSafe = INF;
    int value = 0;
    vector<node> decision;
    decision.clear();

    for (i = x1; i <= x2; i++)
        for (j = y1; j <= y2; j++)
        if (dist(x,y,i,j) <= spd && askWhat(i,j) == EMPTY)
        {
            value = calValue(i,j);
            decision.push_back(node(i,j,value));
        }

    int safety = SAFETY;
    int dam1 = 0;
    int dam2 = 0;
    bool found = false;

    while (safety <= 100)
    {
        ans = 0;
        ansSafe = INF;
        for (i = 0; i < decision.size(); i++)
        {
            calSafe(decision[i].x,decision[i].y,dam1,dam2);
            if (dam1 * 100 > safety * HP) continue;
            if (decision[i].v < ans) continue;
            if (decision[i].v > ans)
            {
                found = true;
                px = decision[i].x;
                py = decision[i].y;
                ans = decision[i].v;
                ansSafe =dam2;
            }
            else if (dam2 < ansSafe)
            {
                found = true;
                px = decision[i].x;
                py = decision[i].y;
                ans = decision[i].v;
                ansSafe =dam2;
            }
        }
        if (found) break;
        safety += 1;
    }

    ans = 0;
    for (i = 1; i <= 4; i++)
    if (px + DX[i] >= 1 && px + DX[i] <= N && py + DY[i] >= 1 && py + DY[i] <= M)
    {
        value = calSingleValue(px + DX[i],py + DY[i]);
        if (value > ans)
        {
            ans = value;
            ax = px + DX[i];
            ay = py + DY[i];
        }
    }
}

void st31::play()
{
    int px = 0,py = 0,ax = 0,ay = 0;

    round ++;

    update();
    scan();

    decide(px,py,ax,ay);

    move(px,py);
    if (ax > 0 && ay > 0)attack(ax,ay);

    upgrade();
}

bool st31::isSafe(int x,int y,int safety)
{
    int i,j,k;
    int damage = 0;
    bool eat = false;

    for (i = 1; i <= 4; i++)
    if (x + DX[i] >= 1 && x + DX[i] <= N && y + DY[i] >= 1 && y + DY[i] <= M)
    {
        if (map[x + DX[i]][y + DY[i]]> 0 && map[x + DX[i]][y + DY[i]] != myID)
        {
            enemy& eID = enemies[map[x + DX[i]][y + DY[i]]];
            damage += eID.atk;
        }
        else if (map[x + DX[i]][y + DY[i]] == FOOD && !eat)
        {
            damage -= max(2,HP / 10);
            eat = true;
        }
    }
    if (damage * 100 > safety * HP)
        return false;

    int x1 = max(1,x - spd);
    int x2 = min(N,x + spd);
    int y1 = max(1,y - spd);
    int y2 = min(M,y + spd);
    bool ok = false;

    for (i = x1; i <= x2; i++)
        for (j = y1; j <= y2; j++)
        if (dist(x,y,i,j) <= spd && map[i][j] == EMPTY)
        {
            damage = 0;
            eat = false;
            for (k = 1; k <= 4; k++)
            if (i + DX[k] >= 1 && i + DX[k] <= N && j + DY[k] >= 1 && j + DY[k] <= M)
            {
                if (map[i + DX[k]][j + DY[k]] > 0 && map[i + DX[k]][j + DY[k]] != myID)
                {
                    enemy& eID = enemies[map[i + DX[k]][j + DY[k]]];
                    damage += eID.atk;
                }
                else if (map[i + DX[k]][j + DY[k]] == FOOD && !eat)
                {
                    eat = true;
                    damage -= max(2,HP / 10);
                }
            }
            if (damage * 100 <= safety * HP) ok = true;
        }
    return ok;
}

void st31::revive(int& rx,int& ry)
{
    int i,j;
    int ans = 0;
    int value = 0;
    int safety = REVIVE_SAFETY;
    bool found = false;

    update();

    while (safety <= 100)
    {
        for (i = 1; i <= N; i++)
            for (j = 1; j <= M; j++)
            if (map[i][j] == EMPTY && isSafe(i,j,safety))
            {
                found = true;
                value = calValue(i,j);
                if (value > ans)
                {
                    rx = i;
                    ry = j;
                    ans = value;
                }
            }
        if (found) break;
        safety += 1;
    }
}

enemy::enemy()
{
    x = 0;
    y = 0;
    HP = 0;
    maxHP = 0;
    hurt = 0;
    atk = 0;
    spd = 0;
    move = 0;
    live = false;
    level = 0;
    deadTime = 0;
}

enemy::~enemy()
{
}

void enemy::setXY(int tx,int ty)
{
    if (level != 0 && x != 0 && y != 0)
        move = abs(x - tx) + abs(y - ty);
    x = tx;
    y = ty;
}

void enemy::setHP(int tHP)
{
    if (HP == 0)
    {
        HP = tHP;
        maxHP = max(maxHP,HP);
        return;
    }
    if (tHP <= HP) hurt = HP - tHP;
    HP = tHP;
    maxHP = max(maxHP,tHP);
}

void enemy::setAtk(int tatk)
{
    atk = max(tatk,atk);
}

int enemy::exp()
{
    return max(1,level / 2);
}

void enemy::update(const enemy* enemies,int map[][M + 1] )
{
    if (!live) return;

    if (level == 0)
    {
        level = 1;
        int left = 10 - maxHP / 2;
        atk = left / 2;
        spd = left / 2;
        return;
    }
    spd = max(spd,move);
    move = 0;
    level = (maxHP / 2 + spd + atk - 10) / 3 + 1;

    int i,j,x1,x2,y1,y2,totAtk;

    for (i = 1; i <= 4; i++)
        if (x + DX[i] >= 1 && x + DX[i] <= N && y + DY[i] >= 1 && y + DY[i] <= M)
        {
            x1 = x + DX[i];
            y1 = y + DY[i];
            if (map[x1][y1] <= 0) continue;
            const enemy& eID = enemies[map[x1][y1]];
            for (j = 1; j <= 4; j++)
                if (x1 + DX[j] >= 1 && x1 + DX[j] <= N && y1 + DY[j] >= 1 && y1 + DY[j] <= M)
                {
                    x2 = x1 + DX[j];
                    y2 = y1 + DY[j];
                    if (map[x2][y2] > 0)
                        totAtk += enemies[map[x2][y2]].atk;
                }
            atk = max(atk, eID.hurt * atk / totAtk);
        }
}

void enemy::setLive(bool isLive)
{
    if (isLive == false) deadTime++;
    live = isLive;
    if (!live)
    {
        x = 0;
        y = 0;
        hurt = 0;
        move = 0;
        HP = 0;
    }
}

