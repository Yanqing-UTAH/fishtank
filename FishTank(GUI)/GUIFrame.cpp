#include "GUIFrame.h"

BEGIN_EVENT_TABLE(GUIFrame, wxFrame)
    EVT_MENU(wxID_EXIT, GUIFrame::OnQuit)
    EVT_MENU(wxID_ABOUT, GUIFrame::OnAbout)
    EVT_PAINT(GUIFrame::OnPaint)
    EVT_MENU(START_ID, GUIFrame::OnStart)
    EVT_MENU(PAUSE_ID, GUIFrame::OnPause)
    EVT_MENU(RESUME_ID, GUIFrame::OnResume)
    EVT_COMMAND(START_ID, wxEVT_COMMAND_BUTTON_CLICKED, GUIFrame::OnStart)
    EVT_COMMAND(PAUSE_ID, wxEVT_COMMAND_BUTTON_CLICKED, GUIFrame::OnPause)
    EVT_COMMAND(RESUME_ID, wxEVT_COMMAND_BUTTON_CLICKED, GUIFrame::OnResume)
    EVT_SET_MAP(SET_MAP_ID, GUIFrame::OnSetMap)
    EVT_COMMAND(SEND_SB_ID, wxEVT_SEND_SB, GUIFrame::OnSendStatusBar)
    EVT_COMMAND(SEND_MSG_ID, wxEVT_SEND_MSG, GUIFrame::OnSendMsg)
    EVT_COMMAND(CHANGE_DATA_ID, wxEVT_CHANGE_DATA, GUIFrame::OnRefreshList)
    EVT_COMMAND(GAME_OVER_ID, wxEVT_GAME_OVER, GUIFrame::OnGameOver)
    EVT_LIST_COL_CLICK(LIST_ID, GUIFrame::OnClickCol)
END_EVENT_TABLE()

