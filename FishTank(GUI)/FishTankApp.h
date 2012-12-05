#ifndef FISHTANKAPP_H
#define FISHTANKAPP_H

#include "utildef.h"
#include "FishInfo.h"
#include "WatcherFrame.h"
#include "SystemThread.h"

class FishTankApp: public wxApp
{
    virtual bool OnInit();
public:
    wxFrame* frame;
    FishInfo* data;
    WatcherFrame* watcher;
    SystemThread* thread;
};

#endif // FISHTANKAPP_H
