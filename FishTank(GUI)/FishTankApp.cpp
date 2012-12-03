#include "FishTankApp.h"
#include "GUIFrame.h"

DECLARE_APP(FishTankApp)
IMPLEMENT_APP(FishTankApp)

bool FishTankApp::OnInit()
{
    frame = new FishTankFrame(wxT("FishTank system"), wxDefaultPosition, wxGetDisplaySize(), wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN | wxFULL_REPAINT_ON_RESIZE);
    frame -> Show(true);
    data = new FishInfo();
    SetTopWindow(frame);
    return true;
}
