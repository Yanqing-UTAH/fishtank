//等级估算公式：
//Level = ((MaxHP / 2 + Speed) + InitAtt - 7) / Concern_for_Hth&Spd
//由于MaxHP和Speed只能估算，InitAtt和Concern_for_Hth&Spd为猜测值，所有除法取ceiling——+0.99后取floor，存在类型转换！
//Concer_for_Hth&Spd为float，存在类型转换！
//Concer_for_Hth&Spd应该在2左右
//估算杀人价值时，等级按Level-1算

//Att估算公式：
//Att=(7 + 3 * Level) - MaxHP / 2 - Speed
//除法取floor


#include "st35.h"
#include <cstdlib>
#define MyName st35
using namespace std;

const int Concer_for_Hth_Spd = 2;//对其他人对血量和速度的关注程度的猜测，将影响Level的估算
const int InitAtt = 3;//对其他人初始攻击力的猜测

const int Weak = -1;//虚弱态
const int Dying = 0;//濒死态
const int Normal = 1;//正常态
const int Frenzy = 2;//狂热态

const int initHealth = 3;//初始生命
const int initSpeed = 6;//初始速度
const int initStrenth = 1;//初始力量

const int FrenzyRound = 20;
const float WeakPoint = 0.6;
const float DyingPoint = 0.3;

const int dir[4][2] = {-1, 0,
                       1, 0,
                       0, -1,
                       0, 1
                      };

inline int Max(int a, int b)
{
    return (a > b) ? a : b;
}

inline int Min(int a, int b)
{
    return (a < b) ? a : b;
}
void MyName::init()
{
    Round = 0;
    MyID = getID();
    incStr(initStrenth);
    incHth(initHealth);
    incSpd(initSpeed);
    Status = Active;
}

void MyName::play()
{
    if (Status == Active)
    {
        Round++;
    }
    else Status = Active;

    ScanBattlefield();
    switch (Condition())
    {
    case Weak:
        Play_when_Weak();
        break;
    case Normal:
        Play_when_Normal();
        break;
    case Frenzy:
        Play_when_Frenzy();
        break;
    case Dying:
        Play_when_Dying();
        break;
    }
    while (MyLv < getLevel())
    {
        ++MyLv;
        Level_Up(MyLv);
    }
}

void MyName::revive(int& x, int& y)
{
    Evaluate_when_Revive();
    FindBest(x, y, 0, 0);
    Round += 2;
    Status = Revive;
    while ((Condition() == Dying)&&(incHth(1)));
}



bool MyName::incStr(int n)
{
    int i;
    bool f = true;

    for(i = 0; i < n; i++)
    {
        f = increaseStrength();
    }
    return f;
}

bool MyName::incHth(int n)
{
    int i;
    bool f = true;

    for (i = 0; i < n; i++)
    {
        f = increaseHealth();
    }
    return f;
}

bool MyName::incSpd(int n)
{
    int i;
    bool f = true;

    for (i = 0; i< n; i++)
    {
        f = increaseSpeed();
    }
    return f;
}


int MyName::Distance(int x1, int y1, int x2, int y2)
{
    return ((x1 > x2) ? (x1 - x2) : (x2 - x1)) + ((y1 > y2) ? (y1 - y2) : (y2 - y1));
}


