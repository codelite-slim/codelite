#include "ClusterManager.h"
#include <wx/confbase.h>
#include "MainFrame.h"
#include <wx/arrstr.h>
#include "Utils.hpp"
#include <wx/tokenzr.h>
#include <unordered_map>

ClusterManager::ClusterManager()
{
}

ClusterManager::~ClusterManager()
{
}

ClusterManager& ClusterManager::Get()
{
    static ClusterManager mgr;
    return mgr;
}

NodeInfo::Vec_t ClusterManager::GetNodes(int mainPort) const
{
    NodeInfo::Vec_t nodes;
    wxConfigBase* conf = wxConfigBase::Get();
    wxString redisCli;
    if(!conf->Read("/Settings/redis-cli", &redisCli)) {
        MainFrame::Log("Unable to locate redis-cli in the settings");
        return nodes;
    }

    wxString command;
    wxArrayString outArr;
    command = Utils::WrapWithQuotes(redisCli);
    command << " -p " << mainPort << " -c cluster nodes";
    ::wxExecute(command, outArr, wxEXEC_SYNC);

    for(const wxString & line : outArr) {
        NodeInfo node;
        if(node.FromString(line)) {
            nodes.push_back(node);
        }
    }
    return nodes;
}
