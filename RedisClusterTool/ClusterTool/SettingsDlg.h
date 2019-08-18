#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H
#include "wxcrafter.h"

class SettingsDlg : public SettingsDlgBase
{
public:
    SettingsDlg(wxWindow* parent);
    virtual ~SettingsDlg();
    
    wxString GetRedisServer() const;
    wxString GetRedisCli() const;
};
#endif // SETTINGSDLG_H