void MyName::ScanBattlefield()
{
    int i, j;
    int Dist;

    for (i = 1; i <= MAX_PLAYER; i++)
    {
        Fishes[i].Flag = false;
    }
    for (i = 1; i <= M; i++)
    {
        for (j = 1; j <= N; j++)
        {
            Map[i][j] = askWhat(i, j);
            switch (Map[i][j])
            {
            case -1:
                Fishes[Map[i][j]].HP = 1;
                break;
            case 0:
                Fishes[Map[i][j]].HP = 0;
                break;
            default:
                Fishes[Map[i][j]].HP = askHP(Map[i][j]);
                Fishes[Map[i][j]].guessMaxHP = Max(Fishes[Map[i][j]].guessMaxHP, Fishes[Map[i][j]].HP);

                Dist = Distance(i, j, Fishes[Map[i][j]].X, Fishes[Map[i][j]].Y);
                Fishes[Map[i][j]].X = i;
                Fishes[Map[i][j]].Y = j;
                switch (Fishes[Map[i][j]].Status)
                {
                case Dead:
                    Fishes[Map[i][j]].Status++;
                    break;
                case Revive:
                    Fishes[Map[i][j]].Status++;
                case Active:
                    Fishes[Map[i][j]].guessSpeed = Max(Fishes[Map[i][j]].guessSpeed, Dist);
                    break;
                }

                Fishes[Map[i][j]].guessLevel = Max(1, (int)((double)(((int)(Fishes[Map[i][j]].guessMaxHP / 2.0 + 0.99) + InitAtt - 7)) / Concer_for_Hth_Spd + 0.99));
                Fishes[Map[i][j]].guessAtt = (7 + 3 * Fishes[Map[i][j]].guessLevel) - Fishes[Map[i][j]].guessMaxHP / 2 - Fishes[Map[i][j]].guessSpeed;//攻击力估计


            }
        }
    }
    MyX = getX();
    MyY = getY();
    MyHP = getHP();
    MyMaxHP = getMaxHP();
    MyAtt = getAtt();
    MySp = getSp();
    LeftPoint = getPoint();
    MyLv = getLevel();
    MyExp = getExp();
    MaxExp = (MyLv == 1) ? 2 : (2 * MyLv + MyLv * (MyLv - 1) / 2);
}

int MyName::Condition()
{
    int c;

    if (Round > FrenzyRound) c = Frenzy;
    else c = Normal;
    if (MyHP <= (int)(MyMaxHP * WeakPoint)) c = Weak;
    if (MyHP <= (int)(MyMaxHP * DyingPoint)) c = Dying;

    return c;
}

void MyName::Level_Up(int Lv)
{
    incHth(1);
    if (getSp() > 40)
        incStr(1);
    else
        (Lv % 3 == 1) ? incStr(1) : incSpd(1);
}

void MyName::FindBest(int& x, int& y, int MyX, int MyY)
{
    double MaxValue = -999999.9;
    x = MyX;
    y = MyY;

    for (int i = 1; i <= M; ++i)
    {
        for (int j = 1; j <= N; ++j)
        {
            if ((ValueMap[i][j] > MaxValue) && (Distance(i, j, MyX, MyY) <= MySp))
            {
                MaxValue = ValueMap[i][j];
                x = i;
                y = j;
            }
			if ((ValueMap[i][j] == MaxValue) && (Distance(i, j, MyX, MyY) <= MySp) && (rand() / 32767.0 < 0.5))
            {
                MaxValue = ValueMap[i][j];
                x = i;
                y = j;
            }
        }
    }
}

void MyName::Evaluate_when_Revive()
{
    for (int i = 0; i <= M + 1; ++i)
    {
        for (int j = 0; j <= N + 1; ++j)
        {
            ValueMap[i][j] = 0;
        }
    }
    for (int i = 1; i <= M; ++i)
    {
        for (int j = 1; j <= N; ++j)
        {
            for(int p = 1; p <= M; ++p)
            {
                for (int q = 1; q <= N; ++q)
                {
                    ValueMap[i][j] -= Fishes[Map[i][j]].guessAtt / (1 + (Distance(i, j, p, q)));
                }
            }
        }
    }
    while ((Condition() == Dying) && incHth(1));
}

void MyName::Play_when_Dying()
{
    int potentialAtt = Min(2, LeftPoint);
    int p, q;
    int d, maxValue = 0;

    Evaluate_when_Dying();
    FindBest(p, q, MyX, MyY);
    move(p, q);

    d = 0;
    for (int i = 0; i < 4; ++i)
    {
        int Value;

        int x = p + dir[i][0];
        int y = q + dir[i][1];
        switch (Map[x][y])
        {
        case EMPTY:
            Value = 0;
            break;
        case FOOD:
            Value =20 + 10 * Min(MyMaxHP - MyHP, Max(2, MyMaxHP / 10));
            break;
        default:
            if (askHP(Map[x][y]) < MyAtt + potentialAtt) Value = askHP(Map[x][y]) * 1000;
            else Value = MyAtt / 10 + 1;
        }
        if (Value > maxValue)
        {
            maxValue = Value;
            d = i;
        }
    }
    int x = p + dir[d][0];
    int y = q + dir[d][1];
    if (Map[x][y] > 0)
    {
        if (askHP(Map[x][y]) < MyAtt + potentialAtt)
        {
            int att = 0;
            while ((askHP(Map[x][y]) < MyAtt + att) && (incStr(1) && ++att));
            attack(x, y);
        }
        else attack(x, y);
    }
    else attack(x, y);
    while ((Condition() == Dying) && incHth(1));
}

