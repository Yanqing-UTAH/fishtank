#include "GUIFrame.h"

BEGIN_EVENT_TABLE(GUIFrame, wxFrame)
    EVT_MENU(wxID_EXIT, GUIFrame::OnQuit)
    EVT_MENU(wxID_ABOUT, GUIFrame::OnAbout)
    EVT_PAINT(GUIFrame::OnPaint)
    EVT_MENU(START_ID, GUIFrame::OnStart)
    EVT_MENU(PAUSE_ID, GUIFrame::OnPause)
    EVT_MENU(RESUME_ID, GUIFrame::OnResume)
    EVT_MENU(RESET_ID, GUIFrame::OnReset)
    EVT_SET_MAP(SET_MAP_ID, GUIFrame::OnSetMap)
    EVT_COMMAND(SEND_SB_ID, wxEVT_SEND_SB, GUIFrame::OnSendStatusBar)
    EVT_CLOSE(GUIFrame::OnClose)
END_EVENT_TABLE()

GUIFrame::GUIFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    :wxFrame((wxFrame*)NULL, -1, title, pos, size, style)
{
    CreateStatusBar();
    SetStatusText(wxT("FishTank System Ready"));

    wxMenu* menuFile = new wxMenu;
    menuFile -> Append(START_ID, wxT("&Start\tCtrl-S"), wxT("Start a new game"));
    menuFile -> Append(PAUSE_ID, wxT("&Pause\tCtrl-P"), wxT("Pause current game"));
    menuFile -> Append(RESUME_ID, wxT("&Resume\tCtrl-R"), wxT("Resume current game"));
    menuFile -> Append(RESET_ID, wxT("R&eset\tCtrl-E"), wxT("Reset the game"));
    menuFile -> AppendSeparator();
    menuFile -> Append(wxID_EXIT, wxT("E&xit\tAlt-X"), wxT("Quit"));
    wxMenu* menuHelp = new wxMenu;
    menuHelp -> Append(wxID_ABOUT, wxT("&About...\tCtrl-A"), wxT("Shou about dialog"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar -> Append(menuFile, wxT("&File"));
    menuBar -> Append(menuHelp, wxT("&Help"));
    SetMenuBar(menuBar);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(BoardLength + (BoardX << 1), BoardLength + (BoardY << 1));
    SetSizer(sizer);
    Layout();
    sizer->Fit(this);

}

void GUIFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void GUIFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxT("Big Homework of 2012 ACM Class\nThis is the GUI system of FishTank made by Ianking Penn\nThanks to wxWidgets"), wxT("About AISystem"), wxOK | wxICON_INFORMATION);
}

void GUIFrame::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxGREY_BRUSH);

    const int W = BoardLength / (wxMax(N, M) + 2);

    dc.DrawRectangle(BoardX, BoardY, W * (N + 2), W * (M + 2));

    for (int i = 1; i <= N + 1; ++i)
    {
        if (i % 5 == 0)
            dc.DrawText(wxString::Format("%d", i), wxPoint(BoardX + i * W + (i < 10 ? W / 4 : W / 8), BoardY));
        dc.DrawLine(BoardX + i * W, BoardY + W, BoardX + i * W, BoardY + (M + 1) * W + 1);
    }

    for (int i = 1; i <= M + 1; ++i)
    {
        if (i % 5 == 0)
            dc.DrawText(wxString::Format("%d", i), wxPoint(BoardX + (i < 10 ? W * 3 / 8 : 0), BoardY + i * W));
        dc.DrawLine(BoardX + W, BoardY + i * W, BoardX + (N + 1) * W + 1, BoardY + i * W);
    }
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j)
            PaintGrid(i, j);
}

void GUIFrame::OnStart(wxCommandEvent& WXUNUSED(event))
{
    ((FishTankApp*)wxTheApp)->thread->Run();
}

void GUIFrame::OnPause(wxCommandEvent& WXUNUSED(event))
{
    if (((FishTankApp*)wxTheApp)->thread->IsRunning())
        ((FishTankApp*)wxTheApp)->thread->Pause();
}

void GUIFrame::OnResume(wxCommandEvent& WXUNUSED(event))
{
    if (((FishTankApp*)wxTheApp)->thread->IsPaused())
        ((FishTankApp*)wxTheApp)->thread->Resume();
}

void GUIFrame::OnReset(wxCommandEvent& WXUNUSED(event))
{
    //thread->Kill();
    //thread = 0;
    //myWatcher->m_txtctrl->Clear();
    Refresh();
}

void GUIFrame::OnSendStatusBar(wxCommandEvent& event)
{
    SetStatusText(event.GetString() + '\n');
}

void GUIFrame::OnSetMap(wxSetMapEvent& event)
{
    PaintGrid(event.GetX(), event.GetY());
}

void GUIFrame::PaintGrid(int x, int y)
{
    wxClientDC dc(this);
    const int W = BoardLength / (wxMax(N, M) + 2);
    int target = (((FishTankApp*)wxTheApp)->data->askWhat(x, y));
    if (target == EMPTY)
    {
        dc.SetPen(*wxGREY_PEN);
        dc.SetBrush(*wxGREY_BRUSH);
        dc.DrawRectangle(BoardX + W * x + 1, BoardY + W * y + 1, W - 1, W - 1);
    }
    else if (target == FOOD)
    {
        dc.SetPen(*wxGREEN_PEN);
        dc.SetBrush(*wxGREEN_BRUSH);
        dc.DrawCircle(BoardX + W * x + W / 2, BoardY + W * y + W / 2, W * 5 / 12);
    }
    else
    {
        dc.SetPen(*wxBLUE_PEN);
        dc.SetBrush(*wxBLUE_BRUSH);
        dc.DrawCircle(BoardX + W * x + W / 2, BoardY + W * y + W / 2, W * 5 / 12);
        dc.DrawText(wxString::Format("%d", target), wxPoint(BoardX + W * x + W / 8, BoardY + W * y + W / 8));
    }
}

void GUIFrame::OnClose(wxCloseEvent& event)
{
    ((FishTankApp*)wxTheApp) -> watcher -> Destroy();
    Destroy();
}
