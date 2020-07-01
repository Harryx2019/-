#ifndef ADDNEWBUS_H_INCLUDED
#define ADDNEWBUS_H_INCLUDED

int AddNewBus(BusMap &Map);
void AddNewBusToMap(BusMap &Map,char *newBus,char *newStart,char *newEnd);
void AddNewBusRouteToMap(BusMap &Map,char *newBus);

void AddNewBusToArray(char *newBus,char *newStart,char *newEnd);

#endif // ADDNEWBUS_H_INCLUDED
