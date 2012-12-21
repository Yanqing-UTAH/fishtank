#include "st34.h"

using namespace std;

void st34::init()
{
    d[0][0] = -1;
    d[0][1] = 0;
    d[1][0] = 1;
    d[1][1] = 0;
    d[2][0] = 0;
    d[2][1] = -1;
    d[3][0] = 0;
    d[3][1] = 1;
    for (int i = 1; i <= 2; ++i)
        increaseHealth();
    for (int i = 1; i <= 1; ++i)
        increaseStrength();
    for (int i = 1; i <= 7; ++i)
        increaseSpeed();
    sx = -1;
    sy = -1;
    for (int i = 1; i <= MAX_PLAYER; ++i)
    {
        fX[i] = 0;
        fY[i] = 0;
        fMaxHP[i] = 4;
        fSp[i] = 3;
        fDie[i] = 0;
    }
}

void st34::play()
{
    sx = -1;
    sy = -1;
    while (getPoint())
        increasePoint();
    scan();
    int x0 = 0, y0 = 0, a = -1;
    for (int x = 1; x <= N; ++x)
        for (int y = 1; y <= M; ++y)
            calcSafe(x, y);
    if (eNow(x0, y0, a) > 0)
        if ((x0 >= 1) && (x0 <= N) && (y0 >= 1) && (y0 <= N))
            if (move(x0, y0))
            {
                if ((a >= 0) && (a <= 3))
                    attack(x0 + d[a][0], y0 + d[a][1]);
                while (getPoint())
                    increasePoint();
                return;
            }
    int x1 = -1, y1 = -1, x2 = -1, y2 = -1, minsv = 99999, msv = 99999, maxv = -1;
    for (int x = 1; x <= N; ++x)
        for (int y = 1; y <= M; ++y)
        {
            if (safe[x][y] - ff[x][y] < minsv)
            {
                minsv = safe[x][y] - ff[x][y];
                sx = x;
                sy = y;
            }
            if ((askWhat(x, y) == EMPTY) && (dis(x, y) <= getSp()))
            {
                int value = ff[x][y] - safe[x][y]  * (fDie[getID()] / 4 + 1) * getMaxHP() / getHP();
                if (value > maxv)
                {
                    maxv = value;
                    x1 = x;
                    y1 = y;
                }
                if (safe[x][y] - ff[x][y] < msv)
                {
                    msv = safe[x][y] - ff[x][y];
                    x2 = x;
                    y2 = y;
                }
            }
        }
    if ((x1 >= 1) && (x1 <= N) && (y1 >= 1) && (y1 <= M))
        if (move(x1, y1))
        {
            checkAtt();
            return;
        }
    if ((msv >= 0) || (fDie[getID()] >= 20))
    {
        if (move(x2, y2))
        {
            checkAtt();
            return;
        }
    }
    move(x0, y0);
    if ((a >= 0) && (a <= 3))
        attack(x0 + d[a][0], y0 + d[a][1]);
    while (getPoint())
        increasePoint();
    checkAtt();
    return;
}

void st34::checkAtt()
{
    int x = getX(), y = getY();
    for (int t = 0; t != 4; ++t)
    {
        int x0 = x + d[t][0], y0 = y + d[t][1];
        if ((x0 >= 1) && (x0 <= N) && (y0 >= 1) && (y0 <= M) && (askWhat(x0, y0) != EMPTY))
        {
            attack(x0, y0);
            while (getPoint())
                increasePoint();
        }
    }
}

void st34::revive(int& x, int& y)
{
    ++fDie[getID()];
    x = sx;
    y = sy;
    sx = -1;
    sy = -1;
    for (int i = 1; i <= MAX_PLAYER; ++i)
    {
        fX[i] = 0;
        fY[i] = 0;
    }
}

int st34::eNow(int& dx, int& dy, int& da)
{
    int ta = -1, maxv = -1, sp = getSp();
    dx = -1;
    dy = -1;
    da = -1;
    for (int x = 1; x <= N; ++x)
        for (int y = 1; y <= M; ++y)
            if ((dis(x, y) <= sp) && (askWhat(x, y) == EMPTY))
            {
                int value = evaluate(x, y, ta) - safe[x][y] * (fDie[getID()] / 4 + 1) * getMaxHP() / getHP() + ff[x][y] * 4 / N / M;
                if (value > maxv)
                {
                    maxv = value;
                    dx = x;
                    dy = y;
                    da = ta;
                }
            }
    return maxv;
}

