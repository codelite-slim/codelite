//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: NewProjectDialogBase.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "NewProjectDialogBase.h"

// Declare the bitmap loading function
extern void wxCF72DInitBitmapResources();

static bool bBitmapLoaded = false;

NewProjectDialogBase::NewProjectDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos,
                                           const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if(!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCF72DInitBitmapResources();
        bBitmapLoaded = true;
    }

    wxBoxSizer* boxSizer2 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer2);

    wxFlexGridSizer* flexGridSizer10 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer10->SetFlexibleDirection(wxBOTH);
    flexGridSizer10->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
    flexGridSizer10->AddGrowableCol(1);

    boxSizer2->Add(flexGridSizer10, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText20 =
        new wxStaticText(this, wxID_ANY, _("Path:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer10->Add(m_staticText20, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_dirPicker = new wxDirPickerCtrl(this, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition,
                                      wxDLG_UNIT(this, wxSize(-1, -1)), wxDIRP_SMALL | wxDIRP_USE_TEXTCTRL);

    flexGridSizer10->Add(m_dirPicker, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText12 =
        new wxStaticText(this, wxID_ANY, _("Name:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer10->Add(m_staticText12, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_textCtrlName = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(500, -1)), 0);
    m_textCtrlName->SetFocus();
#if wxVERSION_NUMBER >= 3000
    m_textCtrlName->SetHint(wxT(""));
#endif

    flexGridSizer10->Add(m_textCtrlName, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    flexGridSizer10->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));

    m_checkBoxSepFolder = new wxCheckBox(this, wxID_ANY, _("Create the project in its own folder"), wxDefaultPosition,
                                         wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_checkBoxSepFolder->SetValue(false);

    flexGridSizer10->Add(m_checkBoxSepFolder, 0, wxALL | wxALIGN_LEFT, WXC_FROM_DIP(5));

    m_staticText36 =
        new wxStaticText(this, wxID_ANY, _("Category:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer10->Add(m_staticText36, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    wxArrayString m_choiceCategoryArr;
    m_choiceCategory =
        new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), m_choiceCategoryArr, 0);

    flexGridSizer10->Add(m_choiceCategory, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText16 =
        new wxStaticText(this, wxID_ANY, _("Type:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer10->Add(m_staticText16, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    wxArrayString m_choiceTypeArr;
    m_choiceType =
        new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), m_choiceTypeArr, 0);

    flexGridSizer10->Add(m_choiceType, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText24 =
        new wxStaticText(this, wxID_ANY, _("Compiler:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer10->Add(m_staticText24, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    wxArrayString m_choiceCompilerArr;
    m_choiceCompiler =
        new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), m_choiceCompilerArr, 0);

    flexGridSizer10->Add(m_choiceCompiler, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText28 =
        new wxStaticText(this, wxID_ANY, _("Debugger:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer10->Add(m_staticText28, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    wxArrayString m_choiceDebuggerArr;
    m_choiceDebugger =
        new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), m_choiceDebuggerArr, 0);

    flexGridSizer10->Add(m_choiceDebugger, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText32 =
        new wxStaticText(this, wxID_ANY, _("Build System:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer10->Add(m_staticText32, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    wxArrayString m_choiceBuildArr;
    m_choiceBuild =
        new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), m_choiceBuildArr, 0);

    flexGridSizer10->Add(m_choiceBuild, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_stdBtnSizer4 = new wxStdDialogButtonSizer();

    boxSizer2->Add(m_stdBtnSizer4, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(10));

    m_button6 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_button6->SetDefault();
    m_stdBtnSizer4->AddButton(m_button6);

    m_button8 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer4->AddButton(m_button8);
    m_stdBtnSizer4->Realize();

    SetName(wxT("NewProjectDialogBase"));
    SetSize(wxDLG_UNIT(this, wxSize(-1, -1)));
    if(GetSizer()) { GetSizer()->Fit(this); }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
    // Connect events
    m_dirPicker->Connect(wxEVT_COMMAND_DIRPICKER_CHANGED,
                         wxFileDirPickerEventHandler(NewProjectDialogBase::OnPathSelected), NULL, this);
    m_textCtrlName->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(NewProjectDialogBase::OnNameTyped), NULL,
                            this);
    m_choiceCategory->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                              wxCommandEventHandler(NewProjectDialogBase::OnCategoryChanged), NULL, this);
    m_button6->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(NewProjectDialogBase::OnOKUI), NULL, this);
    m_button6->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NewProjectDialogBase::OnOK), NULL, this);
}

NewProjectDialogBase::~NewProjectDialogBase()
{
    m_dirPicker->Disconnect(wxEVT_COMMAND_DIRPICKER_CHANGED,
                            wxFileDirPickerEventHandler(NewProjectDialogBase::OnPathSelected), NULL, this);
    m_textCtrlName->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(NewProjectDialogBase::OnNameTyped),
                               NULL, this);
    m_choiceCategory->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED,
                                 wxCommandEventHandler(NewProjectDialogBase::OnCategoryChanged), NULL, this);
    m_button6->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(NewProjectDialogBase::OnOKUI), NULL, this);
    m_button6->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NewProjectDialogBase::OnOK), NULL, this);
}
