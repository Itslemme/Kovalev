// Kovalev_demo.cpp: ������� ���� �������.

#include "stdafx.h"		//����������� ���������
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <clocale>

using namespace std;
using namespace System;

#define ENTER 13	//����������� ������
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 71
#define END 79

const char quest[7][49]={
	"� ������ ������� ������ ����� ����������?       ",
	"����� ������ ������ ����?                       ",
	"������ �������� � ������� ���������� ����� X.   ",
	"���������� ������ ���� ��������.                ",
	"�������� ������� �� ����-��� ���-�� ����������. ",
	"��������� ����������� ���� ���������� ��������. ",
	"�����.                                          "
};		//������� ������� ����
const char Blank[]="                                                  ";	//������ ������

struct z {		
	char name[20];	//��������� �������
	char city[15];	//�������� ������
	long listeners;	//���������� ����������
	char date[11];	//���� ��������
	long points;	//���������� �����
	long position;	//����� � ������
};	//������ ��������� ��� �������� ������

struct alphalist {
	char nikname[20];	//��������� ������� � ��������� ����������� ������
	long pos;	//������� � ��������� ����������� ������
	long listeners;	//�-�� ���������� � ��������� ����������� ������
	struct alphalist* next;	//������ �� ����. ������� ������
	struct alphalist* prev;	//������ �� ����. ������� ������
};		//������ ��������� ��� ������������� ������

struct teamlist {
	char name[15];	//�������� �������
	long points;	//�-�� ����� �������
	struct teamlist* next;	//������ �� ����. �������
	struct teamlist* prev;	//������ �� ����. �������
};	//������ �������� ��� ������ ������

int menu(int);	//�������� �������� ������������ �������
void texttodate(char*, char*);
void max(struct z*, int);
void oldest(struct z*, int);
void greaterthen(struct z*, int);
void alphabet(struct z*, struct alphalist** , struct alphalist** );
void insertion(struct z*, char*, long, struct alphalist** , struct alphalist** , int);
void samepoints(struct z*, struct teamlist**);
void teaminsertion(struct z*, long, char*, struct teamlist**, struct teamlist**, int);
void diagram(struct z*, int, struct alphalist**);
void search(struct teamlist*);

int main(array<System::String ^> ^args)
{	//���� ����������� ��������, ������ �� ����
	//������������ � ���������� ������
	int i, n;
	int DA;		//���������� ����� � ����� � ��������� �������
	FILE *in;
	struct z *artists;
	setlocale(LC_CTYPE, "RUSSIAN");	//��������� �������� �����
	Console::CursorVisible::set(false);	//���������� ��������� �������
	Console::BackgroundColor=ConsoleColor::Black;	
	Console::BufferHeight=Console::WindowHeight;
	Console::BufferWidth=Console::WindowWidth;

	if((in=fopen("artists.txt", "r"))==NULL)//������� ������� ����
		{	
		printf("\n���� �� ������!");	//����� ������ ���
		getch(); exit(1);	//������������ ������� ����
		}

	fscanf(in, "%d", &DA);	//���������� ����� ����� � �������� �����
	artists=(struct z*)malloc(DA*sizeof(struct z));	//��������� ������
	Console::Clear();
	for(i=0;i<DA;i++)	//������ �� �����
		fscanf(in, "%s%s%ld%s%ld", artists[i].name, artists[i].city,
		&artists[i].listeners, artists[i].date, &artists[i].points);

	struct alphalist *glist = NULL, *endlist = NULL;
	for(i=0;i<DA;i++)	//���������� ������
		insertion(artists,artists[i].name,artists[i].listeners, &glist, &endlist, DA);

	struct teamlist *plist = NULL, *endplist = NULL;
	for(i=0;i<DA;i++)	
		teaminsertion(artists,artists[i].points,artists[i].city, &plist, &endplist, DA);	//������� ������� � ������

	printf("=============================================================================\n");
	printf("������               �����           ���������      ���� ��������   �����    \n");
	printf("=============================================================================\n");
	for(i=0;i<DA;i++)	//����� ������ �� �����
		printf("\n%-20s %-15s %7ld %17s %8ld ", artists[i].name, artists[i].city,
		artists[i].listeners, artists[i].date, artists[i].points);

	getch();

	while(1)
	{	//� ����� ��������� ������� ����
		Console::ForegroundColor=ConsoleColor::DarkCyan;
		Console::BackgroundColor=ConsoleColor::Black;
		Console::Clear();
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::White;
		Console::CursorLeft=10;	//����� ������ ����
		Console::CursorTop=4;
		printf(Blank);

		Console::CursorLeft = 31;
		Console::CursorTop = 3;
		printf( " ������� " );

		for(i=0;i<7;i++)	//������ ��������
			{	
			Console::CursorLeft=10;
			Console::CursorTop=i+5;
			printf(" %s ", quest[i]);
			}
		Console::CursorLeft=10;	//����� ����� ����
		Console::CursorTop=12;
		printf(Blank);

		n=menu(7);	//����� ������� � ����
		switch(n)
		{
			case 1:max(artists, DA);break;
			case 2:oldest(artists, DA);break;
			case 3:greaterthen(artists, DA);break;
			case 4:alphabet(artists, &glist, &endlist);break;
			case 5:samepoints(artists, &plist);break;
			case 6:diagram(artists, DA, &glist);break;
			case 7:exit(0);
		}
	}	//����� ����� while(1)
    return 0;
}//����� main()