void MyName::Evaluate_when_Dying()
{
    int potentialAtt = Min(2, LeftPoint);

    for (int i = 0; i <= M + 1; ++i)
    {
        for (int j = 0; j <= N + 1; ++j)
        {
            ValueMap[i][j] = 0;
        }
    }
    for (int i = 1; i <= M; ++i)
    {
        for (int j = 1; j <= N; ++j)
        {
            if (Map[i][j] != 0) ValueMap[i][j] = -999999.9;
            else
            {
                for (int p = 1; p <= M; ++p)
                {
                    for (int q = 1; q <= N; ++q)
                    {
                        if (!((p == i) && (q == j)) && (Map[p][q] > 0)) ValueMap[i][j] -= (Fishes[Map[i][j]].guessAtt /
                                    (1 + (Distance(i, j, p, q) / Max(1, Fishes[Map[i][j]].guessSpeed)))
                                    + Fishes[Map[i][j]].guessAtt / (1 + (Distance(i, j, p, q))));
                    }
                }

                int MaxValue = -999999.9;

                for (int k = 0; k < 4; ++k)
                {
                    int x = i + dir[k][0];
                    int y = j + dir[k][1];
                    int Value = 0;
                    switch (Map[x][y])
                    {
                    case EMPTY:
                        Value = 0;
                        break;
                    case FOOD:
                        Value = 20 + 10 * Min(MyMaxHP - MyHP, Max(2, MyMaxHP / 10));
                        break;
                    default:
                        if (askHP(Map[x][y]) < MyAtt + potentialAtt) Value = askHP(Map[x][y]) * 1000;
                        else Value = MyAtt / 10 + 1;
                    }
                    if (Value > MaxValue) MaxValue = Value;
                }
                ValueMap[i][j] += MaxValue;
            }
        }
    }
}

void MyName::Play_when_Normal()
{
    int potentialAtt = LeftPoint;
    int p, q;
    int d, maxValue = 0;

    Evaluate_when_Normal();
    FindBest(p, q, MyX, MyY);
    move(p, q);

    d = 0;
    for (int i = 0; i < 4; ++i)
    {
        int Value;

        int x = p + dir[i][0];
        int y = q + dir[i][1];
        switch (Map[x][y])
        {
        case EMPTY:
            Value = 0;
            break;
        case FOOD:
            Value =5 + Min(MyMaxHP - MyHP, Max(2, MyMaxHP / 10));
            break;
        default:
            if (askHP(Map[x][y]) < MyAtt + potentialAtt) Value = askHP(Map[x][y]) * 1000;
            else Value = MyAtt / 2 + 1;
        }
        if (Value > maxValue)
        {
            maxValue = Value;
            d = i;
        }
    }
    int x = p + dir[d][0];
    int y = q + dir[d][1];
    if (Map[x][y] > 0)
    {
        if (askHP(Map[x][y]) < MyAtt + potentialAtt)
        {
            int att = 0;
            while ((askHP(Map[x][y]) < MyAtt + att) && (incStr(1) && ++att));
            attack(x, y);
        }
        else attack(x, y);
    }
    else attack(x, y);
}//正常状态下的操作

