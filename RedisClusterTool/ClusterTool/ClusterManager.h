#ifndef CLUSTERMANAGER_H
#define CLUSTERMANAGER_H

#include "NodeInfo.h"
class ClusterManager
{
public:
    ClusterManager();
    ~ClusterManager();
    
    static ClusterManager& Get();
    
    NodeInfo::Vec_t GetNodes(int mainPort) const;
};

#endif // CLUSTERMANAGER_H
