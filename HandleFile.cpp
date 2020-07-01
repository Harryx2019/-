#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int BUS_NUM;
extern int STATION_NUM;
extern int ROUTE_NUM;

extern char **BUSES;
extern char **STATIONS;
extern int *ROUTES;

void ReadDataFromFile()
{
    //从文件中读取数据
    //stations.txt文件数据格式"站点编号 站点名称" 例如："59 武汉科技大学"
    //buses.txt文件 数据格式"公交编号 公交名称 起点站名 终点站名" 例如："0 1路上行 方庄道口 张孙村"
    //routes.txt文件 数据格式"公交编号,起始站点编号,目标站点编号,站点之间距离" 例如："0,2,3,300"

    //确定公交数、站点数、路段数
    FILE *fp;
    char buf[50];
    fp=fopen("buses.txt","r");
    while(fgets(buf,sizeof(buf),fp)!=NULL)
        BUS_NUM++;
    fclose(fp);

    fp=fopen("stations.txt","r");
    while(fgets(buf,sizeof(buf),fp)!=NULL)
        STATION_NUM++;
    fclose(fp);

    fp=fopen("routes.txt","r");
    while(fgets(buf,sizeof(buf),fp)!=NULL)
        ROUTE_NUM++;
    fclose(fp);

    int i,j;
    //读取站点信息
    STATIONS=(char**)malloc(sizeof(char*)*STATION_NUM);
    fp=fopen("stations.txt","r");
    for(i=0; i<STATION_NUM; i++)
    {
        fscanf(fp,"%d %s",&j,buf);
        STATIONS[i]=(char*)malloc(sizeof(char)*20);
        strcpy(STATIONS[i],buf);
    }
    fclose(fp);

    //读取公交信息
    int k;
    char buf2[20],buf3[20];
    BUSES=(char**)malloc(sizeof(char*)*(BUS_NUM*3));
    fp=fopen("buses.txt","r");
    for(i=0,j=0; i<BUS_NUM; i++,j+=3)
    {
        fscanf(fp,"%d %s %s %s",&k,buf,buf2,buf3);
        BUSES[j]=(char*)malloc(sizeof(char)*20);
        BUSES[j+1]=(char*)malloc(sizeof(char)*20);
        BUSES[j+2]=(char*)malloc(sizeof(char)*20);
        strcpy(BUSES[j],buf);
        strcpy(BUSES[j+1],buf2);
        strcpy(BUSES[j+2],buf3);
    }
    fclose(fp);

    //读取路段信息
    int bus,start,end,distance;
    ROUTES=(int*)malloc(sizeof(int)*(ROUTE_NUM*4));
    fp=fopen("routes.txt","r");
    for(i=0,j=0; i<ROUTE_NUM; i++,j+=4)
    {
        fscanf(fp,"%d,%d,%d,%d",&bus,&start,&end,&distance);
        ROUTES[j]=bus;
        ROUTES[j+1]=start;
        ROUTES[j+2]=end;
        ROUTES[j+3]=distance;
    }
}

void WriteDataToFile()
{
    FILE *fpw;
    //将站点数组写入文件
    int i;
    fpw=fopen("stations.txt","w");
    for(i=0; i<STATION_NUM-1; i++)
        fprintf(fpw,"%d %s\n",i,STATIONS[i]);
    fprintf(fpw,"%d %s",i,STATIONS[i]);
    fclose(fpw);

    //将公交数组写入文件
    int j;
    fpw=fopen("buses.txt","w");
    for(i=0,j=0; i<BUS_NUM-1; i++,j+=3)
        fprintf(fpw,"%d %s %s %s\n",i,BUSES[j],BUSES[j+1],BUSES[j+2]);
    fprintf(fpw,"%d %s %s %s",i,BUSES[j],BUSES[j+1],BUSES[j+2]);
    fclose(fpw);

    //将路段数组写入文件
    fpw=fopen("routes.txt","w");
    for(i=0,j=0;i<ROUTE_NUM-1;i++,j+=4)
        fprintf(fpw,"%d,%d,%d,%d\n",ROUTES[j],ROUTES[j+1],ROUTES[j+2],ROUTES[j+3]);
    fprintf(fpw,"%d,%d,%d,%d",ROUTES[j],ROUTES[j+1],ROUTES[j+2],ROUTES[j+3]);
    fclose(fpw);

    printf("\n--------------------------------------\n");
    printf("数据已成功保存！是否重启公交管理系统！\n");
    printf("      是（1）   否（0）\n");
    scanf("%d",&i);
    if(i==1)
        exit(0);
}
