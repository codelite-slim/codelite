#include "GTKNotebook.hpp"

#if CL_USE_NATIVEBOOK

#include "clTabRenderer.h"
#include <gtk/gtk.h>
#include <iostream>
#include <unordered_map>
#include <wx/dataobj.h>
#include <wx/defs.h>
#include <wx/dnd.h>

wxDEFINE_EVENT(wxEVT_BOOK_PAGE_CHANGING, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_BOOK_PAGE_CHANGED, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_BOOK_PAGE_CLOSING, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_BOOK_PAGE_CLOSED, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_BOOK_PAGE_CLOSE_BUTTON, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_BOOK_TAB_DCLICKED, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_BOOK_TABAREA_DCLICKED, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_BOOK_TAB_CONTEXT_MENU, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_BOOK_FILELIST_BUTTON_CLICKED, clContextMenuEvent);

//===------------------
// GTK specifics
//===------------------
class wxGtkNotebookPage : public wxObject
{
public:
    GtkWidget* m_box;
    GtkWidget* m_label;
    GtkWidget* m_image;
    int m_imageIndex;
};

static void on_button_clicked(GtkWidget* widget, gpointer* data)
{
    wxUnusedVar(widget);
    wxWindow* page = reinterpret_cast<wxWindow*>(data);
    Notebook* book = dynamic_cast<Notebook*>(page->GetParent());
    std::cout << "button clicked" << std::endl;
    book->TabButtonClicked(page);
}

static void on_page_reordered(Notebook* notebook, GtkWidget* child, guint page_num, gpointer user_data)
{
    wxUnusedVar(notebook);
    wxUnusedVar(child);
    wxUnusedVar(page_num);
    Notebook* wxbook = reinterpret_cast<Notebook*>(user_data);
    wxbook->TabReordered();
}

static gboolean button_press_event(GtkWidget* WXUNUSED_IN_GTK3(widget), GdkEventButton* gdk_event, Notebook* win)
{
    // check the mouse button clicked
    if(gdk_event->button == 1 && gdk_event->type == GDK_2BUTTON_PRESS) {
        // wxEVT_LEFT_DCLICK
        win->GTKLeftDClick();
        return true;
    } else if(gdk_event->button == 2 && gdk_event->type == GDK_BUTTON_PRESS) {
        // wxEVT_MIDDLE_DOWN
        win->GTKMiddleDown();
        return true;
    } else if(gdk_event->button == 3 && gdk_event->type == GDK_BUTTON_PRESS) {
        // wxEVT_RIGHT_DOWN
        win->GTKRightDown();
        return true;
    } else {
        return false;
    }
}

//===------------------
//===------------------

Notebook::Notebook(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style,
                   const wxString& name)
    : wxNotebook(parent, id, pos, size, wxBK_DEFAULT, name)
{
    Initialise(style);
}

Notebook::~Notebook() { wxDELETE(m_tabContextMenu); }

bool Notebook::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style,
                      const wxString& name)
{
    Initialise(style);
    return wxNotebook::Create(parent, id, pos, size, wxBK_DEFAULT);
}

void Notebook::AddPage(wxWindow* page, const wxString& label, bool selected, const wxBitmap& bmp,
                       const wxString& shortLabel)
{
    wxUnusedVar(bmp);
    if(!wxNotebook::InsertPage(GetPageCount(), page, label, selected, wxNOT_FOUND)) {
        return;
    }
    DoFinaliseAddPage(page, shortLabel, bmp);
}

bool Notebook::InsertPage(size_t index, wxWindow* page, const wxString& label, bool selected, const wxBitmap& bmp,
                          const wxString& shortLabel)
{
    wxUnusedVar(bmp);
    if(!wxNotebook::InsertPage(index, page, label, selected, wxNOT_FOUND)) {
        return false;
    }

    DoFinaliseAddPage(page, shortLabel, bmp);
    return true;
}

int Notebook::GetPageIndex(wxWindow* page) const
{
    for(size_t i = 0; i < GetPageCount(); ++i) {
        if(page == GetPage(i)) {
            return static_cast<int>(i);
        }
    }
    return wxNOT_FOUND;
}

void Notebook::BindEvents()
{
    Bind(wxEVT_NOTEBOOK_PAGE_CHANGING, &Notebook::OnPageChanging, this);
    Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &Notebook::OnPageChanged, this);

    g_signal_connect(GTK_WIDGET(GetHandle()), "button_press_event", G_CALLBACK(button_press_event), this);
    g_signal_connect(GTK_NOTEBOOK(GetHandle()), "page-reordered", G_CALLBACK(on_page_reordered), this);
}

