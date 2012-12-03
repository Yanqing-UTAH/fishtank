#include "fish.h"
#include "cstdlib"
#pragma once
const int Dead = -1;
const int Active = 1;
const int Revive = 0;

struct character//属性
{
    int HP;//HP
    int guessSpeed;//速度估计
    int guessLevel;//等级估计
    int guessMaxHP;//最大HP估计
    int guessAtt;
    int Status;//状态(Dead, Active, Revive)
    int X, Y;//坐标(主要作用为记录前一轮坐标以估算速度)
    bool Flag;//扫描标示
};


class st35:public fish{
    private:

    int Map[M + 2][N + 2];//战场情况
    int ValueMap[M + 2][N + 2];//走位估价表
    character Fishes[N + 2];

    int MyID;//我的ID
    int MyX, MyY;//我的坐标
    int MyHP, MySp, MyAtt, MyMaxHP;//我的属性
    int MyLv, MyExp, MaxExp;//我的等级、经验值和下一等级经验下限
    int LeftPoint;//我的剩余点数

    int Round;//回合数

    int Status;

    bool incStr(int);//增加力量n点
    bool incHth(int);//增加血量n点
    bool incSpd(int);//增加速度n点
    int Distance(int, int, int ,int);

    void ScanBattlefield();//扫描战场

    int Condition();

    void Play_when_Weak();//虚弱状态下操作
    void Evaluate_when_Weak();//虚弱状态下的估价函数
    void Play_when_Normal();//正常状态下的操作
    void Evaluate_when_Normal();//正常状态下的估价函数
    void Play_when_Frenzy();//疯狂状态下的操作
    void Evaluate_when_Frenzy();//疯狂状态下的估价函数
    void Play_when_Dying();
    void Evaluate_when_Dying();
    void Evaluate_when_Revive();

    void FindBest(int&, int&, int, int);

    void Level_Up(int);

    int distance(int, int, int, int);

    public:
    void init();

    void play();

    void revive(int&,int&);

};


//可自行增加所需函数所需函数及变量，但需保证上面每个函数的完整与可调用

//不要使用全局变量！！


/*在revive和play中同时加round，添加一个Flag表示该轮play前是否进行了revive
死亡隔一轮后复活，可以估计所有人Speed
添加前轮信息记录
扩充他人信息记录
Dying状态后期需要修改
留点
Level_Up()还没写
打算level mod 3 = 0时加Att，其余加速度，每次均加Health
*/