int menu(int n)
	{
		int y1=0,y2=n-1;
		char c=1;
		while (c!=ESC)	//�������� ��������� ��� ������� ESC
			{
				switch(c) 
					{
					case DOWN: y2=y1; y1++; break;	//����������������
						//������ DOWN ��� ����������� ���� �� ����
					case UP: y2=y1; y1--; break;	//����������������
						//������ UP ��� ����������� ����� �� ����
					case ENTER: return y1+1;	//����������������
						//������ ENTER ��� ������ �������
					case HOME: y2=y1;y1=0; break;	//����������������
						//������ HOME, ���������� ������ ������
					case END: y2=y1;y1=n-1; break;	//����������������
						//������ END, ���������� ��������� ������
					}
			if(y1>n-1)	//���� ������� ��������� ������ ��� ������� DOWN
				{
				y2=n-1;
				y1=0;			
				}
			if(y1<0)	//���� ������� ������ ������ ��� ������� UP
				{
				y2=0;
				y1=n-1;
				}
		Console::ForegroundColor=ConsoleColor::Black;	//����� �����������
		//������� ������������ � ������
		Console::BackgroundColor=ConsoleColor::Cyan;	//����� ���������
		//������� ������������ � �������
		Console::CursorLeft=10;
		Console::CursorTop=y1+5;
		printf(" %s ",quest[y1]);
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::White;
		Console::CursorLeft=10;
		Console::CursorTop=y2+5;
		printf(" %s ",quest[y2]);
		c=getch();
		}	//����� while(c!=ESC)
	exit(0);	//�������� ���������
}
//������������ ������ � ������� ���
//������� ������ ������� � ���������� ���������
void texttodate(char *a, char *b)
	{
		char a0[3], mth[12][9]=
			{
			"������", "�������", "�����", "������", "���", "����",
			"����", "�������", "��������", "�������", "������", "�������"
			};
		strncpy(a,b,2);	//"������������" ���
		strcat(a," ");
		strncpy(a0, b+3,2);	//"�����������" ������
		a0[2]=0;
		strcat(a, mth[atoi(a0)-1]);
		strcat(a," ");
		strncat(a,b+6,4);	//"������������" ����

		return;
	}
//� ������ ������� ������ ����������? (�������
//������ ���������)
void max(struct z* artists, int DA)
	{
		int i=0; struct z max;
		max.listeners=artists[0].listeners;	//� ���������� ���������
		//���������� ������� ������� ������� ��������
		for(i=1;i<DA;i++)
				if(artists[i].listeners>max.listeners)	//������� ������ ���������
					{
						strcpy(max.name, artists[i].name);
						max.listeners=artists[i].listeners;
					}
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::Cyan;
		Console::CursorLeft=10;	//������� ������ ������ ������
		Console::CursorTop=15;
		printf("���������� ���������� ����������(%ld) � �������", max.listeners);
		//����� ������
		Console::CursorLeft=10;	//������� ������ ����� �������
		Console::CursorTop=16;
		printf("%s", max.name);	//����� ���������� �������
		getch();
	}

