#ifndef LOADMAPDATA_H_INCLUDED
#define LOADMAPDATA_H_INCLUDED

void LoadMapData(BusMap &Map);
void AddNewNodeToMap(BusMap Map,int nBus,int nStart,int nEnd,int distance);

int FindBus(BusMap Map,char *bus);
int FindStation(BusMap Map,char *station);
int FindStationInBus(BusMap Map,int nBus,int nStation);
int FindPreStationInBus(BusMap Map,int nBus,int nStation,int &distance);
int FindPostStationInBus(BusMap Map,int nBus,int nStation,int &distance);

void PrintMap(BusMap Map);

#endif // LOADMAPDATA_H_INCLUDED
