#ifndef NODEINFO_H
#define NODEINFO_H

#include <vector>
#include <wx/string.h>

class NodeInfo
{
    wxString m_id;
    wxString m_replicateOf;
    wxString m_type; // master / slav
    wxString m_addr;
    int m_port = 0;

public:
    typedef std::vector<NodeInfo> Vec_t;

public:
    NodeInfo();
    ~NodeInfo();
    
    bool FromString(const wxString& str);
    bool IsMaster() const
    {
        return m_type.Contains("master");
    }
    bool IsSlave() const
    {
        return m_type.Contains("slave");
    }
    void SetId(const wxString& id)
    {
        this->m_id = id;
    }
    void SetPort(int port)
    {
        this->m_port = port;
    }
    void SetType(const wxString& type)
    {
        this->m_type = type;
    }
    const wxString& GetId() const
    {
        return m_id;
    }
    int GetPort() const
    {
        return m_port;
    }
    const wxString& GetType() const
    {
        return m_type;
    }
    void SetAddr(const wxString& addr)
    {
        this->m_addr = addr;
    }
    const wxString& GetAddr() const
    {
        return m_addr;
    }

    void SetReplicateOf(const wxString& replicateOf)
    {
        this->m_replicateOf = replicateOf;
    }
    const wxString& GetReplicateOf() const
    {
        return m_replicateOf;
    }
    wxString ToString() const;
};

#endif // NODEINFO_H
