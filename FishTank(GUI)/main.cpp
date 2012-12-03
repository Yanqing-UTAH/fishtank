#include <wx/wxprec.h>
#include <wx/wx.h>

class FishTankApp: public wxApp
{
    virtual bool OnInit();
};

class FishTankFrame: public wxFrame
{
public:
    FishTankFrame(const wxString& title,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxDEFAULT_FRAME_STYLE);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
private:
    DECLARE_EVENT_TABLE()
};


DECLARE_APP(FishTankApp)
IMPLEMENT_APP(FishTankApp)

BEGIN_EVENT_TABLE(FishTankFrame, wxFrame)
    EVT_MENU(wxID_EXIT, FishTankFrame::OnQuit)
    EVT_MENU(wxID_ABOUT, FishTankFrame::OnAbout)
END_EVENT_TABLE()

bool FishTankApp::OnInit()
{
    FishTankFrame* frame = new FishTankFrame("FishTank system", wxDefaultPosition, wxGetDisplaySize(), wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN | wxFULL_REPAINT_ON_RESIZE);
    frame -> Show(true);
    SetTopWindow(frame);
    return true;
}

FishTankFrame::FishTankFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    :wxFrame((wxFrame*)NULL, -1, title, pos, size, style)
{
    SetIcon(wxICON(wxWidgets));
    wxMenu* menuFile = new wxMenu;
    menuFile -> Append(wxID_EXIT, _T("E&xit"), _T("Quit"));
    wxMenu* menuHelp = new wxMenu;
    menuHelp -> Append(wxID_ABOUT, _T("&About..."), _T("Shou about dialog"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar -> Append(menuFile, _T("&File"));
    menuBar -> Append(menuHelp, _T("&Help"));
    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("FishTank System Ready");
}

void FishTankFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void FishTankFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("This is the system of FishTank made by Ianking Penn", "About AISystem", wxOK | wxICON_INFORMATION);
}
/*
int main()
{
    env& system = env::GetInstance();
    for (int i = 1; i <= MAX_PLAYER; ++i)
        system.addAI(new TAAI());
    system.play();
}
*/