//����� ������ ������ ����? (�������
//������ ���������)
void oldest(struct z* artists, int DA)
	{
		int i;
		long da1, da2;
		char a[17], d1[3],d2[3],m1[3],m2[3],y1[5],y2[5];
		struct z* oldest=artists;
		//����� ������� ������� ������� � ������ �������� ������
		strcpy(y1, artists[0].date+6);	//������ � ����������
		//���� ��������
		strncpy(m1, artists[0].date+3,2);	//������ � ����������
		//������ ��������
		strncpy(d1, artists[0].date,2);	//������ � ����������
		//��� ��������
		d1[2]=0;  m1[2]=0;  y1[4]=0;
		da1=atoi(d1)+atoi(m1)*100+atoi(y1)*10000;
		//���������� ���� ��� �����
		oldest=&artists[0];
		for(i=1;i<DA;i++)
		{
			strcpy(y2, artists[i].date+6);	//������ � ����������
		//���� ��������
			strncpy(m2, artists[i].date+3,2);	//������ � ����������
		//������ ��������
	
			strncpy(d2, artists[i].date,2);	//������ � ����������
		//��� ��������
			d2[2]=0; m2[2]=0; y2[4]=0;
			da2=atoi(d2)+atoi(m2)*100+atoi(y2)*10000;
			//���������� ���� ��� �����
			if(da1>da2)	//���������� ��� ����
			{
			da1=da2;
			oldest=&artists[i];
			}
		}
		texttodate(a,oldest->date);
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::Cyan;
		Console::CursorLeft=10;	//������� ������ ������ ������
		Console::CursorTop=15;
		printf("����� �������� ������ %s", oldest->name);
		Console::CursorLeft=10;
		Console::CursorTop=16;
		printf("������� %s �.",a);
		getch();
	}

void greaterthen(struct z* artists, int DA) 
	{
		int i, k=0, x;
		struct z* greaterthen;
		greaterthen=(struct z*)malloc(DA*sizeof(struct z));
		//��������� ������ ��� ���������
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::Cyan;
		Console::Clear();
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::Cyan;
		Console::CursorLeft=10;	//������� ������ ������ � �������� �����
		Console::CursorTop=1;
		printf("������� ���-�� ����������: ");
		scanf("%d", &x);	//���������� �������� ��� ��������� � ����������
		for(i=0;i<DA;i++)	//���� ��� ���������� ��������� ����������,
			//����������� ��� �������
			if(artists[i].listeners>x)
				{
					strcpy(greaterthen[k].name,artists[i].name);
					greaterthen[k].listeners=artists[i].listeners;
					k++;
				}

		Console::CursorLeft=10;	//������� ������ ������
		Console::CursorTop=2;
		printf("������               ����������       ");
		for(i=0;i<k;i++)	//����� ������
		{
			printf("\n");
			Console::CursorLeft=10;	
			printf("%-20s %7ld", greaterthen[i].name, greaterthen[i].listeners);	
		}
		getch();
	}

void alphabet(struct z* artists, struct alphalist** glist, struct alphalist** endlist)
{
	int k=4;
	struct alphalist* nextartist;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Cyan;
	Console::Clear();
printf("\n ���������� ������ ��������");
printf("\n ========================== \n");
for(nextartist=*glist;nextartist!=0;nextartist=nextartist->next)
	//���� ��� ������ ������ �� � �� �
	printf("\n %-20s ", nextartist->nikname);
Console::CursorLeft=30;
Console::CursorTop=1;
printf(" �������� ���������� ������ ��������");
Console::CursorLeft=30;
Console::CursorTop=2;
printf(" =============================");
for(nextartist=*endlist;nextartist!=0;nextartist=nextartist->prev)
	//���� ��� ������ ������ �� � �� �
	{
		Console::CursorLeft=30;	//������� ��� ������ ������
		Console::CursorTop=k;
		printf(" %-20s", nextartist->nikname);
		k++;
}
getch();
}

