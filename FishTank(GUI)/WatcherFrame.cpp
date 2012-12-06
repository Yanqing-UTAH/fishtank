#include "WatcherFrame.h"

BEGIN_EVENT_TABLE(WatcherFrame, wxFrame)
    EVT_COMMAND(SEND_MSG_ID, wxEVT_SEND_MSG, WatcherFrame::OnSendMsg)
    EVT_CLOSE(WatcherFrame::OnClose)
    EVT_LIST_COL_CLICK(LIST_ID, WatcherFrame::OnClickCol)
END_EVENT_TABLE()

WatcherFrame::WatcherFrame(FishInfo* info, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    :wxFrame((wxFrame*)NULL, -1, title, pos, size, style), data(info)
{
    wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    txtctrl = new wxTextCtrl(notebook, wxID_ANY, "",
                             wxDefaultPosition, wxSize(300, 350),
                             wxTE_MULTILINE | wxTE_READONLY);

    wxFont font(wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_TELETYPE,
                wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    txtctrl->SetFont(font);

    txtctrl->SetFocus();

    list = new wxListCtrl(notebook, LIST_ID, wxDefaultPosition, wxSize(300, 350), wxLC_REPORT | wxLC_SINGLE_SEL);

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
    }
    Refresh();
    notebook->AddPage(txtctrl, wxT("Log"), true);
    notebook->AddPage(list, wxT("Status"), false);
}

void WatcherFrame::OnSendMsg(wxCommandEvent& event)
{
    txtctrl->AppendText(event.GetString() + '\n');
}

void WatcherFrame::OnClose(wxCloseEvent& event)
{
    event.Veto();
    return;
}

void WatcherFrame::Refresh()
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

void WatcherFrame::OnClickCol(wxListEvent& event)
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

void WatcherFrame::SortOnCol(int col)
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
