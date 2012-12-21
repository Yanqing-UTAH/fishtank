#include "env.h"
#include "st01.h"
#include "st02.h"
#include "st03.h"

env& fish::host = env::GetInstance();

int main()
{
    env& system = env::GetInstance();
    system.addAI(new st01());
    system.addAI(new st02());
    system.addAI(new st03());
    system.play();
}

