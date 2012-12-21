#ifndef UTILDEF_H
#define UTILDEF_H

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <cstdlib>

static const int N = 40; //地图横坐标范围1-N
static const int M = 40; //地图纵坐标范围1-M
static const int MAX_PLAYER = 40; //最多玩家数量，ID从1-MAX_PLAYER
static const int MAX_FOOD = 50; //刷新食物数量
static const int FOOD_ROUND = 5; //刷新食物回合数
static const int EMPTY = 0; //格子为空
static const int FOOD = -1; //食物
static const int LEVEL_POINT = 3; //升级能获得的点数
static const int GAME_ROUND = 500; //游戏回合数

class fish;
class SystemThread;

enum
{
    START_ID = 1000,
    PAUSE_ID,
    RESUME_ID,
    SET_MAP_ID,
    SEND_MSG_ID,
    SEND_SB_ID,
    LIST_ID,
    CHANGE_DATA_ID,
    GAME_OVER_ID
};

inline int larger(int a, int b)
{
    return a > b ? a : b;
}

inline int dis(int x1, int y1, int x2, int y2)
{
    return (std::abs(x1 - x2) + std::abs(y1 - y2));
}

inline bool validCor(int x, int y)
{
    return ((x >= 1) && (x <= N) && (y >= 1) && (y <= M));
}

#endif // UTILDEF_H
