#include "SettingsDlg.h"
#include <wx/confbase.h>

SettingsDlg::SettingsDlg(wxWindow* parent)
    : SettingsDlgBase(parent)
{
    wxConfigBase* conf = wxConfigBase::Get();
    wxString redisCli, redisServer;
    conf->Read("/Settings/redis-server", &redisServer);
    conf->Read("/Settings/redis-cli", &redisCli);
    m_filePickerCli->SetPath(redisCli);
    m_filePickerServer->SetPath(redisServer);
    CentreOnParent();
}

SettingsDlg::~SettingsDlg() {}

wxString SettingsDlg::GetRedisServer() const { return m_filePickerServer->GetPath(); }

wxString SettingsDlg::GetRedisCli() const { return m_filePickerCli->GetPath(); }
