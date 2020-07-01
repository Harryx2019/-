#ifndef REMOVEROUTE_H_INCLUDED
#define REMOVEROUTE_H_INCLUDED

int RemoveRoute(BusMap &Map);
int RemoveRouteToMap(BusMap &Map,int nBus,int nStart,int nEnd);
int RemoveRoutesNum(BusMap Map,int nBus,int nStart,int nEnd,int *STARTS,int *ENDS,int *DISTANCES,int &routesNum);
void RemoveRouteToMap2(BusMap &Map,int nBus);

void RemoveNodeToMap(BusMap Map,int nBus,int nStart,int nEnd);
void RemoveRouteFromArray(int nBus,int nStart,int nEnd);

#endif // REMOVEROUTE_H_INCLUDED
