#ifndef SEARCHROUTE_H_INCLUDED
#define SEARCHROUTE_H_INCLUDED

int SearchRoute(BusMap &Map);
int QueryRoute(BusMap &Map,int nStartStation,int nEndStation);
void updateRouteStatus(BusMap &Map,int station,int *stationInStack);
void PrintRoute(BusMap Map,int nStartStation,int nEndStation,int pathNum,int *pathStationNum,int **path);

#endif // SEARCHROUTE_H_INCLUDED
