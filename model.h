#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#define NONE -1

typedef struct Bus
{
    char *name;//公交名
    int start;//起点
    int end;//终点
}Bus;

typedef struct Station
{
    char *station;//站点名
    struct Route *routes;//从该站点出发的所有下行路线的链域
}Station;

typedef struct Route
{
    int station;//指向的站点索引号
    int bus;//公交索引号
    int distance;//两站之间公路的距离
    struct Route *next;//起始站点相同的，下一条下行路线
    int staus=0;//寻找路径时，若该route两站点均不在遍历的栈结构中，则为0
}Route;

typedef struct BusMap
{
    Bus *buses;//公交线路数组
    Station *stations;//站点数组
    int station_num;//站点数
    int bus_num;//公交线路数
}BusMap;

#endif // MODEL_H_INCLUDED
