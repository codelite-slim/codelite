//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: GotoAnythingBaseUI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _CODELITE_PLUGIN_GOTOANYTHINGBASEUI_BASE_CLASSES_H
#define _CODELITE_PLUGIN_GOTOANYTHINGBASEUI_BASE_CLASSES_H

// clang-format off
#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/dataview.h>
#include "clThemedListCtrl.h"
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

// clang-format on

class GotoAnythingBaseDlg : public wxDialog
{
protected:
    wxTextCtrl* m_textCtrlSearch;
    clThemedListCtrl* m_dvListCtrl;

protected:
    virtual void OnIdle(wxIdleEvent& event) { event.Skip(); }
    virtual void OnKeyDown(wxKeyEvent& event) { event.Skip(); }
    virtual void OnEnter(wxCommandEvent& event) { event.Skip(); }
    virtual void OnItemActivated(wxDataViewEvent& event) { event.Skip(); }

public:
    wxTextCtrl* GetTextCtrlSearch() { return m_textCtrlSearch; }
    clThemedListCtrl* GetDvListCtrl() { return m_dvListCtrl; }
    GotoAnythingBaseDlg(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Goto Anything"),
                        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1, -1),
                        long style = wxBORDER_NONE);
    virtual ~GotoAnythingBaseDlg();
};

#endif
