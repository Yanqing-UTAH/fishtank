#include "FishTankApp.h"
#include "GUIFrame.h"

DECLARE_APP(FishTankApp)
IMPLEMENT_APP(FishTankApp)

bool FishTankApp::OnInit()
{
    data = new FishInfo();
    frame = new GUIFrame(wxT("FishTank system"), wxDefaultPosition, wxDefaultSize, wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN | wxFULL_REPAINT_ON_RESIZE);
    frame -> Show(true);
    watcher = new WatcherFrame(data, wxT("FishTank Watcher"), wxDefaultPosition, wxDefaultSize, wxMINIMIZE_BOX | wxCAPTION | wxCLIP_CHILDREN);
    watcher -> Show(true);
    thread = new SystemThread();
    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread!"));
    }
    SetTopWindow(frame);
    return true;
}