void MyName::Evaluate_when_Normal()
{
    int potentialAtt = LeftPoint;

    for (int i = 0; i <= M + 1; ++i)
    {
        for (int j = 0; j <= N + 1; ++j)
        {
            ValueMap[i][j] = 0;
        }
    }
    for (int i = 1; i <= M; ++i)
    {
        for (int j = 1; j <= N; ++j)
        {
            if (Map[i][j] != 0) ValueMap[i][j] = -999999.9;
            else
            {
                for (int p = 1; p <= M; ++p)
                {
                    for (int q = 1; q <= N; ++q)
                    {
                        if (!((p == i) && (q == j))&&(Map[p][q] > 0)) ValueMap[i][j] -= 0.2* (Fishes[Map[i][j]].guessAtt /
                                    (1 + (Distance(i, j, p, q) / Max(1, Fishes[Map[i][j]].guessSpeed)))
                                    + Fishes[Map[i][j]].guessAtt / (1 + (Distance(i, j, p, q))));
                        if ((Map[p][q] == FOOD) && (Distance(i, j, MyX, MyY) <= MySp) && (Distance(p, q, i, j) <=MySp)) ValueMap[i][j] += 20.0 / (1 + (Distance(i, j, MyX, MyY) + Distance(p, q, i, j)));
                    }
            }

            int MaxValue = -999999.9;

            for (int k = 0; k < 4; ++k)
                {
                    int x = i + dir[k][0];
                    int y = j + dir[k][1];
                    int Value = 0;
                    switch (Map[x][y])
                    {
                    case EMPTY:
                        Value = 0;
                        break;
                    case FOOD:
                        Value = 5 + Min(MyMaxHP - MyHP, Max(2, MyMaxHP / 10));
                        break;
                    default:
                        if (askHP(Map[x][y]) < MyAtt + potentialAtt) Value = askHP(Map[x][y]) * 1000;
                        else Value = MyAtt / 2 + 1;
                    }
                    if (Value > MaxValue) MaxValue = Value;
                }
                ValueMap[i][j] += MaxValue;
            }
        }
    }
}//正常状态下的估价函数

void MyName::Play_when_Weak()
{
    int potentialAtt = LeftPoint;
    int p, q;
    int d, maxValue = 0;

    Evaluate_when_Weak();
    FindBest(p, q, MyX, MyY);
    move(p, q);

    d = 0;
    for (int i = 0; i < 4; ++i)
    {
        int Value;

        int x = p + dir[i][0];
        int y = q + dir[i][1];
        switch (Map[x][y])
        {
        case EMPTY:
            Value = 0;
            break;
        case FOOD:
            Value =20 + 5 * Min(MyMaxHP - MyHP, Max(2, MyMaxHP / 10));
            break;
        default:
            if (askHP(Map[x][y]) < MyAtt + potentialAtt) Value = askHP(Map[x][y]) * 1000;
            else Value = MyAtt / 5 + 1;
        }
        if (Value > maxValue)
        {
            maxValue = Value;
            d = i;
        }
    }
    int x = p + dir[d][0];
    int y = q + dir[d][1];
    if (Map[x][y] > 0)
    {
        if (askHP(Map[x][y]) < MyAtt + potentialAtt)
        {
            int att = 0;
            while ((askHP(Map[x][y]) < MyAtt + att) && (incStr(1) && ++att));
            attack(x, y);
        }
        else attack(x, y);
    }
    else attack(x, y);
} //虚弱状态下操作

void MyName::Evaluate_when_Weak()
{
    int potentialAtt = LeftPoint;

    for (int i = 0; i <= M + 1; ++i)
    {
        for (int j = 0; j <= N + 1; ++j)
        {
            ValueMap[i][j] = 0;
        }
    }
    for (int i = 1; i <= M; ++i)
    {
        for (int j = 1; j <= N; ++j)
        {
            if (Map[i][j] != 0) ValueMap[i][j] = -999999.9;
            else
            {
                for (int p = 1; p <= M; ++p)
                {
                    for (int q = 1; q <= N; ++q)
                    {
                        if (!((p == i) && (q == j))&&(Map[p][q] > 0)) ValueMap[i][j] -= 0.5* (Fishes[Map[i][j]].guessAtt /
                                    (1 + (Distance(i, j, p, q) / Max(1, Fishes[Map[i][j]].guessSpeed)))
                                    + Fishes[Map[i][j]].guessAtt / (1 + (Distance(i, j, p, q))));
                        if ((Map[p][q] == FOOD) && (Distance(i, j, MyX, MyY) <= MySp) && (Distance(p, q, i, j) <=MySp)) ValueMap[i][j] += 20.0 / (1 + (Distance(i, j, MyX, MyY) + Distance(p, q, i, j)));
                    }
            }

            int MaxValue = -999999.9;

            for (int k = 0; k < 4; ++k)
                {
                    int x = i + dir[k][0];
                    int y = j + dir[k][1];
                    int Value = 0;
                    switch (Map[x][y])
                    {
                    case EMPTY:
                        Value = 0;
                        break;
                    case FOOD:
                        Value = 20 + 5 * Min(MyMaxHP - MyHP, Max(2, MyMaxHP / 10));
                        break;
                    default:
                        if (askHP(Map[x][y]) < MyAtt + potentialAtt) Value = askHP(Map[x][y]) * 1000;
                        else Value = MyAtt / 5 + 1;
                    }
                    if (Value > MaxValue) MaxValue = Value;
                }
                ValueMap[i][j] += MaxValue;
            }
        }
    }
} //虚弱状态下的估价函数

