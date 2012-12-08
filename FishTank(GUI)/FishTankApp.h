#ifndef FISHTANKAPP_H
#define FISHTANKAPP_H

#include "utildef.h"
#include "FishInfo.h"
#include "SystemThread.h"
#include "GUIFrame.h"

class FishTankApp: public wxApp
{
    virtual bool OnInit();
public:
    GUIFrame* frame;
    FishInfo* data;
    SystemThread* thread;
};

#endif // FISHTANKAPP_H
