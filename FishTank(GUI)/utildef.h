#ifndef UTILDEF_H
#define UTILDEF_H

#include <wx/wxprec.h>
#include <wx/wx.h>

static const int N = 40; //��ͼ�����귶Χ1-N
static const int M = 40; //��ͼ�����귶Χ1-M
static const int MAX_PLAYER = 40; //������������ID��1-MAX_PLAYER
static const int MAX_FOOD = 50; //ˢ��ʳ������
static const int FOOD_ROUND = 5; //ˢ��ʳ��غ���
static const int EMPTY = 0; //����Ϊ��
static const int FOOD = -1; //ʳ��
static const int LEVEL_POINT = 3; //�����ܻ�õĵ���
static const int GAME_ROUND = 500; //��Ϸ�غ���

class fish;
class env;

enum
{
    START_ID = 1000,
    PAUSE_ID,
    RESUME_ID,
    RESET_ID,
    SET_MAP_ID,
    SEND_MSG_ID,
    SEND_STATUS_ID
};

#endif // UTILDEF_H
