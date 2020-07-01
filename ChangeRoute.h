#ifndef CHANGEROUTE_H_INCLUDED
#define CHANGEROUTE_H_INCLUDED

int ChangeRoute(BusMap &Map);
int ChangeRouteToMap(BusMap &Map,int nBus,int nStart,int nEnd);
int ChangeRouteNum(BusMap &Map,int nBus,int nStart,int nEnd,int *routes);

void ChangeRouteStartAsBusStart(BusMap &Map,int nBus,int changeStart,int routeStart,int routeEnd,int routeDistance);
void ChangeRouteEndAsBusEnd(BusMap &Map,int nBus,int changeEnd,int routeStart,int routeEnd,int routeDistance);
void ChangeRouteStart(BusMap &Map,int nBus,int preStation,int changeStart,int preDistance,int newStart,int routeEnd,int routeDistance);
void ChangeRouteEnd(BusMap &Map,int nBus,int postStation,int changeEnd,int postDistance,int routeStart,int newEnd,int routeDistance);

void ChangeNodeToMap(BusMap &Map,int nBus,int nStart,int changeEnd,int changeDistance);
void ChangeRouteArray(int nBus,int nStart,int changeEnd,int changeDistance);

#endif // CHANGEROUTE_H_INCLUDED
