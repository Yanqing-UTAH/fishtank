#include "GUIFrame.h"

BEGIN_EVENT_TABLE(FishTankFrame, wxFrame)
    EVT_MENU(wxID_EXIT, FishTankFrame::OnQuit)
    EVT_MENU(wxID_ABOUT, FishTankFrame::OnAbout)
    EVT_PAINT(FishTankFrame::OnPaint)
    EVT_BUTTON(START_ID, FishTankFrame::OnStart)
    EVT_BUTTON(PAUSE_ID, FishTankFrame::OnPause)
    EVT_BUTTON(RESUME_ID, FishTankFrame::OnResume)
    EVT_BUTTON(RESET_ID, FishTankFrame::OnReset)
    EVT_SET_MAP(SET_MAP_ID, FishTankFrame::OnSetMap)
    EVT_SEND_MSG(SEND_MSG_ID, FishTankFrame::OnSendMsg)
    EVT_SEND_STATUS(SEND_STATUS_ID, FishTankFrame::OnSendStatus)
END_EVENT_TABLE()

FishTankFrame::FishTankFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    :wxFrame((wxFrame*)NULL, -1, title, pos, size, style)
{
    SetIcon(wxICON(wxWidgets));
    wxMenu* menuFile = new wxMenu;
    menuFile -> Append(wxID_EXIT, wxT("E&xit"), wxT("Quit"));
    wxMenu* menuHelp = new wxMenu;
    menuHelp -> Append(wxID_ABOUT, wxT("&About..."), wxT("Shou about dialog"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar -> Append(menuFile, wxT("&File"));
    menuBar -> Append(menuHelp, wxT("&Help"));
    SetMenuBar(menuBar);

    wxBoxSizer* bSizer1 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* bSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Game Control")), wxHORIZONTAL);
    bSizer1->Add(BoardLength + BoardX, BoardLength + BoardY * 3);
    BtnStart = new wxButton(this, START_ID, wxT("&Start"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer->Add(BtnStart, 0, wxALL, 5);
    BtnPause = new wxButton(this, PAUSE_ID, wxT("&Pause"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer->Add(BtnPause, 0, wxALL, 5);
    BtnResume = new wxButton(this, RESUME_ID, wxT("&Resume"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer->Add(BtnResume, 0, wxALL, 5);
    BtnReset = new wxButton(this, RESET_ID, wxT("R&eset"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer->Add(BtnReset, 0, wxALL, 5);

    m_txtctrl = new wxTextCtrl(this, wxID_ANY, "",
                               wxDefaultPosition, wxSize(400, 510),
                               wxTE_MULTILINE | wxTE_READONLY);

    wxFont font(wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_TELETYPE,
                wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    m_txtctrl->SetFont(font);

    m_txtctrl->SetFocus();

    wxBoxSizer* bSizer2 = new wxBoxSizer(wxVERTICAL);

    bSizer2->Add(bSizer, 0, wxALL, 10);
    bSizer2->Add(m_txtctrl, 0, wxALL, 10);

    bSizer1->Add(bSizer2, 0, wxEXPAND, 5);
    SetSizer(bSizer1);
    Layout();
    bSizer1->Fit(this);

    BtnStart->Enable(true);
    BtnPause->Enable(false);
    BtnResume->Enable(false);
    BtnReset->Enable(false);

    CreateStatusBar();
    SetStatusText(wxT("FishTank System Ready"));
}

void FishTankFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void FishTankFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxT("This is the GUI system of FishTank made by Ianking Penn"), wxT("About AISystem"), wxOK | wxICON_INFORMATION);
}

void FishTankFrame::OnPaint(wxPaintEvent& event)
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
    }/*
    if (thread)
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j)
        {
            wxSetMapEvent* event = new wxSetMapEvent(i, j, wxGetApp().data->askWhat(i, j));
            OnSetMap(*event);
        }*/
}

void FishTankFrame::OnStart(wxCommandEvent& WXUNUSED(event))
{
    if (!thread)
        thread = CreateThread();
    BtnStart->Enable(false);
    BtnPause->Enable(true);
    BtnResume->Enable(false);
    BtnReset->Enable(true);
}

void FishTankFrame::OnPause(wxCommandEvent& WXUNUSED(event))
{
    if (thread)
        if (thread->IsRunning())
            thread -> Pause();
    BtnPause->Enable(false);
    BtnResume->Enable(true);
}

void FishTankFrame::OnResume(wxCommandEvent& WXUNUSED(event))
{
    if (thread)
        if (thread->IsPaused())
            thread -> Resume();
    BtnPause->Enable(true);
    BtnResume->Enable(false);
}

void FishTankFrame::OnReset(wxCommandEvent& WXUNUSED(event))
{
    thread->Kill();
    thread = 0;
    BtnStart->Enable(true);
    BtnPause->Enable(false);
    BtnResume->Enable(false);
    BtnReset->Enable(false);
    m_txtctrl->Clear();
    Refresh();
}

SystemThread *FishTankFrame::CreateThread()
{
    SystemThread *thread = new SystemThread();
    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread!"));
    }
    thread->Run();
    return thread;
}

void FishTankFrame::OnSendMsg(wxSendMsgEvent& event)
{
    m_txtctrl->AppendText(event.GetMsg() + '\n');
}

void FishTankFrame::OnSendStatus(wxSendMsgEvent& event)
{
    SetStatusText(event.GetMsg() + '\n');
}

void FishTankFrame::OnSetMap(wxSetMapEvent& event)
{
    wxClientDC dc(this);
    const int W = BoardLength / (wxMax(N, M) + 2);
    if (event.GetTarget() == EMPTY)
    {
        dc.SetPen(*wxGREY_PEN);
        dc.SetBrush(*wxGREY_BRUSH);
        dc.DrawRectangle(BoardX + W * event.GetX() + 1, BoardY + W * event.GetY() + 1, W - 1, W - 1);
    }
    else if (event.GetTarget() == FOOD)
    {
        dc.SetPen(*wxGREEN_PEN);
        dc.SetBrush(*wxGREEN_BRUSH);
        dc.DrawCircle(BoardX + W * event.GetX() + W / 2, BoardY + W * event.GetY() + W / 2, W * 5 / 12);
    }
    else
    {
        dc.SetPen(*wxBLUE_PEN);
        dc.SetBrush(*wxBLUE_BRUSH);
        dc.DrawCircle(BoardX + W * event.GetX() + W / 2, BoardY + W * event.GetY() + W / 2, W * 5 / 12);
        dc.DrawText(wxString::Format("%d", event.GetTarget()), wxPoint(BoardX + W * event.GetX() + W / 8, BoardY + W * event.GetY() + W / 8));
    }
}
