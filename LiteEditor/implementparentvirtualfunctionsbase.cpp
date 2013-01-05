//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "implementparentvirtualfunctionsbase.h"


// Declare the bitmap loading function
extern void wxCA6AAInitBitmapResources();

static bool bBitmapLoaded = false;


ImplementParentVirtualFunctionsBase::ImplementParentVirtualFunctionsBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCA6AAInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(mainSizer);
    
    m_banner4 = new wxBannerWindow(this, wxID_ANY, wxBOTTOM, wxDefaultPosition, wxSize(-1,-1), wxBORDER_THEME);
    m_banner4->SetBitmap(wxNullBitmap);
    m_banner4->SetText(_("Implmenet inherited virtual functions"), _("Select from the list below the functions that you want to override in your class"));
    m_banner4->SetGradient(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION), wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
    m_banner4->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT));
    
    mainSizer->Add(m_banner4, 0, wxALL|wxEXPAND, 5);
    
    wxBoxSizer* bSizer4 = new wxBoxSizer(wxHORIZONTAL);
    
    mainSizer->Add(bSizer4, 0, wxTOP|wxBOTTOM|wxEXPAND, 5);
    
    m_staticText2 = new wxStaticText(this, wxID_ANY, _("File:"), wxDefaultPosition, wxSize(-1, -1), 0);
    
    bSizer4->Add(m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    
    m_textCtrlImplFile = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1, -1), 0);
    m_textCtrlImplFile->SetToolTip(_("Generate the functions in this filename"));
    
    bSizer4->Add(m_textCtrlImplFile, 1, wxALL|wxEXPAND, 5);
    
    wxBoxSizer* boxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    
    mainSizer->Add(boxSizer6, 1, wxALL|wxEXPAND, 5);
    
    m_dataview = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxDV_VERT_RULES|wxDV_HORIZ_RULES|wxDV_ROW_LINES|wxDV_SINGLE);
    m_dataview->SetToolTip(_("Select which function to generate by ticking the '?' column\nAll fields on this table are editable"));
    
    m_dataviewModel = new FunctionsModel;
    m_dataviewModel->SetColCount( 5 );
    m_dataview->AssociateModel(m_dataviewModel.get() );
    
    boxSizer6->Add(m_dataview, 1, wxEXPAND, 5);
    
    m_dataview->AppendToggleColumn(_("?"), m_dataview->GetColumnCount(), wxDATAVIEW_CELL_ACTIVATABLE, -2, wxALIGN_CENTER);
    m_dataview->AppendTextColumn(_("Name"), m_dataview->GetColumnCount(), wxDATAVIEW_CELL_INERT, 200, wxALIGN_LEFT);
    {
        wxArrayString choices;
        choices.Add(_("public"));
        choices.Add(_("protected"));
        choices.Add(_("private"));
        m_dataview->AppendColumn( new wxDataViewColumn( _("Visibility"), new wxDataViewChoiceRenderer( choices, wxDATAVIEW_CELL_EDITABLE, wxDVR_DEFAULT_ALIGNMENT), m_dataview->GetColumnCount(), -2, wxALIGN_LEFT));
    }
    
    m_dataview->AppendToggleColumn(_("Virtual"), m_dataview->GetColumnCount(), wxDATAVIEW_CELL_ACTIVATABLE, -2, wxALIGN_LEFT);
    m_dataview->AppendToggleColumn(_("Generate doxygen comment?"), m_dataview->GetColumnCount(), wxDATAVIEW_CELL_ACTIVATABLE, -2, wxALIGN_LEFT);
    wxBoxSizer* boxSizer8 = new wxBoxSizer(wxVERTICAL);
    
    boxSizer6->Add(boxSizer8, 0, wxEXPAND, 5);
    
    m_button10 = new wxButton(this, wxID_ANY, _("Check all"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    boxSizer8->Add(m_button10, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 5);
    
    m_button12 = new wxButton(this, wxID_ANY, _("Uncheck all"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    boxSizer8->Add(m_button12, 0, wxALL|wxEXPAND, 5);
    
    wxStaticBoxSizer* sbSizer1 = new wxStaticBoxSizer( new wxStaticBox(this, wxID_ANY, _("Options:")), wxVERTICAL);
    
    mainSizer->Add(sbSizer1, 0, wxALL|wxEXPAND, 5);
    
    m_checkBoxFormat = new wxCheckBox(this, wxID_ANY, _("Format text after insertion"), wxDefaultPosition, wxSize(-1, -1), 0);
    m_checkBoxFormat->SetValue(false);
    
    sbSizer1->Add(m_checkBoxFormat, 0, wxALL, 5);
    
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    
    mainSizer->Add(buttonSizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
    
    m_buttonOk = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxSize(-1, -1), 0);
    m_buttonOk->SetDefault();
    
    buttonSizer->Add(m_buttonOk, 0, wxALL, 5);
    
    m_buttonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxSize(-1, -1), 0);
    
    buttonSizer->Add(m_buttonCancel, 0, wxALL, 5);
    
    
    SetSizeHints(-1,400);
    if ( GetSizer() ) {
         GetSizer()->Fit(this);
    }
    Centre(wxBOTH);
    // Connect events
    m_dataview->Connect(wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler(ImplementParentVirtualFunctionsBase::OnValueChanged), NULL, this);
    m_button10->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImplementParentVirtualFunctionsBase::OnCheckAll), NULL, this);
    m_button12->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImplementParentVirtualFunctionsBase::OnUnCheckAll), NULL, this);
    
}

ImplementParentVirtualFunctionsBase::~ImplementParentVirtualFunctionsBase()
{
    m_dataview->Disconnect(wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler(ImplementParentVirtualFunctionsBase::OnValueChanged), NULL, this);
    m_button10->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImplementParentVirtualFunctionsBase::OnCheckAll), NULL, this);
    m_button12->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImplementParentVirtualFunctionsBase::OnUnCheckAll), NULL, this);
    
}