void Notebook::GTKLeftDClick()
{
    long flags = 0;
    wxPoint pt = ScreenToClient(::wxGetMousePosition());
    int pos = HitTest(pt, &flags);
    if(pos != wxNOT_FOUND && (flags & (wxBK_HITTEST_ONICON | wxBK_HITTEST_ONLABEL))) {
        // Fire event
        wxBookCtrlEvent event(wxEVT_BOOK_TAB_DCLICKED);
        event.SetEventObject(this);
        event.SetSelection(pos);
        GetEventHandler()->AddPendingEvent(event);
    } else if(flags & wxBK_HITTEST_NOWHERE) {
        // No where
        wxBookCtrlEvent event(wxEVT_BOOK_TABAREA_DCLICKED);
        event.SetEventObject(this);
        GetEventHandler()->AddPendingEvent(event);
    }
}

void Notebook::GTKMiddleDown()
{
    long flags = 0;
    wxPoint pt = ScreenToClient(::wxGetMousePosition());

    int pos = HitTest(pt, &flags);
    if(pos == wxNOT_FOUND) {
        return;
    }
    if(m_bookStyle & kNotebook_MouseMiddleClickClosesTab) {
        // Close the tab
        DeletePage(pos);
    } else if(m_bookStyle & kNotebook_CloseButtonOnActiveTabFireEvent) {
        // Fire event
        wxBookCtrlEvent event(wxEVT_BOOK_PAGE_CLOSE_BUTTON);
        event.SetEventObject(this);
        event.SetSelection(pos);
        GetEventHandler()->ProcessEvent(event);
    }
}

void Notebook::GTKRightDown()
{
    long flags = 0;
    wxPoint pt = ScreenToClient(::wxGetMousePosition());
    int where = HitTest(pt, &flags);
    if(where == wxNOT_FOUND) {
        return;
    }

    if(m_tabContextMenu) {
        PopupMenu(m_tabContextMenu);
    } else {
        // fire an event
        wxBookCtrlEvent menuEvent(wxEVT_BOOK_TAB_CONTEXT_MENU);
        menuEvent.SetEventObject(this);
        menuEvent.SetSelection(where);
        GetEventHandler()->ProcessEvent(menuEvent);
    }
}

bool Notebook::GetPageDetails(wxWindow* page, int& curindex, wxString& label, int& imageId) const
{
    for(size_t i = 0; i < GetPageCount(); ++i) {
        if(page == GetPage(i)) {
            curindex = i;
            label = GetPageText(i);
            imageId = GetPageImage(i);
            return true;
        }
    }
    return false;
}

int Notebook::FindPageByGTKHandle(WXWidget page) const
{
    for(size_t i = 0; i < GetPageCount(); ++i) {
        if(page == GetPage(i)->GetHandle()) {
            return i;
        }
    }
    return wxNOT_FOUND;
}

void Notebook::OnPageChanged(wxBookCtrlEvent& e)
{
    wxBookCtrlEvent event(wxEVT_BOOK_PAGE_CHANGED);
    event.SetEventObject(this);
    event.SetSelection(GetSelection());
    event.SetOldSelection(e.GetOldSelection());
    GetEventHandler()->ProcessEvent(event);
}

void Notebook::OnPageChanging(wxBookCtrlEvent& e)
{
    // wrap this event with our own event
    wxBookCtrlEvent event(wxEVT_BOOK_PAGE_CHANGING);
    event.SetEventObject(this);
    event.SetSelection(e.GetSelection());
    event.SetOldSelection(GetSelection());
    GetEventHandler()->ProcessEvent(event);
    if(!event.IsAllowed()) {
        // Vetoed by the user, veto the original event and return
        e.Veto();
        return;
    }
    // Allow changing
    e.Skip();
    if(GetCurrentPage()) {
        m_history->Push(GetCurrentPage());
    }
}

wxWindow* Notebook::DoUpdateHistoryPreRemove(wxWindow* page)
{
    bool deletingSelection = (page == GetCurrentPage());
    wxWindow* nextSelection = nullptr;
    if(deletingSelection) {
        while(!m_history->GetHistory().empty() && !nextSelection) {
            nextSelection = m_history->PrevPage();
            if(GetPageIndex(nextSelection) == wxNOT_FOUND) {
                // The history contains a tab that no longer exists
                m_history->Pop(nextSelection);
                nextSelection = NULL;
            }
        }
        // It is OK to end up with a null next selection, we will handle it later
    }
    return nextSelection;
}

