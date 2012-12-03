#include "env.h"
#include "TAAI.h"
#include "st34.h"
#include "st20.h"
#include "st31.h"
#include "st32.h"
#include "st35.h"
#include "st26.h"
#include "st26(sp).h"
#include "st06.h"

env& fish::host = env::GetInstance();

int main()
{
    env& system = env::GetInstance();
    system.addAI(new st34());
    system.addAI(new st31());
    system.addAI(new st32());
    system.addAI(new st261());
    system.addAI(new st20());
    system.addAI(new st35());
    system.addAI(new st26());
    system.addAI(new st06());
    system.addAI(new st32());
    system.addAI(new st261());
    system.addAI(new st20());
    system.addAI(new st35());
    system.addAI(new st26());
    system.addAI(new st06());
    system.addAI(new st32());
    system.addAI(new st261());
    system.addAI(new st20());
    system.addAI(new st35());
    system.addAI(new st26());
    system.addAI(new st06());
    system.addAI(new st32());
    system.addAI(new st261());
    system.addAI(new st20());
    system.addAI(new st35());
    system.addAI(new st26());
    system.addAI(new st06());
    system.addAI(new st32());
    system.addAI(new st261());
    system.addAI(new st20());
    system.addAI(new st35());
    system.addAI(new st26());
    system.addAI(new st06());
    system.addAI(new st32());
    system.addAI(new st261());
    system.addAI(new st20());
    system.addAI(new st35());
    system.addAI(new st26());
    system.addAI(new st06());
    system.play();
}

