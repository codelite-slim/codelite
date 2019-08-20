#include "NodeInfo.h"
#include <wx/tokenzr.h>

NodeInfo::NodeInfo()
{
}

NodeInfo::~NodeInfo()
{
}

wxString NodeInfo::ToString() const
{
    wxString output;
    output << "Node ID: " << GetId() << ", Addr: " << GetAddr() << ", Is Master:" << (IsMaster() ? "Yes" : "No");
    if(IsSlave() && !GetReplicateOf().IsEmpty()) {
        output << ", Replicates: " <<  GetReplicateOf();
    }
    return output;
}

bool NodeInfo::FromString(const wxString& str)
{
    wxArrayString tokens = ::wxStringTokenize(str, " ");
    if(tokens.size() < 8) {
        return false;
    }
    if(tokens.Item(7) != "connected") {
        return false;
    }

    SetId(tokens.Item(0));
    SetAddr(tokens.Item(1));
    wxString port = GetAddr().AfterFirst(':');
    port = port.BeforeFirst('@');
    long nPort = -1;
    if(port.ToCLong(&nPort)) {
        SetPort(nPort);
    }

    SetType(tokens.Item(2));
    if(tokens.Item(3) != "-" ) {
        SetReplicateOf(tokens.Item(3));
    }
    return true;
}