void Notebook::DoUpdateHistoryPostRemove(wxWindow* page, bool deletedSelection)
{
    // Choose a new selection
    if(deletedSelection) {
        // Always make sure we have something to select...
        if(!page && GetPageCount()) {
            page = GetPage(0);
        }

        int nextSel = FindPage(page);
        if(nextSel != wxNOT_FOUND) {
            SetSelection(nextSel);
        }
    }
}

bool Notebook::RemovePage(size_t page, bool notify)
{
    if(GetPageCount() <= page) {
        return false;
    }
    wxWindow* win = GetPage(page);
    if(!win) {
        return false;
    }

    if(notify) {
        wxBookCtrlEvent event(wxEVT_BOOK_PAGE_CLOSING);
        event.SetEventObject(this);
        event.SetSelection(page);
        GetEventHandler()->ProcessEvent(event);
        if(!event.IsAllowed()) {
            // Vetoed
            return false;
        }
    }

    m_userData.erase(win);
    m_history->Pop(win);
    bool deletingSelection = (page == GetSelection());
    wxWindow* nextSelection = DoUpdateHistoryPreRemove(win);
    wxNotebook::RemovePage(page);

    // notify about removal
    if(notify) {
        wxBookCtrlEvent event(wxEVT_BOOK_PAGE_CLOSED);
        event.SetEventObject(this);
        GetEventHandler()->ProcessEvent(event);
    }

    DoUpdateHistoryPostRemove(nextSelection, deletingSelection);
    return true;
}

bool Notebook::RemovePage(size_t page) { return RemovePage(page, false); }

bool Notebook::DeletePage(size_t page, bool notify)
{
    if(GetPageCount() <= page) {
        return false;
    }
    wxWindow* win = GetPage(page);
    if(!win) {
        return false;
    }

    // Can we close this page?
    if(notify) {
        wxBookCtrlEvent eventClosing(wxEVT_BOOK_PAGE_CLOSING);
        eventClosing.SetEventObject(this);
        eventClosing.SetSelection(page);
        GetEventHandler()->ProcessEvent(eventClosing);
        if(!eventClosing.IsAllowed()) {
            // Vetoed
            return false;
        }
    }

    bool deletingSelection = (page == GetSelection());
    wxWindow* nextSelection = DoUpdateHistoryPreRemove(win);

    m_history->Pop(win);
    wxNotebook::DeletePage(page);
    m_userData.erase(win);

    // notify about removal
    if(notify) {
        wxBookCtrlEvent eventClosed(wxEVT_BOOK_PAGE_CLOSED);
        eventClosed.SetEventObject(this);
        GetEventHandler()->ProcessEvent(eventClosed);
    }

    DoUpdateHistoryPostRemove(nextSelection, deletingSelection);
    return true;
}

bool Notebook::DeletePage(size_t page) { return DeletePage(page, true); }

void Notebook::TabButtonClicked(wxWindow* page)
{
    int where = GetPageIndex(page);
    if(where == wxNOT_FOUND) {
        return;
    }
    if(m_bookStyle & kNotebook_CloseButtonOnActiveTabFireEvent) {
        // just fire an event
        // let the user process this
        wxBookCtrlEvent event(wxEVT_BOOK_PAGE_CLOSE_BUTTON);
        event.SetEventObject(this);
        event.SetSelection(where);
        GetEventHandler()->ProcessEvent(event);
    } else {
        // Close the tab (the below will fire events)
        DeletePage(where);
    }
}

void Notebook::Initialise(long style)
{
    m_history.reset(new clTabHistory());
    m_bookStyle = (style & ~wxWINDOW_STYLE_MASK);
    if(!(m_bookStyle & kNotebook_CloseButtonOnActiveTab)) {
        SetPadding(wxSize(5, 5));
    }
    BindEvents();
}

void Notebook::DoFinaliseAddPage(wxWindow* page, const wxString& shortlabel, const wxBitmap& bmp)
{
    // do we need to add buton?
    int index = GetPageIndex(page);
    if(index == wxNOT_FOUND) {
        return;
    }

    if(m_userData.count(page) == 0) {
        m_userData.insert({ page, {} });
    }
    auto& data = m_userData[page];
    data.tooltip = shortlabel;
    data.bitmap = bmp;

    wxGtkNotebookPage* p = GetNotebookPage(index);
    if(m_bookStyle & kNotebook_CloseButtonOnActiveTab) {
        GtkToolItem* button = gtk_tool_button_new(nullptr, "✖");

        // remove the label and insert it back at the start

        // add nother reference before removing it, since we plan on reusing it later
        g_object_ref(p->m_label);
        gtk_container_remove(GTK_CONTAINER(p->m_box), p->m_label);

        gtk_box_pack_start(GTK_BOX(p->m_box), p->m_label, TRUE, TRUE, 20);
        gtk_box_pack_end(GTK_BOX(p->m_box), GTK_WIDGET(button), FALSE, FALSE, 0);

        gtk_widget_show_all(p->m_box);

        // connect gtk signals
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_button_clicked), page);
    }

    if(m_bookStyle & kNotebook_AllowDnD) {
        // we allow tabs to be moved around
        gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(GetHandle()), GTK_WIDGET(page->GetHandle()), true);
    }
}

