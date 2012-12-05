#ifndef GUIFRAME_H
#define GUIFRAME_H

#include "utildef.h"
#include "Events.h"
#include "FishTankApp.h"

class GUIFrame: public wxFrame
{
public:
    GUIFrame(const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_FRAME_STYLE);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnStart(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnResume(wxCommandEvent& event);
    void OnReset(wxCommandEvent& event);
    void OnSetMap(wxSetMapEvent& event);
    void OnSendMsg(wxSendMsgEvent& event);
    void OnSendStatus(wxSendMsgEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnClose(wxCloseEvent& event);
private:
    static const int BoardLength = 600;
    static const int BoardX = 15;
    static const int BoardY = 15;
protected:
    DECLARE_EVENT_TABLE()
};

#endif // GUIFRAME_H
