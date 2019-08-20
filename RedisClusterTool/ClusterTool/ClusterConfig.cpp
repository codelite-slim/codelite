#include "ClusterConfig.h"
#include "ClusterPage.h"
#include "Utils.hpp"
#include <wx/confbase.h>
#include <wx/ffile.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/tokenzr.h>
#include <wx/utils.h>
#include "MainFrame.h"
#include "NodeInfo.h"
#include "ClusterManager.h"
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

ClusterConfig::~ClusterConfig() {}

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

void ClusterConfig::CreateConfigurations()
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
    wxBusyCursor bc;
    wxConfigBase* conf = wxConfigBase::Get();
    wxString redisServer;
    conf->Read("/Settings/redis-server", &redisServer);
    if(!wxFileName::FileExists(redisServer)) {
        ::wxMessageBox("Unable to locate redis-server", "Error", wxICON_ERROR | wxCENTRE);
        return;
    }
    redisServer = Utils::WrapWithQuotes(redisServer);
    
    wxString redisCli;
    conf->Read("/Settings/redis-cli", &redisCli);
    if(!wxFileName::FileExists(redisCli)) {
        ::wxMessageBox("Unable to locate redis-cli", "Error", wxICON_ERROR | wxCENTRE);
        return;
    }
    
    redisCli = Utils::WrapWithQuotes(redisCli);
    // Just incase, deploy it. This will create all the needed files
    MainFrame::Log("Generating configuration files...");
    CreateConfigurations();
    
    int port = GetFirstPort();
    int size = GetSize();
    wxString runInstancesScript;
    wxString stopInstancesScript;
    wxString runValgrindInstancesScript;
    runInstancesScript << "#!/bin/bash\n";
    stopInstancesScript << "#!/bin/bash\n";
    runValgrindInstancesScript << "#!/bin/bash\n";
    
    stopInstancesScript << 
"stopRedis()\n"
"{\n"
"    port=$1\n"
"    re='^[0-9]+$'\n"
"    echo Stopping redis-server port: $port\n"
"    pid=`ps -ef|grep redis-server|grep $port|awk '{print $2;}'`\n"
"    if [[ $pid =~ $re ]] ; then\n"
"        kill $pid;\n"
"    fi\n"
"}\n\n"
;

    runInstancesScript << "base_dir=" << Utils::WrapWithQuotes(GetDeploymentPath()) << "\n";
    runInstancesScript << "redis_server=" << redisServer << "\n";
    
    runValgrindInstancesScript << "base_dir=" << Utils::WrapWithQuotes(GetDeploymentPath()) << "\n";
    runValgrindInstancesScript << "redis_server=" << redisServer << "\n";
    
    for(int i = port; i < (port + size); ++i) {
        wxFileName d(GetDeploymentPath(), "");
        d.AppendDir(wxString() << i);
        wxFileName redisConfig(d.GetFullPath(), GetRedisConfigFileName(i));
        wxString command;
        command << redisServer;
        command << " " << GetRedisConfigFileName(i);
        runInstancesScript << "cd $base_dir/" << i << " && $redis_server redis.conf &\n";
        runValgrindInstancesScript << "cd $base_dir/" << i 
                                   << " && /usr/bin/valgrind --log-file=/tmp/redis.vg." << i << ".log $redis_server redis.conf &\n";
        stopInstancesScript << "stopRedis " << i << "\n";
    }
    
    // Create the run script
    MainFrame::Log("Generating run-instances.sh script...");
    WriteScript("run-instances.sh", runInstancesScript);
    
    MainFrame::Log("Generating run-vg-instances.sh script...");
    WriteScript("run-vg-instances.sh", runValgrindInstancesScript);
    
    MainFrame::Log("Generating stop-instances.sh script...");
    WriteScript("stop-instances.sh", stopInstancesScript);
    
    // Run the cluster
    RunScript("run-instances.sh");
    
    // Now, add each node into the cluster
    MainFrame::Log("Forming cluster...");
    for(int i = (port + 1); i < (port + size); ++i) {
        AddNodeToCluster(redisCli, port, i);
    }

    if(HasReplica()) {
        ::wxSleep(5);
        // We need to get list of nodes
        bool foundReplicates = false;
        NodeInfo::Vec_t nodes = ClusterManager::Get().GetNodes(port);
        for(const NodeInfo& node : nodes) {
            if(!node.GetReplicateOf().IsEmpty()) {
                foundReplicates = true;
                break;
            }
        }

        if(foundReplicates) {
            ::wxMessageBox("I see that replication is already configured. Will not change this");
            return;
        }
        
        // Split the list into 2
        // masters and replicas
        size_t middle = nodes.size()/2;
        NodeInfo::Vec_t masters(nodes.begin(), nodes.begin() + middle);
        NodeInfo::Vec_t slaves(nodes.begin() + middle, nodes.end());
        size_t size = std::min(masters.size(), slaves.size());
        MainFrame::Log("Creating replicas...");
        for(size_t i=0; i<size; ++i) {
            // connect to the slave and tell it replicate the master
            wxString command;
            const NodeInfo& slave = slaves[i];
            const NodeInfo& master = masters[i];
            command << redisCli << " -p " << slave.GetPort() << " -c CLUSTERADMIN REPLICATE " << master.GetId();
            MainFrame::Log(command);
            ::wxExecute(command, wxEXEC_SYNC | wxEXEC_MAKE_GROUP_LEADER);
        }
        MainFrame::Log("Done");
    }
}

wxString ClusterConfig::GetRedisConfigFileName(int port) const
{
    return "redis.conf";
}

void ClusterConfig::AddNodeToCluster(const wxString& redisCli, int mainPort, int portToAdd)
{
    // build the command to execute:
    wxString command;
    command << redisCli;
    command << " -p " << mainPort << " -c CLUSTERADMIN MEET 127.0.0.1 " << portToAdd;
    MainFrame::Log(command, 1);
    ::wxExecute(command, wxEXEC_SYNC | wxEXEC_MAKE_GROUP_LEADER);
}

void ClusterConfig::CreateScripts()
{
}

void ClusterConfig::WriteScript(const wxString& name, const wxString& content)
{
    wxFileName script(GetDeploymentPath(), name);
    wxFFile fp(script.GetFullPath(), "w+b");
    if(!fp.IsOpened()) { return; }
    fp.Write(content);
    fp.Close();
    chmod(script.GetFullPath().mb_str().data(), 0775);
}

void ClusterConfig::RunScript(const wxString& name)
{
    wxFileName script(GetDeploymentPath(), name);
    wxExecute(script.GetFullPath(), wxEXEC_SYNC);
}
