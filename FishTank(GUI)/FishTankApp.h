#ifndef FISHTANKAPP_H
#define FISHTANKAPP_H

#include "utildef.h"
#include "FishInfo.h"

class FishTankApp: public wxApp
{
    virtual bool OnInit();
public:
    wxFrame* frame;
    FishInfo* data;
};

#endif // FISHTANKAPP_H
