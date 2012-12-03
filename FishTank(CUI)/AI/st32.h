#ifndef SUPERY_H2
#define SUPERY_H2
#include "fish.h"


class st32:public fish{

    public:
    void init();

    void play();

    void revive(int&,int&);

};

#endif

//可自行增加所需函数所需函数及变量，但需保证上面每个函数的完整与可调用

//不要使用全局变量！！
