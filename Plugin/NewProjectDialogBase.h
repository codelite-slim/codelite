//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: NewProjectDialogBase.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _CODELITE_PLUGIN_NEWPROJECTDIALOGBASE_BASE_CLASSES_H
#define _CODELITE_PLUGIN_NEWPROJECTDIALOGBASE_BASE_CLASSES_H

// clang-format off
#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/filepicker.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/arrstr.h>
#include <wx/button.h>
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

class NewProjectDialogBase : public wxDialog
{
protected:
    wxStaticText* m_staticText20;
    wxDirPickerCtrl* m_dirPicker;
    wxStaticText* m_staticText12;
    wxTextCtrl* m_textCtrlName;
    wxCheckBox* m_checkBoxSepFolder;
    wxStaticText* m_staticText36;
    wxChoice* m_choiceCategory;
    wxStaticText* m_staticText16;
    wxChoice* m_choiceType;
    wxStaticText* m_staticText24;
    wxChoice* m_choiceCompiler;
    wxStaticText* m_staticText28;
    wxChoice* m_choiceDebugger;
    wxStaticText* m_staticText32;
    wxChoice* m_choiceBuild;
    wxStdDialogButtonSizer* m_stdBtnSizer4;
    wxButton* m_button6;
    wxButton* m_button8;

protected:
    virtual void OnPathSelected(wxFileDirPickerEvent& event) { event.Skip(); }
    virtual void OnNameTyped(wxCommandEvent& event) { event.Skip(); }
    virtual void OnCategoryChanged(wxCommandEvent& event) { event.Skip(); }
    virtual void OnOKUI(wxUpdateUIEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticText20() { return m_staticText20; }
    wxDirPickerCtrl* GetDirPicker() { return m_dirPicker; }
    wxStaticText* GetStaticText12() { return m_staticText12; }
    wxTextCtrl* GetTextCtrlName() { return m_textCtrlName; }
    wxCheckBox* GetCheckBoxSepFolder() { return m_checkBoxSepFolder; }
    wxStaticText* GetStaticText36() { return m_staticText36; }
    wxChoice* GetChoiceCategory() { return m_choiceCategory; }
    wxStaticText* GetStaticText16() { return m_staticText16; }
    wxChoice* GetChoiceType() { return m_choiceType; }
    wxStaticText* GetStaticText24() { return m_staticText24; }
    wxChoice* GetChoiceCompiler() { return m_choiceCompiler; }
    wxStaticText* GetStaticText28() { return m_staticText28; }
    wxChoice* GetChoiceDebugger() { return m_choiceDebugger; }
    wxStaticText* GetStaticText32() { return m_staticText32; }
    wxChoice* GetChoiceBuild() { return m_choiceBuild; }
    NewProjectDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("New Project"),
                         const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1, -1),
                         long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    virtual ~NewProjectDialogBase();
};

#endif
