#include <stdio.h>
#include <stdlib.h>

#include "model.h"
#include "LoadMapData.h"
#include "ResetSystem.h"

void ResetSystem(BusMap &Map)
{
    FILE *fp,*fpw;
    char buf[100];
    //重置buses.txt
    fp=fopen("备份buses.txt","r");
    fpw=fopen("buses.txt","w");
    while(fgets(buf,sizeof(buf),fp)!=NULL)
        fputs(buf,fpw);
    fclose(fpw);
    fclose(fp);

    //重置stations.txt
    fp=fopen("备份stations.txt","r");
    fpw=fopen("stations.txt","w");
    while(fgets(buf,sizeof(buf),fp)!=NULL)
        fputs(buf,fpw);
    fclose(fpw);
    fclose(fp);

    //重置routes.txt
    fp=fopen("备份routes.txt","r");
    fpw=fopen("routes.txt","w");
    while(fgets(buf,sizeof(buf),fp)!=NULL)
        fputs(buf,fpw);
    fclose(fpw);
    fclose(fp);

    LoadMapData(Map);

    int i;
    printf("\n--------------------------------------\n");
    printf("数据已成功重置！是否重启公交管理系统！\n");
    printf("      是（1）   否（0）\n");
    scanf("%d",&i);
    if(i==1)
        exit(0);
}