void MyName::Play_when_Frenzy()
{
    int potentialAtt = LeftPoint;
    int p, q;
    int d, maxValue = 0;

    Evaluate_when_Normal();
    FindBest(p, q, MyX, MyY);
    move(p, q);

    d = 0;
    for (int i = 0; i < 4; ++i)
    {
        int Value;

        int x = p + dir[i][0];
        int y = q + dir[i][1];
        switch (Map[x][y])
        {
        case EMPTY:
            Value = 0;
            break;
        case FOOD:
            Value =5 + Min(MyMaxHP - MyHP, Max(2, MyMaxHP / 10));
            break;
        default:
            if (askHP(Map[x][y]) < MyAtt + potentialAtt) Value = askHP(Map[x][y]) * 1000;
            else Value = MyAtt + 1;
        }
        if (Value > maxValue)
        {
            maxValue = Value;
            d = i;
        }
    }
    int x = p + dir[d][0];
    int y = q + dir[d][1];
    if (Map[x][y] > 0)
    {
        if (askHP(Map[x][y]) < MyAtt + potentialAtt)
        {
            int att = 0;
            while ((askHP(Map[x][y]) < MyAtt + att) && (incStr(1) && ++att));
            attack(x, y);
        }
        else attack(x, y);
    }
    else attack(x, y);
} //疯狂状态下的操作


void MyName::Evaluate_when_Frenzy()
{
    int potentialAtt = LeftPoint;

    for (int i = 0; i <= M + 1; ++i)
    {
        for (int j = 0; j <= N + 1; ++j)
        {
            ValueMap[i][j] = 0;
        }
    }
    for (int i = 1; i <= M; ++i)
    {
        for (int j = 1; j <= N; ++j)
        {
            if (Map[i][j] != 0) ValueMap[i][j] = -999999.9;
            else
            {
                for (int p = 1; p <= M; ++p)
                {
                    for (int q = 1; q <= N; ++q)
                    {
                        if ((Map[p][q] == FOOD) && (Distance(i, j, MyX, MyY) <= MySp) && (Distance(p, q, i, j) <=MySp)) ValueMap[i][j] += 10 / (1 + (Distance(i, j, MyX, MyY) + Distance(p, q, i, j)));
                        if ((Map[p][q] > 0) && (Distance(i, j, MyX, MyY) <= MySp) && (Distance(p, q, i, j) <=MySp)) ValueMap[i][j] += Max((2 * MyAtt - Fishes[Map[p][q]].HP) , 0) / (1 + (Distance(i, j, MyX, MyY) + Distance(p, q, i, j)));
                    }
            }

            int MaxValue = -999999.9;

            for (int k = 0; k < 4; ++k)
                {
                    int x = i + dir[k][0];
                    int y = j + dir[k][1];
                    int Value = 0;
                    switch (Map[x][y])
                    {
                    case EMPTY:
                        Value = 0;
                        break;
                    case FOOD:
                        Value = 5 + Min(MyMaxHP - MyHP, Max(2, MyMaxHP / 10));
                        break;
                    default:
                        if (askHP(Map[x][y]) < MyAtt + potentialAtt) Value = askHP(Map[x][y]) * 1000;
                        else Value = MyAtt + 1;
                    }
                    if (Value > MaxValue) MaxValue = Value;
                }
                ValueMap[i][j] += MaxValue;
            }
        }
    }
} //疯狂状态下的估价函数
