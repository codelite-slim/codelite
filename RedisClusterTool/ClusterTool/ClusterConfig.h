#ifndef CLUSTERCONFIG_H
#define CLUSTERCONFIG_H

#include <wx/arrstr.h>
#include <wx/string.h>
#include <wx/filename.h>
#include "NodeInfo.h"

class ClusterPage;
class ClusterConfig
{
public:
    enum eFlags {   
                    kEnableReplica = (1 << 0), 
                    kProtectedMode = (1<<1),
                };

protected:
    wxString m_name;
    wxString m_path;
    int m_firstPort = 7000;
    int m_size = 6;
    int m_flags = kEnableReplica;

protected:
    wxString GetPrefix() const;
    void AddNodeToCluster(const wxString& redisCli, int mainPort, int portToAdd, int clusterSize);
    void CreateScripts();
    void WriteScript(const wxString& name, const wxString& content);
    void RunScript(const wxString& name);
    void AssignSlots(const NodeInfo::Vec_t& nodes, const wxString& redisCli);

public:
    ClusterConfig(const wxString& name = "");
    ClusterConfig(ClusterPage* win);
    ~ClusterConfig();

    void Load();
    void Save();
    void CreateConfigurations();
    void Run();

    static wxArrayString GetAllClusters();
    static void SetClusters(const wxArrayString& arr);

    wxString GetDeploymentPath() const;
    bool HasFlag(ClusterConfig::eFlags flag) const { return m_flags & flag; }

    void SetHasReplica(bool b)
    {
        if(b) {
            m_flags |= kEnableReplica;
        } else {
            m_flags &= ~kEnableReplica;
        }
    }
    
    void SetProtectedMode(bool b)
    {
        if(b) {
            m_flags |= kProtectedMode;
        } else {
            m_flags &= ~kProtectedMode;
        }
    }

    bool HasReplica() const { return m_flags & kEnableReplica; }
    bool EnableProtectedMode() const { return m_flags & kProtectedMode; }
    
    ClusterConfig& SetFirstPort(int firstPort)
    {
        this->m_firstPort = firstPort;
        return *this;
    }
    ClusterConfig& SetName(const wxString& name)
    {
        this->m_name = name;
        return *this;
    }
    ClusterConfig& SetPath(const wxString& path)
    {
        this->m_path = path;
        return *this;
    }
    ClusterConfig& SetSize(int size)
    {
        this->m_size = size;
        return *this;
    }
    int GetFirstPort() const { return m_firstPort; }
    int GetFlags() const { return m_flags; }
    const wxString& GetName() const { return m_name; }
    const wxString& GetPath() const { return m_path; }
    int GetSize() const { return m_size; }
    wxString GetRedisConfigFileName(int port) const;
};

#endif // CLUSTERCONFIG_H