//������������ ����������� ������ ��������
void insertion(struct z* artists, char* nikname, long listeners, struct alphalist** glist, struct alphalist** endlist, int DA)
{
	int i;
	struct alphalist *newartist, *nextartist, *prevartist=0;
	for(nextartist=*glist; nextartist!=0 && strcmp(nextartist->nikname,nikname)<0; prevartist=nextartist, nextartist=nextartist->next);
	//����� ����� ��� ������� �������� � ������ �� ��������
	if(nextartist && strcmp(nextartist->nikname,nikname)==0) return;
	newartist=(struct alphalist*)malloc(sizeof(struct alphalist));
	strcpy(newartist->nikname,nikname);	//����������� ���� �� � ������ �� �������
	for(i=0;i<DA;i++)
		if(strcmp(artists[i].name,nikname)==0)	//���� �� ����������� � �������
			//������ ��������� � �� ������� � ��������� ���������
			{
				newartist->pos=artists[i].position;	//����������� ���� �
				//������ ������� � ������ �������� ����� ���������� � ����� ������
				newartist->listeners=artists[i].listeners;	//����������� ���� �-��
				//���������� ������� � ������ �������� �-�� ���������� ����������
				//� ����� �������
			}
	newartist->next=nextartist;	//������ �� ���������� �������
	newartist->prev=prevartist;	//������ �� ����������� �������
	if(!nextartist)	//���� ������ ������ ��������� �� ������
			*endlist=newartist;	//��������� �� ����� ������ ���������
	//�� �������, � ������� ������� �������� � ������ ������
	else	nextartist->prev=newartist; //������ �� ����������� �������
	//� ���������� ������� ��������� �� �������, � ������� �������
	//�������� � ������ ������
	if(!prevartist)	//���� � ������ ��� ����������� �������
		*glist=newartist;	//��������� �� ������ ������ ���������
	//�� �������, � ������� ������� �������� � ������ ������
	else prevartist->next=newartist;	//������ �� ���������� �������
	//� ����������� ������� ��������� �� �������, � ������� �������
	//�������� � ������ ������
	return;
}

//������������ ������ ������� 
void samepoints(struct z* artists, struct teamlist** plist)
{	
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Cyan;
	Console::Clear();
	Console::CursorLeft=5;	//������� ������ ������ ������
	Console::CursorTop=3;
	printf("������, � ���������� ���-��� ����������:\n");
	search(*plist);	//����� ������� � ����������� �����������
getch();
}

//�������, ������� ��������� ����� � ������ �������
void teaminsertion(struct z* artists, long points, char* name, struct teamlist** plist, struct teamlist** endplist, int DA)
{
	int i;
	struct teamlist *newteam, *nextcity, *prevteam=0;
	for(nextcity=*plist; nextcity!=0 && strcmp(nextcity->name,name)<0; prevteam=nextcity, nextcity=nextcity->next);
	//����� ����� ��� ������� ������� � ������
	if(nextcity && strcmp(nextcity->name,name)==0) return;
	newteam=(struct teamlist*)malloc(sizeof(struct teamlist));
	//��������� ����� � ������ ��� �����
	strcpy(newteam->name,name);	//����������� ���� �������� ������� ���������
	//�������� ������ � ������� �������� �������
	newteam->next=nextcity;	//������ �� ��������� �����
	newteam->prev=prevteam;	//������ �� ���������� �����
	newteam->points=0;
	for(i=0;i<DA;i++)	//���� ��� �������� ����� ������ �� ���� ��������
		//���� �������
		if(strcmp(artists[i].city,name)==0)
			newteam->points+=artists[i].points;
	if(!nextcity)	//���� ��� ��������� �������
			*endplist=newteam;	//����� ������ ��������������� �� ������ �������
	else	nextcity->prev=newteam; //��������� �� ���������� ������� � ��������� 
	//��������������� �� ������� �������
	if(!prevteam)	//���� ��� ���������� �������
		*plist=newteam;	//������ ������ ��������������� �� ������� �������
	else prevteam->next=newteam;	//��������� �� ��������� �������
	//� ���������� ��������������� �� ������� �������
	return;
}

