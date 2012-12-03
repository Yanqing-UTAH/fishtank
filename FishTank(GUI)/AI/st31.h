#ifndef PLAYER_H
#define PLAYER_H

#include "fish.h"

const int INIT_HP = 4;
const int INIT_ATK = 1;
const int INIT_SPD = 5;
const int SAFETY = 51;
const int INF = 10000000;
const int ATK1_MOVE1 = INF;
const int ATK2_MOVE1 = 5000;
const int ATK3_MOVE1 = 1000;
const int ATK1_MOVE2 = 15000;
const int ATK1_MOVE3 = 3000;
const int ATK2_MOVE2 = 1500;
const int REVIVE_SAFETY = 20;
const int FOOD_VALUE = 10000;
const int INIT_FOOD_VALUE = 7;
const int DX[5] = {0,1, 0, -1, 0};
const int DY[5] = {0,0, 1, 0, -1};

struct enemy
{
    enemy();
    ~enemy();
    void setXY(int,int);
    void setHP(int);
    void setAtk(int);
    void setLive(bool);
    void update(const enemy*, int map[][M + 1]);
    int exp();
    int x;
    int y;
    int HP;
    int maxHP;
    int hurt;
    int level;
    int atk;
    int spd;
    int move;
    bool live;
    int deadTime;
};

class st31 : public fish
{
    public:
        st31();
        ~st31();

        void scan();
        void decide(int&,int&,int&,int&);
        void initDecide(int&,int&,int&,int&);
        void upgrade();
        void update();
        void calSafe(int,int,int&,int&);
        bool isSafe(int,int,int);
        int calValue(int,int);
        int calSingleValue(int,int);
        int initCalValue(int,int);
        bool initIsSafe(int,int);

        virtual void init();
        virtual void play();
        virtual void revive(int&,int&);
    private:
        int round;
        int myID;
        int HP;
        int maxHP;
        int atk;
        int spd;
        int x;
        int y;
        int map[N + 1][M + 1];
        enemy enemies[200];
        int dist(int,int,int,int);
        int ceil(int,int);
};

struct node
{
    int x,y,v;
    node (int tx = 0,int ty = 0,int tv = 0)
    {
        x = tx;
        y = ty;
        v = tv;
    }
};
#endif // PLAYER_H

