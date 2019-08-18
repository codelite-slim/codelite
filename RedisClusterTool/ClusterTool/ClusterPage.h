#ifndef CLUSTERPAGE_H
#define CLUSTERPAGE_H
#include "wxcrafter.h"

class ClusterConfig;
class ClusterPage : public ClusterPageBase
{
public:
    ClusterPage(wxWindow* parent, ClusterConfig* conf = NULL);
    virtual ~ClusterPage();
    
    wxString GetClusterName() const { return m_textCtrlName->GetValue(); }
    void SetClusterName(const wxString& name) { m_textCtrlName->ChangeValue(name); }

    wxString GetClusterPath() const { return m_dirPicker->GetPath(); }
    void SetClusterPath(const wxString& path) { m_dirPicker->SetPath(path); }
    
    int GetFirstPort() const { return m_spinCtrlPort->GetValue(); }
    void SetFirstPort(int port) { m_spinCtrlPort->SetValue(port); }
    
    int GetClusterSize() const { return m_spinCtrl37->GetValue(); }
    void SetClusterSize(int size) { m_spinCtrl37->SetValue(size); }
    
    bool HasReplica() const { return m_checkBox45->IsChecked(); }
    void EnableReplica(bool b) { m_checkBox45->SetValue(b); }
};
#endif // CLUSTERPAGE_H