void Notebook::TabReordered()
{
    // we need to update two data structures:
    // m_pagesData & m_pages

    // build a map with the correct order of pages
    int pageCount = GetPageCount();
    GtkNotebook* gbook = GTK_NOTEBOOK(GetHandle());
    std::vector<wxWindow*> modifiedPages;
    std::vector<wxGtkNotebookPage*> modifiedPagesDataList;
    modifiedPages.resize(m_pages.size());
    modifiedPagesDataList.resize(m_pages.size());
    for(int i = 0; i < pageCount; ++i) {
        GtkWidget* page = gtk_notebook_get_nth_page(gbook, i);
        int curidx = FindPageByGTKHandle(page);
        modifiedPages[i] = GetPage(curidx);
        modifiedPagesDataList[i] = m_pagesData[curidx];
    }

    // update the lists
    m_pages.clear();
    for(wxWindow* win : modifiedPages) {
        m_pages.push_back(win);
    }
    m_pagesData.clear();
    for(size_t i = 0; i < modifiedPagesDataList.size(); ++i) {
        m_pagesData.push_back(modifiedPagesDataList[i]);
    }
}

void Notebook::SetTabDirection(wxDirection d)
{
    GtkPositionType pos;
    switch(d) {
    case wxLEFT:
        pos = GTK_POS_LEFT;
        break;
    case wxRIGHT:
        pos = GTK_POS_RIGHT;
        break;
    case wxBOTTOM:
        pos = GTK_POS_BOTTOM;
        break;
    case wxTOP:
        pos = GTK_POS_TOP;
        break;
    default:
        pos = GTK_POS_TOP;
        break;
    }
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(GetHandle()), pos);
}

void Notebook::EnableStyle(NotebookStyle style, bool enable)
{
    wxUnusedVar(style);
    wxUnusedVar(enable);
}

wxWindow* Notebook::GetCurrentPage() const
{
    if(GetSelection() == wxNOT_FOUND) {
        return nullptr;
    }
    return GetPage(GetSelection());
}

bool Notebook::DeleteAllPages()
{
    if(GetPageCount() == 0) {
        return true;
    }
    while(GetPageCount()) {
        DeletePage(0, false);
    }
    m_history->Clear();
    m_userData.clear();
    return true;
}

void Notebook::SetPageBitmap(size_t index, const wxBitmap& bmp)
{
    wxWindow* win = GetPage(index);
    if(m_userData.count(win) == 0) {
        return;
    }
    m_userData[win].bitmap = bmp;
}

wxBitmap Notebook::GetPageBitmap(size_t index) const
{
    auto iter = m_userData.find(GetPage(index));
    if(iter == m_userData.end()) {
        return wxNullBitmap;
    }
    return iter->second.bitmap;
}

int Notebook::GetPageIndex(const wxString& label) const
{
    for(size_t i = 0; i < GetPageCount(); ++i) {
        if(GetPageText(i) == label) {
            return i;
        }
    }
    return wxNOT_FOUND;
}

void Notebook::GetAllPages(std::vector<wxWindow*>& pages)
{
    for(size_t i = 0; i < GetPageCount(); ++i) {
        pages.push_back(GetPage(i));
    }
}

bool Notebook::SetPageToolTip(size_t page, const wxString& tooltip)
{
    wxWindow* win = GetPage(page);
    if(m_userData.count(win) == 0) {
        return false;
    }
    m_userData[win].tooltip = tooltip;
    return true;
}

bool Notebook::MoveActivePage(int newIndex)
{
    wxUnusedVar(newIndex);
    return false;
}
clTabHistory::Ptr_t Notebook::GetHistory() const
{
    // return the history
    return m_history;
}

size_t Notebook::GetAllTabs(clTabInfo::Vec_t& tabs)
{
    for(size_t i = 0; i < GetPageCount(); ++i) {
        clTabInfo::Ptr_t info(new clTabInfo(nullptr, 0, GetPage(i), GetPageText(i), GetPageBitmap(i)));
        tabs.push_back(info);
    }
    return tabs.size();
}

#endif // __WXGTK3__
