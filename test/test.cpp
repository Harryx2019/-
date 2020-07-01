#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct Station
{
    int station_no;
    char station_name;
} Station;

typedef struct Bus
{
    int bus_no;
    char bus_name[20];
    int start;
    int end;
} Bus;

typedef struct Route
{
    int route_no;
    int stationf;
    int statione;
    int dis;
} Route;

int station_num=0;
int bus_num=0;
int route_num=0;

void read(Station * &S,Bus * &B,Route * &R)
{
    FILE *fp;
    int i;
    char str[50];

    fp=fopen("D:\\stations.txt","r");
    if(fp==NULL)
    {
        printf("文件打开失败\n");
        exit(-1);
    }
    else
    {
        while(fgets(str,sizeof(str),fp)!=NULL)
            station_num++;
    }
    fclose(fp);

    fp=fopen("D:\\buses.txt","r");
    if(fp==NULL)
    {
        printf("文件打开失败\n");
        exit(-1);
    }
    else
    {
        while(fgets(str,sizeof(str),fp)!=NULL)
            bus_num++;
    }
    fclose(fp);

    fp=fopen("D:\\routes.txt","r");
    if(fp==NULL)
    {
        printf("文件打开失败\n");
        exit(-1);
    }
    else
    {
        while(fgets(str,sizeof(str),fp)!=NULL)
            route_num++;
    }
    fclose(fp);


    fp=fopen("D:\\stations.txt","r");
    S=(Station*)malloc(sizeof(Station)*station_num);
    for(i=0; i<station_num; i++)
    {
        fscanf(fp,"%d %s\n",&S[i].station_no,S[i].station_name);
    }
    fclose(fp);

    fp=fopen("D:\\buses.txt","r");
    B=(Bus*)malloc(sizeof(Bus)*bus_num);
    for(i=0; i<bus_num; i++)
    {
        fscanf(fp,"%d %s %d %d\n",&B[i].bus_no,B[i].bus_name,&B[i].start,&B[i].end);
    }
    fclose(fp);

    fp=fopen("D:\\routes.txt","r");
    R=(Route*)malloc(sizeof(Route)*route_num);
    for(i=0; i<route_num; i++)
    {
        fscanf(fp,"%d %d %d %d\n",&R[i].route_no,&R[i].stationf,&R[i].statione,&R[i].dis);
    }
    fclose(fp);

}

int main()
{
    Station *station;
    Bus *bus;
    Route *route;
    int i;
    read(station,bus,route);

    for(i=0; i<station_num; i++)
    {
        printf("%d %s\n",station[i].station_no,station[i].station_name);
    }

    for(i=0; i<bus_num; i++)
    {
        printf("%d %s %d %d\n",bus[i].bus_no,bus[i].bus_name,bus[i].start,bus[i].end);
    }

    for(i=0; i<route_num; i++)
    {
        printf("%d %d %d %d\n",route[i].route_no,route[i].stationf,route[i].statione,route[i].dis);
    }

    return 0;
}
