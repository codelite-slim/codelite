#include "LanguageServerPage.h"
#include <algorithm>
#include <macros.h>
#include "LanguageServerProtocol.h"
#include "globals.h"
#include <wx/choicdlg.h>
#include "ColoursAndFontsManager.h"

LanguageServerPage::LanguageServerPage(wxWindow* parent, const LanguageServerEntry& data)
    : LanguageServerPageBase(parent)
{
    LexerConf::Ptr_t lex = ColoursAndFontsManager::Get().GetLexer("text");
    if(lex) { lex->Apply(m_stcCommand); }
    m_textCtrlName->SetValue(data.GetName());
    m_dirPickerWorkingDir->SetPath(data.GetWorkingDirectory());
    m_stcCommand->SetText(data.GetCommand());
    m_checkBoxEnabled->SetValue(data.IsEnabled());
    const wxArrayString& langs = data.GetLanguages();
    wxString languages = wxJoin(langs, ';');
    this->m_textCtrlLanguages->SetValue(languages);
    this->m_comboBoxConnection->SetValue(data.GetConnectionString());
    m_checkBoxDiagnostics->SetValue(data.IsDisaplayDiagnostics());
    m_sliderPriority->SetValue(data.GetPriority());
}

LanguageServerPage::LanguageServerPage(wxWindow* parent)
    : LanguageServerPageBase(parent)
{
    LexerConf::Ptr_t lex = ColoursAndFontsManager::Get().GetLexer("text");
    if(lex) { lex->Apply(m_stcCommand); }
}

LanguageServerPage::~LanguageServerPage() {}

LanguageServerEntry LanguageServerPage::GetData() const
{
    LanguageServerEntry d;
    d.SetName(m_textCtrlName->GetValue());
    d.SetCommand(m_stcCommand->GetText().Trim().Trim(false));
    d.SetWorkingDirectory(m_dirPickerWorkingDir->GetPath());
    d.SetLanguages(GetLanguages());
    d.SetEnabled(m_checkBoxEnabled->IsChecked());
    d.SetConnectionString(m_comboBoxConnection->GetValue());
    d.SetPriority(m_sliderPriority->GetValue());
    d.SetDisaplayDiagnostics(m_checkBoxDiagnostics->IsChecked());
    return d;
}

wxArrayString LanguageServerPage::GetLanguages() const
{
    wxArrayString selectedLanguages;
    wxString langStr = m_textCtrlLanguages->GetValue();
    selectedLanguages = ::wxStringTokenize(langStr, ";,", wxTOKEN_STRTOK);
    return selectedLanguages;
}

void LanguageServerPage::OnSuggestLanguages(wxCommandEvent& event)
{
    const std::set<wxString>& langSet = LanguageServerProtocol::GetSupportedLanguages();
    wxArrayString arrLang;
    for(const wxString& lang : langSet) {
        arrLang.Add(lang);
    }

    wxArrayInt selections;
    int count = ::wxGetSelectedChoices(selections, _("Select the supported languages by this server:"), _("CodeLite"),
                                       arrLang, GetParent());
    if(count == wxNOT_FOUND) { return; }

    wxString newText;
    for(int sel : selections) {
        newText << arrLang.Item(sel) << ";";
    }
    m_textCtrlLanguages->ChangeValue(newText);
}
