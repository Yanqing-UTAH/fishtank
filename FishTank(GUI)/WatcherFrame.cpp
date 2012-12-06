#include "WatcherFrame.h"

BEGIN_EVENT_TABLE(WatcherFrame, wxFrame)
    EVT_COMMAND(SEND_MSG_ID, wxEVT_SEND_MSG, WatcherFrame::OnSendMsg)
    EVT_CLOSE(WatcherFrame::OnClose)
END_EVENT_TABLE()


int wxCALLBACK SortCmp(wxIntPtr item1, wxIntPtr item2, wxIntPtr WXUNUSED(sortData))
{
    // inverse the order
    if (item1 < item2)
        return 1;
    if (item1 > item2)
        return -1;

    return 0;
}


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

    list = new wxListCtrl(notebook, wxID_ANY, wxDefaultPosition, wxSize(300, 350), wxLC_REPORT | wxLC_SINGLE_SEL);

    list->AppendColumn("ID", wxLIST_FORMAT_CENTRE, 30);
    list->AppendColumn("Lv", wxLIST_FORMAT_CENTRE, 30);
    list->AppendColumn("HP", wxLIST_FORMAT_CENTRE, 30);
    list->AppendColumn("MaxHP", wxLIST_FORMAT_CENTRE, 60);
    list->AppendColumn("Att", wxLIST_FORMAT_CENTRE, 40);
    list->AppendColumn("Sp", wxLIST_FORMAT_CENTRE, 30);
    list->AppendColumn("Exp", wxLIST_FORMAT_CENTRE, 40);
    list->AppendColumn("Point", wxLIST_FORMAT_CENTRE, 50);
    list->AppendColumn("Score", wxLIST_FORMAT_CENTRE, 50);
    //for (int i = 1; i <= data->getFishCount(); ++i)
    for (int i = 1; i <= MAX_PLAYER; ++i)
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
        list->SetItemData(item, data->getScore(i));
    }
    list->SortItems(SortCmp, 0);
}
