#include "ClusterPage.h"
#include "ClusterConfig.h"

ClusterPage::ClusterPage(wxWindow* parent, ClusterConfig* conf)
    : ClusterPageBase(parent)
{
    if(conf) {
        SetClusterName(conf->GetName());
        SetClusterPath(conf->GetPath());
        SetClusterSize(conf->GetSize());
        SetFirstPort(conf->GetFirstPort());
        EnableReplica(conf->HasReplica());
        m_textCtrlName->Enable(false);
    }
}

ClusterPage::~ClusterPage()
{
}