GUIFrame::GUIFrame(FishInfo* info, SystemThread* env, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    :wxFrame((wxFrame*)NULL, -1, title, pos, size, style), data(info), thread(env)
{
    CreateStatusBar();
    SetStatusText(wxT("FishTank System Ready"));

    wxMenu* menuFile = new wxMenu;
    menuFile -> Append(START_ID, wxT("&Start\tCtrl-S"), wxT("Start a new game"));
    menuFile -> Append(PAUSE_ID, wxT("&Pause\tCtrl-P"), wxT("Pause current game"));
    menuFile -> Append(RESUME_ID, wxT("&Resume\tCtrl-R"), wxT("Resume current game"));
    menuFile -> AppendSeparator();
    menuFile -> Append(wxID_EXIT, wxT("E&xit\tAlt-X"), wxT("Quit"));
    wxMenu* menuHelp = new wxMenu;
    menuHelp -> Append(wxID_ABOUT, wxT("&About...\tCtrl-A"), wxT("Shou about dialog"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar -> Append(menuFile, wxT("&File"));
    menuBar -> Append(menuHelp, wxT("&Help"));
    SetMenuBar(menuBar);

    txtctrl = new wxTextCtrl(this, wxID_ANY, "",
                             wxDefaultPosition, wxSize(430, 150),
                             wxTE_MULTILINE | wxTE_READONLY);

    wxFont font(wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_TELETYPE,
                wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    txtctrl->SetFont(font);

    txtctrl->SetFocus();

    list = new wxListCtrl(this, LIST_ID, wxDefaultPosition, wxSize(430, 390), wxLC_REPORT | wxLC_SINGLE_SEL);

    colItems[0].SetText("  ID");
    colItems_s[0][0].SetText("¨‹ID");
    colItems_s[0][1].SetText("¡øID");

    colItems[1].SetText("Lv");
    colItems_s[1][0].SetText("¨‹Lv");
    colItems_s[1][1].SetText("¡øLv");

    colItems[2].SetText("HP");
    colItems_s[2][0].SetText("¨‹HP");
    colItems_s[2][1].SetText("¡øHP");

    colItems[3].SetText("MaxHP");
    colItems_s[3][0].SetText("¨‹MaxHP");
    colItems_s[3][1].SetText("¡øMaxHP");

    colItems[4].SetText("Att");
    colItems_s[4][0].SetText("¨‹Att");
    colItems_s[4][1].SetText("¡øAtt");

    colItems[5].SetText("Sp");
    colItems_s[5][0].SetText("¨‹Sp");
    colItems_s[5][1].SetText("¡øSp");

    colItems[6].SetText("Exp");
    colItems_s[6][0].SetText("¨‹Exp");
    colItems_s[6][1].SetText("¡øExp");

    colItems[7].SetText("Pt");
    colItems_s[7][0].SetText("¨‹Pt");
    colItems_s[7][1].SetText("¡øPt");

    colItems[8].SetText("Score");
    colItems_s[8][0].SetText("¨‹Score");
    colItems_s[8][1].SetText("¡øScore");

    for (int i = 0; i <= 8; ++i)
    {
        colItems[i].SetColumn(i);
        colItems[i].SetAlign(wxLIST_FORMAT_CENTRE);
        colItems_s[i][0].SetColumn(i);
        colItems_s[i][0].SetAlign(wxLIST_FORMAT_CENTRE);
        colItems_s[i][1].SetColumn(i);
        colItems_s[i][1].SetAlign(wxLIST_FORMAT_CENTRE);
        list->InsertColumn(i, colItems[i]);
    }
    list->SetColumnWidth(0, 37);
    list->SetColumnWidth(1, 37);
    list->SetColumnWidth(2, 40);
    list->SetColumnWidth(3, 65);
    list->SetColumnWidth(4, 42);
    list->SetColumnWidth(5, 40);
    list->SetColumnWidth(6, 45);
    list->SetColumnWidth(7, 37);
    list->SetColumnWidth(8, 65);

    sortCol = 0;
    sortOrder = 1;

    for (int i = 1; i <= data->getFishCount(); ++i)
    {
        wxListItem item;
        item.SetId(i);
        item.SetText(wxString::Format("%d", i));
        item.SetData(0L);
        list->InsertItem(item);
        for (int j = 1; j <= 8; ++j)
            list->SetItem(i - 1, j, "0");
    }


    btnStart = new wxButton(this, START_ID, wxT("Start"));
    btnPause = new wxButton(this, PAUSE_ID, wxT("Pause"));
    btnResume = new wxButton(this, RESUME_ID, wxT("Resume"));
    btnPause->Enable(false);
    btnResume->Enable(false);

    wxStaticBoxSizer* btnSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Game Control")), wxHORIZONTAL);
    btnSizer->Add(btnStart, wxSizerFlags().Border(wxALL, 10));
    btnSizer->Add(btnPause, wxSizerFlags(0).Border(wxALL, 10));
    btnSizer->Add(btnResume, wxSizerFlags(0).Border(wxALL, 10));

    wxBoxSizer* rSizer = new wxBoxSizer(wxVERTICAL);
    rSizer->Add(430, BoardY);
    rSizer->Add(btnSizer, wxSizerFlags(1).Expand());
    rSizer->Add(list);
    rSizer->Add(txtctrl);
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(BoardLength + BoardX, BoardLength + (BoardY << 1));
    sizer->Add(rSizer);

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
    if ((thread) && (!thread->IsRunning()))
    {
        thread->Run();
        btnStart->Enable(false);
        btnPause->Enable(true);
        btnResume->Enable(false);
        SortOnCol(8);
    }
}

void GUIFrame::OnPause(wxCommandEvent& WXUNUSED(event))
{
    if (thread->IsRunning())
    {
        thread->Pause();
        btnPause->Enable(false);
        btnResume->Enable(true);
    }

}

void GUIFrame::OnResume(wxCommandEvent& WXUNUSED(event))
{
    if (thread->IsPaused())
    {
        thread->Resume();
        btnPause->Enable(true);
        btnResume->Enable(false);
    }
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
    int target = data->askWhat(x, y);
    if (target == EMPTY)
    {
        dc.SetPen(*wxGREY_PEN);
        dc.SetBrush(*wxGREY_BRUSH);
        dc.DrawRectangle(BoardY + W * y + 1, BoardX + W * x + 1, W - 1, W - 1);
    }
    else if (target == FOOD)
    {
        dc.SetPen(*wxGREEN_PEN);
        dc.SetBrush(*wxGREEN_BRUSH);
        dc.DrawCircle(BoardY + W * y + W / 2, BoardX + W * x + W / 2, W * 5 / 12);
    }
    else
    {
        dc.SetPen(*wxBLUE_PEN);
        dc.SetBrush(*wxBLUE_BRUSH);
        dc.DrawCircle(BoardY + W * y + W / 2, BoardX + W * x + W / 2, W * 5 / 12);
        dc.DrawText(wxString::Format("%d", target), wxPoint(BoardY + W * y + W / 8, BoardX + W * x + W / 8));
    }
}

void GUIFrame::OnSendMsg(wxCommandEvent& event)
{
    txtctrl->AppendText(event.GetString() + '\n');
}

void GUIFrame::OnGameOver(wxCommandEvent& WXUNUSED(event))
{
    btnPause->Enable(false);
    btnResume->Enable(false);
}

void GUIFrame::OnRefreshList(wxCommandEvent& WXUNUSED(event))
{
    for (int i = 1; i <= data->getFishCount(); ++i)
    {
        long item = list->FindItem(-1, wxString::Format("%d", i));
        list->SetItem(item, 1, wxString::Format("%d", data->getLevel(i)));
        list->SetItem(item, 2, wxString::Format("%d", data->getHP(i)));
        list->SetItem(item, 3, wxString::Format("%d", data->getMaxHP(i)));
        list->SetItem(item, 4, wxString::Format("%d", data->getAtt(i)));
        list->SetItem(item, 5, wxString::Format("%d", data->getSp(i)));
        list->SetItem(item, 6, wxString::Format("%d", data->getExp(i)));
        list->SetItem(item, 7, wxString::Format("%d", data->getPoint(i)));
        list->SetItem(item, 8, wxString::Format("%d", data->getScore(i)));
    }
    sortOrder = 1 - sortOrder;
    SortOnCol(sortCol);
}

void GUIFrame::OnClickCol(wxListEvent& event)
{
    SortOnCol(event.GetColumn());
}

int wxCALLBACK SortCmp(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
    if (sortData)
    {
        if (item1 < item2)
            return -1;
        if (item1 > item2)
            return 1;
    }
    else
    {
        if (item1 < item2)
            return 1;
        if (item1 > item2)
            return -1;
    }
    return 0;
}

void GUIFrame::SortOnCol(int col)
{
    for (int i = 1; i <= data->getFishCount(); ++i)
    {
        long item = list->FindItem(-1, wxString::Format("%d", i));
        if (col == 0)
            list->SetItemData(item, i);
        else if (col == 1)
            list->SetItemData(item, data->getLevel(i));
        else if (col == 2)
            list->SetItemData(item, data->getHP(i));
        else if (col == 3)
            list->SetItemData(item, data->getMaxHP(i));
        else if (col == 4)
            list->SetItemData(item, data->getAtt(i));
        else if (col == 5)
            list->SetItemData(item, data->getSp(i));
        else if (col == 6)
            list->SetItemData(item, data->getExp(i));
        else if (col == 7)
            list->SetItemData(item, data->getPoint(i));
        else if (col == 8)
            list->SetItemData(item, data->getScore(i));
    }
    sortOrder = (col != sortCol) ? 0 : 1 - sortOrder;
    sortCol = col;
    for (int i = 0; i <= 8; ++i)
        if (i != col)
            list->SetColumn(i, colItems[i]);
        else
            list->SetColumn(i, colItems_s[i][sortOrder]);
    list->SortItems(SortCmp, sortOrder);
}