void st34::calcSafe(int i, int j)
{
    int res = 0;
    int res2 = 0;
    bool fb = false;
    for (int x = 1; x <= N; ++x)
        for (int y = 1; y <= M; ++y)
        {
            int d = dis(x, y, i, j);
            if ((isEnemy(askWhat(x, y))))
            {
                int target = askWhat(x, y);
                if (getLevel() >= 4)
                {
                    int cdd = fDie[getID()] - fDie[target] + (fMaxHP[target] / 2 + fSp[target]) / 2;
                    if ((fSp[target] >= d) && (cdd > 0))
                    {
                        if (!fb)
                        {
                            fb = true;
                            res += cdd;
                        }
                        else
                            res += 2 * cdd;
                    }
                }
                else
                {
                    if (d <= 3)
                    {
                        res += 30;
                    }
                    else if (d <= 5)
                    {
                        res += 15;
                    }
                    else if (d <= 7)
                    {
                        res += 5;
                    }
                    else if (d <= 9)
                    {
                        res += 1;
                    }
                }
            }
            if (d <= getSp() / 2)
                res2 += evp(x, y) * 2;
            else if (d <= getSp())
                res2 += evp(x, y);
            else if (d <= getSp() * 3 / 2)
                res2 += evp(x, y) * 2 / 3;
            else if (d <= getSp() * 2)
                res2 += evp(x, y) / 2;
        }
    res = res * getMaxHP() / getHP();
    res2 = res2;
    safe[i][j] = res;
    ff[i][j] = res2;
}

int st34::evaluate(int x, int y, int& da)
{
    int maxv = -1;
    da = -1;
    for (int t = 0; t != 4; ++t)
    {
        int x0 = x + d[t][0], y0 = y + d[t][1];
        if ((x0 >= 1) && (x0 <= N) && (y0 >= 1) && (y0 <= M))
        {
            int value = evp(x0, y0);
            if (value > maxv)
            {
                maxv = value;
                da = t;
            }
        }
    }
    return maxv;
}

int st34::evp(int x, int y)
{
    int value = -1;
    int target = askWhat(x, y);
    if (target == FOOD)
    {
        value += 100 / getLevel() + 120 - getLevel();
        value += 2 * (100 - getLevel()) / (nextLevelExp() - getExp());
        value += 15 * getMaxHP() * getMaxHP() / getHP() / getHP();
        value = value * 6 / getLevel();
    }
    else if (isEnemy(target))
    {
        if ((askHP(target) <= getAtt()) && (fDie[target] >= fDie[getID()]))
            value += (8 * getAtt() + 3000);
        if (askHP(target) <= getAtt())
            value += (4 * getAtt() + 500);
        else if ((askHP(target) <= getAtt() * 2) && (askHP(target) - getAtt() > getLevel() / 2 + 1) && (getHP() > getLevel() * 4 / 3) && (getLevel() >= 6))
            value += (3 * getAtt());
        else if ((askHP(target) > getAtt() * 2) && (askHP(target) < getAtt() * 3) && (getHP() > askHP(target)) && (getHP() > getAtt() * 2) && (getLevel() >= 6))
            value += (2 * getAtt());
        if ((askHP(target) <= getAtt() * 3) && (getLevel() >= 4) && (fDie[getID()] * 2 <= fDie[target]) && (getHP() >= getMaxHP() / 2))
            value += (50 + 6 * (fSp[target] * 2 + fMaxHP[target]));
    }
    int sp = getSp();
    if (sp >= x)
        value -= ((sp + 1 - x) * 3 / 2);
    if (sp >= y)
        value -= ((sp + 1 - y) * 3 / 2);
    if (x + sp > N)
        value -= ((x + sp - N) * 3 / 2);
    if (y + sp > M)
        value -= ((y + sp - M) * 3 / 2);
    return value;
}

void st34::increasePoint()
{
    int lv = getLevel();
    if (lv <= 2)
    {
        if (getHP() < 8)
            increaseHealth();
        else
            increaseSpeed();
    }
    else if (lv <= 4)
    {
        if (getHP() <= lv * 3)
            increaseHealth();
        else if (getSp() < lv * 3)
            increaseSpeed();
        else
            increaseHealth();
    }
    else
    {
        if ((getHP() <= getAtt() * 3) || (getMaxHP() <= lv * 5 / 2) || (fDie[getID()] >= lv / 2))
            increaseHealth();
        else if ((getAtt() <= getMaxHP() / 10) && (getAtt() <= 22))
            increaseStrength();
        else if ((getSp() < (N + M - 2) / 2) && (getSp() <= lv))
            increaseSpeed();
        else increaseHealth();
    }
}

void st34::scan()
{
    for (int i = 1; i <= MAX_PLAYER; ++i)
        found[i] = false;
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j)
        {
            int target = askWhat(i, j);
            if (isEnemy(target))
            {
                found[target] = true;
                int t = askHP(target);
                if (t > fMaxHP[target])
                    fMaxHP[target] = t;
                if ((fX[target] != 0) && (fY[target] != 0))
                {
                    t = dis(fX[target], fY[target], i, j);
                    if (t > fSp[target])
                        fSp[target] = t;
                }
                fX[target] = i;
                fY[target] = j;

            }
        }
    for (int i = 1; i <= MAX_PLAYER; ++i)
        if ((!found[i]) && (i != getID()))
        {
            fX[i] = 0;
            fY[i] = 0;
            ++fDie[i];
        }
}
