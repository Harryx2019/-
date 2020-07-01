#ifndef ADDNEWROUTE_H_INCLUDED
#define ADDNEWROUTE_H_INCLUDED

int AddNewRoute(BusMap &Map);
int AddNewRouteToMap(BusMap &Map,int nBus,int nStart1,int nEnd1,int distance1);

int JudgeRouteType(BusMap Map,int nBus,int nStart1,int nEnd1);
int AddNewRouteFlag0(BusMap Map,int nBus,int nStart1,int nEnd1,int distance1,int distance2);
void AddNewRouteFlag12(BusMap &Map,int nBus,int nStart,int nEnd,int distance,int flag);
int AddNewRouteFlag3(BusMap Map,int nBus,int nStart1,int nEnd1,int distance1,int distance2);

void AddNewRouteToArray(int nBus,int nStart,int nEnd,int nDistance);

#endif // ADDNEWROUTE_H_INCLUDED
