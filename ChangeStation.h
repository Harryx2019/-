#ifndef CHANGESTATION_H_INCLUDED
#define CHANGESTATION_H_INCLUDED

int ChangeStation(BusMap &Map);
int ChangeStationToMap(BusMap &Map,int nStation);
int ChangeStationAsStart(BusMap &Map,int nStation);
int ChangeStationAsEnd(BusMap &Map,int nStation);

#endif // CHANGESTATION_H_INCLUDED
