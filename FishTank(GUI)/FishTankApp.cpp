#include "FishTankApp.h"

DECLARE_APP(FishTankApp)
IMPLEMENT_APP(FishTankApp)

bool FishTankApp::OnInit()
{
    data = new FishInfo();
    thread = new SystemThread(data);
    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread!"));
    }
    frame = new GUIFrame(data, thread, wxT("FishTank system"), wxDefaultPosition, wxDefaultSize, wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN | wxFULL_REPAINT_ON_RESIZE);
    thread -> SetHandler(frame->GetEventHandler());
    data -> SetHandler(frame->GetEventHandler());
    frame -> Show(true);
    SetTopWindow(frame);
    return true;
}
