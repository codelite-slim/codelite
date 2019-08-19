#include "ClusterConfig.h"
#include "ClusterPage.h"
#include "Utils.hpp"
#include <wx/confbase.h>
#include <wx/ffile.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/tokenzr.h>
#include <wx/utils.h>

ClusterConfig::ClusterConfig(ClusterPage* win)
{
    SetName(win->GetClusterName());
    SetHasReplica(win->HasReplica());
    SetPath(win->GetClusterPath());
    SetFirstPort(win->GetFirstPort());
    SetSize(win->GetClusterSize());
}

ClusterConfig::ClusterConfig(const wxString& name)
    : m_name(name)
{
}

ClusterConfig::~ClusterConfig()
{
}

void ClusterConfig::Load()
{
    wxConfigBase* pConfig = wxConfigBase::Get();
    wxString prefix = GetPrefix();
    if(!prefix.IsEmpty()) {
        pConfig->Read(wxString() << prefix << "/path", &m_path);
        pConfig->Read(wxString() << prefix << "/firstPort", &m_firstPort, m_firstPort);
        pConfig->Read(wxString() << prefix << "/size", &m_size, m_size);
        pConfig->Read(wxString() << prefix << "/flags", &m_flags, m_flags);
    }
}

void ClusterConfig::Save()
{
    wxConfigBase* pConfig = wxConfigBase::Get();
    wxString prefix = GetPrefix();
    if(!prefix.IsEmpty()) {
        pConfig->Write(wxString() << prefix << "/path", m_path);
        pConfig->Write(wxString() << prefix << "/firstPort", m_firstPort);
        pConfig->Write(wxString() << prefix << "/size", m_size);
        pConfig->Write(wxString() << prefix << "/flags", m_flags);
        pConfig->Flush();
    }
}

wxArrayString ClusterConfig::GetAllClusters()
{
    wxConfigBase* conf = wxConfigBase::Get();
    wxString clusters;
    conf->Read(wxString() << "/Clusters", &clusters, wxEmptyString);
    wxArrayString arr = ::wxStringTokenize(clusters, ",");
    return arr;
}

void ClusterConfig::SetClusters(const wxArrayString& arr)
{
    wxConfigBase* conf = wxConfigBase::Get();
    wxString clusters = ::wxJoin(arr, ',');
    conf->Write("/Clusters", clusters);
    conf->Flush();
}

wxString ClusterConfig::GetPrefix() const
{
    if(m_name.IsEmpty()) {
        return "";
    }
    return (wxString() << "/Cluster/" << m_name);
}

void ClusterConfig::Deploy()
{
    // Create the folder
    wxFileName path(GetDeploymentPath(), "");
    path.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);

    int port = GetFirstPort();
    int size = GetSize();
    for(int i = port; i < (port + size); ++i) {
        wxFileName d(path.GetFullPath(), "");
        d.AppendDir(wxString() << i);
        d.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
        wxFileName redisConfig(d.GetFullPath(), GetRedisConfigFileName(i));
        wxFileName logfile(d.GetFullPath(), "redis.log");
        wxFFile fp;
        fp.Open(redisConfig.GetFullPath(), "w+b");
        if(fp.IsOpened()) {
            wxString content;
            content << "port " << i << "\n"
                    << "cluster-enabled yes\n"
                    << "cluster-config-file nodes.conf\n"
                    << "cluster-node-timeout 5000\n"
                    << "appendonly yes\n"
                    << "logfile " << logfile.GetFullPath() << "\n";
            fp.Write(content);
            fp.Close();
        }
    }
}

wxString ClusterConfig::GetDeploymentPath() const
{
    wxFileName f(GetPath(), "");
    f.AppendDir(GetName());
    return f.GetPath();
}

void ClusterConfig::Run()
{
    wxConfigBase* conf = wxConfigBase::Get();
    wxString redisServer;
    conf->Read("/Settings/redis-server", &redisServer);
    if(!wxFileName::FileExists(redisServer)) {
        ::wxMessageBox("Unable to locate redis-server", "Error", wxICON_ERROR | wxCENTRE);
        return;
    }

    wxString redisCli;
    conf->Read("/Settings/redis-cli", &redisCli);
    if(!wxFileName::FileExists(redisCli)) {
        ::wxMessageBox("Unable to locate redis-cli", "Error", wxICON_ERROR | wxCENTRE);
        return;
    }

    // Just incase, deploy it
    Deploy();

    // Execute the instances
    int port = GetFirstPort();
    int size = GetSize();
    for(int i = port; i < (port + size); ++i) {
        wxFileName d(GetDeploymentPath(), "");
        d.AppendDir(wxString() << i);
        wxFileName redisConfig(d.GetFullPath(), GetRedisConfigFileName(i));

        DirSaver ds;
        ::wxSetWorkingDirectory(redisConfig.GetPath());

        wxString command;
        command << redisServer;
        command = Utils::WrapWithQuotes(command);
        command << " " << GetRedisConfigFileName(i);
        if(::wxExecute(command, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER) <= 0) {
            ::wxMessageBox(wxString() << "Unable to execute: " << command, "Error", wxICON_ERROR | wxCENTRE);
            return;
        }
    }
    
    // Now that we have a running instances
    // Form it into a cluster
    wxString clusterCommand;
    clusterCommand = redisCli;
    clusterCommand = Utils::WrapWithQuotes(clusterCommand);
    clusterCommand << " --cluster create";
    for(int i = port; i < (port + size); ++i) {
        clusterCommand << " 127.0.0.1:" << i;
    }
    
    if(HasReplica()) {
        clusterCommand << " --cluster-replicas 1 || read";
    }
    
    wxString terminalCommand;
    wxString scriptContent;
    scriptContent << "/usr/bin/gnome-terminal -e '" << clusterCommand << "'\n";
    scriptContent << "echo Hit any key to continue\n";
    scriptContent << "read\n";
    wxFFile scriptFile("/tmp/create-cluster.sh", "w+b");
    if(scriptFile.IsOpened()) {
        scriptFile.Write(scriptContent);
        scriptFile.Close();
    }
    terminalCommand << "/bin/bash -f /tmp/create-cluster.sh";
    wxExecute(terminalCommand, wxEXEC_SYNC);
}

wxString ClusterConfig::GetRedisConfigFileName(int port) const
{
    //wxString f;
    //f << "redis." << port << ".conf";
    //return f;
    return "redis.conf";
}