//���������, ������������ ���������� �����������
//���-�� ���������� ��������
void diagram(struct z* artists, int DA, struct alphalist** glist)
{
	struct alphalist *nextartist;
	int length, ColNum, i;
	long overallstarts=0;	//����� ���������� ����������
	char col1[20];
	char col2[20];
	System::ConsoleColor Color;
	Console::ForegroundColor=ConsoleColor::White;
	Console::BackgroundColor=ConsoleColor::Black;
	Console::Clear();
	for(i=0;i<DA;i++)	//������� ������ ���������� ����������
		overallstarts=overallstarts+artists[i].listeners;
	Color=ConsoleColor::Black;
	ColNum=0;
	for(nextartist=*glist,i=0;nextartist!=0;nextartist=nextartist->next,i++)		//����, ������� �������� �� ������ ������ �������� �� �����
		{
			sprintf(col1,"%s",nextartist->nikname);	//���������� � col1 �� �������
			sprintf(col2,"%3.1f%%",(nextartist->listeners*100./overallstarts));
			//���������� � col2 ������� ���������� ���������� ������������ ������ ���������� ����������
			Console::ForegroundColor=ConsoleColor::White;
			Console::BackgroundColor=ConsoleColor::Black;
			Console::CursorLeft=5;	//������� ������ �� � �������� ���������� ����������
			Console::CursorTop=i+1;
			printf(col1);	//����� �� ������� �� �����
			Console::CursorLeft=25;
			printf("%s", col2);	//����� �������� ���-�� ���������� �� �����
			Console::BackgroundColor=++Color;	//����� ���������� �����
			ColNum++;
			Console::CursorLeft=35;
			for(length=0;length<nextartist->listeners*100/overallstarts;length++)
				printf(" ");	//������ ������� ��������� �������� ������
			if(ColNum==14)	//�������� �� ��������� 14 ������
			{
				Color=ConsoleColor::Black;
				ColNum=0;
			}
		}
	getch();
	return;
}

//������� ��� ������ ������ �
//���������� ���-��� �����
void search(struct teamlist* plist)
{ 
	int k=0, i, j, f=0,g=0, counter=0, *A;
	//k - ���������� ������ � ������
	//A - ������ � ������ ������
	//counter - �������� ����������, ������� �� ����� 0, ���� ����
	//������� � ���������� ���-��� �����
	struct teamlist *curteam, *pointer;
	//pointer - ��������� �� �������, � ������� ������������ ����
	//curteam - �������, ��� ���� ������������
	curteam=plist;
	while(curteam)	//������� ���������� ������ � ������
	{	
		k++;
		curteam=curteam->next;
	}
	A=(int*)malloc(k*sizeof(int));	//��������� ������ ��� ������
	for(i=0;i<k;i++)	//��������� ������ ������� �������
		A[i]=-1;		//������ -1, ����� ����� �� ���� 
	//��������� ���������� �� �-�� �����
	i=0;	//�������� �������
	for(pointer=plist;pointer->next!=0;pointer=pointer->next)
		//���� �� ������ ������ �� �����
		{	
			for(j=0;j<k;j++)				//���� � ������� �������� �����, ������
				if(A[j]==pointer->points)	//���������� ����� �������, �� �������
					f++;					//��������� pointer, ���������� f �������� ��������
			if(f)							//���� f �� ����� 0, ������, ������� � ����� �-���
				{							//����� ��� ����, � ����� ���������� ��������
					f=0;					//�������� �����
					continue;
				}
			A[i]=pointer->points;	//���������� � ������ �-�� ����� �������,
			//� ������� ������������ ����
			Console::CursorLeft=5;
			for (curteam=pointer->next; curteam!=0;curteam=curteam->next)
				//���� �� �������, ��������� �� pointer, �� �����
				if (curteam->points==pointer->points)	//���� ���� ������� �������
					//��������� � ������ �������, �� ������� ��������� pointer
				{
					g++;	//������ �������� �������� ���������� g
					printf("%s, ", curteam->name);
				}
			if(g)	//���� �������� ���������� g �� ����� 0
				{
					printf("%s ", pointer->name);	//����� �������� �������, � �������
					//���������� ������ �������
					printf("(%ld)\n", pointer->points);	//����� ���-�� ����� ������
					counter++;\
				}		
			g=0;
			i++;
		}
	if(!counter)	//���� ��� ������ � ���������� ���-��� �����
	{
		Console::CursorLeft=5;
		printf("������� ���");
	}
}