// Kovalev_demo.cpp: главный файл проекта.

#include "stdafx.h"		//подключение библиотек
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <clocale>

using namespace std;
using namespace System;

#define ENTER 13	//обозначение кнопок
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 71
#define END 79

const char quest[7][49]={
	"У какого артиста больше всего слушателей?       ",
	"Какой артист старше всех?                       ",
	"Список артистов у которых слушателей более X.   ",
	"Алфавитный список всех артистов.                ",
	"Проверка городов на совп-ние кол-ва слушателей. ",
	"Диаграмма соотношения всех слушателей артистов. ",
	"Выход.                                          "
};		//задание пунктов меню
const char Blank[]="                                                  ";	//пустая строка

struct z {		
	char name[20];	//псевдоним артиста
	char city[15];	//название города
	long listeners;	//количество слушателей
	char date[11];	//дата рождения
	long points;	//количество очков
	long position;	//место в зачете
};	//шаблон структуры для исходных данных

struct alphalist {
	char nikname[20];	//псевданим артиста в структуре алфавитного списка
	long pos;	//позиция в структуре алфавитного списка
	long listeners;	//к-во слушателей в структуре алфавитного списка
	struct alphalist* next;	//ссылка на след. элемент списка
	struct alphalist* prev;	//ссылка на пред. элемент списка
};		//шаблон структуры для двустороннего списка

struct teamlist {
	char name[15];	//название команды
	long points;	//к-во очков команды
	struct teamlist* next;	//ссылка на след. команду
	struct teamlist* prev;	//ссылка на пред. команду
};	//шаблон структур для списка команд

int menu(int);	//указание шаблонов используемых функций
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
{	//файл открывается локально, данные из него
	//записываются в выделенную память
	int i, n;
	int DA;		//количество строк в файле с исходными данными
	FILE *in;
	struct z *artists;
	setlocale(LC_CTYPE, "RUSSIAN");	//поддержка русского языка
	Console::CursorVisible::set(false);	//отключение видимости курсора
	Console::BackgroundColor=ConsoleColor::Black;	
	Console::BufferHeight=Console::WindowHeight;
	Console::BufferWidth=Console::WindowWidth;

	if((in=fopen("artists.txt", "r"))==NULL)//попытка открыть файл
		{	
		printf("\nФайл не открыт!");	//вывод ошибки при
		getch(); exit(1);	//невзможности открыть файл
		}

	fscanf(in, "%d", &DA);	//считывание числа строк в исходном файле
	artists=(struct z*)malloc(DA*sizeof(struct z));	//выделение памяти
	Console::Clear();
	for(i=0;i<DA;i++)	//чтение из файла
		fscanf(in, "%s%s%ld%s%ld", artists[i].name, artists[i].city,
		&artists[i].listeners, artists[i].date, &artists[i].points);

	struct alphalist *glist = NULL, *endlist = NULL;
	for(i=0;i<DA;i++)	//заполнение списка
		insertion(artists,artists[i].name,artists[i].listeners, &glist, &endlist, DA);

	struct teamlist *plist = NULL, *endplist = NULL;
	for(i=0;i<DA;i++)	
		teaminsertion(artists,artists[i].points,artists[i].city, &plist, &endplist, DA);	//вставка городов в список

	printf("=============================================================================\n");
	printf("Артист               Город           Слушатели      Дата рождения   Номер    \n");
	printf("=============================================================================\n");
	for(i=0;i<DA;i++)	//вывод данных на экран
		printf("\n%-20s %-15s %7ld %17s %8ld ", artists[i].name, artists[i].city,
		artists[i].listeners, artists[i].date, artists[i].points);

	getch();

	while(1)
	{	//в цикле создается область меню
		Console::ForegroundColor=ConsoleColor::DarkCyan;
		Console::BackgroundColor=ConsoleColor::Black;
		Console::Clear();
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::White;
		Console::CursorLeft=10;	//точка начала меню
		Console::CursorTop=4;
		printf(Blank);

		Console::CursorLeft = 31;
		Console::CursorTop = 3;
		printf( " ВОПРОСЫ " );

		for(i=0;i<7;i++)	//печать вопросов
			{	
			Console::CursorLeft=10;
			Console::CursorTop=i+5;
			printf(" %s ", quest[i]);
			}
		Console::CursorLeft=10;	//точка конца меню
		Console::CursorTop=12;
		printf(Blank);

		n=menu(7);	//выбор вопроса в меню
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
	}	//конец цикла while(1)
    return 0;
}//конец main()

int menu(int n)
	{
		int y1=0,y2=n-1;
		char c=1;
		while (c!=ESC)	//закрытие программы при нажатии ESC
			{
				switch(c) 
					{
					case DOWN: y2=y1; y1++; break;	//программирование
						//кномки DOWN для перемещения вниз по меню
					case UP: y2=y1; y1--; break;	//программирование
						//кнопки UP для перемещения вверх по меню
					case ENTER: return y1+1;	//программирование
						//кнопки ENTER для выбора вопроса
					case HOME: y2=y1;y1=0; break;	//программирование
						//кнопки HOME, выбирающей первый вопрос
					case END: y2=y1;y1=n-1; break;	//программирование
						//кнопки END, выбирающей последний вопрос
					}
			if(y1>n-1)	//если выделен последний вопрос при нажатии DOWN
				{
				y2=n-1;
				y1=0;			
				}
			if(y1<0)	//если выделен первый вопрос при нажатии UP
				{
				y2=0;
				y1=n-1;
				}
		Console::ForegroundColor=ConsoleColor::Black;	//текст выделенного
		//вопроса окрашивается в черный
		Console::BackgroundColor=ConsoleColor::Cyan;	//рамка выделения
		//вопроса окрашивается в красный
		Console::CursorLeft=10;
		Console::CursorTop=y1+5;
		printf(" %s ",quest[y1]);
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::White;
		Console::CursorLeft=10;
		Console::CursorTop=y2+5;
		printf(" %s ",quest[y2]);
		c=getch();
		}	//конец while(c!=ESC)
	exit(0);	//закрытие программы
}
//Формирование строки с ответом для
//функции поиска артиста с наибольшим возрастом
void texttodate(char *a, char *b)
	{
		char a0[3], mth[12][9]=
			{
			"января", "февраля", "марта", "апреля", "мая", "июня",
			"июля", "августа", "сентября", "октября", "ноября", "декабря"
			};
		strncpy(a,b,2);	//"приклеивание" дня
		strcat(a," ");
		strncpy(a0, b+3,2);	//"прикеивание" месяца
		a0[2]=0;
		strcat(a, mth[atoi(a0)-1]);
		strcat(a," ");
		strncat(a,b+6,4);	//"приклеивание" года

		return;
	}
//У какого артиста больше слушателей? (функция
//поиска максимума)
void max(struct z* artists, int DA)
	{
		int i=0; struct z max;
		max.listeners=artists[0].listeners;	//в переменную структуры
		//записываем нулевой элемент массива структур
		for(i=1;i<DA;i++)
				if(artists[i].listeners>max.listeners)	//условие поиска максимума
					{
						strcpy(max.name, artists[i].name);
						max.listeners=artists[i].listeners;
					}
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::Cyan;
		Console::CursorLeft=10;	//область начала вывода ответа
		Console::CursorTop=15;
		printf("Наибольшее количество слушателей(%ld) у артиста", max.listeners);
		//вывод ответа
		Console::CursorLeft=10;	//область вывода имени артиста
		Console::CursorTop=16;
		printf("%s", max.name);	//вывод псевдонима артиста
		getch();
	}

//Какой артист старше всех? (функция
//поиска максимума)
void oldest(struct z* artists, int DA)
	{
		int i;
		long da1, da2;
		char a[17], d1[3],d2[3],m1[3],m2[3],y1[5],y2[5];
		struct z* oldest=artists;
		//Будем считать первого артиста в списке наиболее старым
		strcpy(y1, artists[0].date+6);	//запись в переменную
		//года рождения
		strncpy(m1, artists[0].date+3,2);	//запись в переменную
		//месяца рождения
		strncpy(d1, artists[0].date,2);	//запись в переменную
		//дня рождения
		d1[2]=0;  m1[2]=0;  y1[4]=0;
		da1=atoi(d1)+atoi(m1)*100+atoi(y1)*10000;
		//записываем дату как число
		oldest=&artists[0];
		for(i=1;i<DA;i++)
		{
			strcpy(y2, artists[i].date+6);	//запись в переменную
		//года рождения
			strncpy(m2, artists[i].date+3,2);	//запись в переменную
		//месяца рождения
	
			strncpy(d2, artists[i].date,2);	//запись в переменную
		//дня рождения
			d2[2]=0; m2[2]=0; y2[4]=0;
			da2=atoi(d2)+atoi(m2)*100+atoi(y2)*10000;
			//записываем дату как число
			if(da1>da2)	//сравниваем две даты
			{
			da1=da2;
			oldest=&artists[i];
			}
		}
		texttodate(a,oldest->date);
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::Cyan;
		Console::CursorLeft=10;	//область начала вывода ответа
		Console::CursorTop=15;
		printf("Самый взрослый артист %s", oldest->name);
		Console::CursorLeft=10;
		Console::CursorTop=16;
		printf("родился %s г.",a);
		getch();
	}

void greaterthen(struct z* artists, int DA) 
	{
		int i, k=0, x;
		struct z* greaterthen;
		greaterthen=(struct z*)malloc(DA*sizeof(struct z));
		//выделение памяти под структуру
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::Cyan;
		Console::Clear();
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::Cyan;
		Console::CursorLeft=10;	//область вывода текста с просьбой ввода
		Console::CursorTop=1;
		printf("Введите кол-во слушателей: ");
		scanf("%d", &x);	//считывание значения для сравнения с клавиатуры
		for(i=0;i<DA;i++)	//цикл для заполнения структуры элементами,
			//подходящими под условие
			if(artists[i].listeners>x)
				{
					strcpy(greaterthen[k].name,artists[i].name);
					greaterthen[k].listeners=artists[i].listeners;
					k++;
				}

		Console::CursorLeft=10;	//область вывода ответа
		Console::CursorTop=2;
		printf("Артист               Слушателей       ");
		for(i=0;i<k;i++)	//вывод ответа
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
printf("\n Алфавитный список артистов");
printf("\n ========================== \n");
for(nextartist=*glist;nextartist!=0;nextartist=nextartist->next)
	//цикл для вывода списка от А до Я
	printf("\n %-20s ", nextartist->nikname);
Console::CursorLeft=30;
Console::CursorTop=1;
printf(" Обратный алфавитный список артистов");
Console::CursorLeft=30;
Console::CursorTop=2;
printf(" =============================");
for(nextartist=*endlist;nextartist!=0;nextartist=nextartist->prev)
	//цикл для вывода списка от Я до А
	{
		Console::CursorLeft=30;	//область для строки списка
		Console::CursorTop=k;
		printf(" %-20s", nextartist->nikname);
		k++;
}
getch();
}

//формирование алфавитного списка артистов
void insertion(struct z* artists, char* nikname, long listeners, struct alphalist** glist, struct alphalist** endlist, int DA)
{
	int i;
	struct alphalist *newartist, *nextartist, *prevartist=0;
	for(nextartist=*glist; nextartist!=0 && strcmp(nextartist->nikname,nikname)<0; prevartist=nextartist, nextartist=nextartist->next);
	//поиск места для вставки элемента в список по афвавиту
	if(nextartist && strcmp(nextartist->nikname,nikname)==0) return;
	newartist=(struct alphalist*)malloc(sizeof(struct alphalist));
	strcpy(newartist->nikname,nikname);	//присваиваем полю НН в списке НН артиста
	for(i=0;i<DA;i++)
		if(strcmp(artists[i].name,nikname)==0)	//если НН переданного в функцию
			//пилота совпадает с НН артиста в начальной структуре
			{
				newartist->pos=artists[i].position;	//присваиваем полю с
				//местом артиста в списке значение места найденного в цкиле пилота
				newartist->listeners=artists[i].listeners;	//присваиваем полю к-во
				//слушателей артиста в списке значение к-ва слушателей найденного
				//в цикле артиста
			}
	newartist->next=nextartist;	//ссылка на следующего артиста
	newartist->prev=prevartist;	//ссылка на предыдущего артиста
	if(!nextartist)	//если данный артист последний по списку
			*endlist=newartist;	//указатель на конец списка указывает
	//на элемент, с которым функция работает в данный момент
	else	nextartist->prev=newartist; //ссылка на предыдущего артиста
	//у следующего артиста указывает на артиста, с которым функция
	//работает в данный момент
	if(!prevartist)	//если в списке нет предыдущего артиста
		*glist=newartist;	//указатель на начало списка указывает
	//на элемент, с которым функция работает в данный момент
	else prevartist->next=newartist;	//ссылка на следующего артиста
	//у предыдущего артиста указывает на артиста, с которым функция
	//работает в данный момент
	return;
}

//формирование списка городов 
void samepoints(struct z* artists, struct teamlist** plist)
{	
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Cyan;
	Console::Clear();
	Console::CursorLeft=5;	//область начала вывода списка
	Console::CursorTop=3;
	printf("Города, с одинаковым кол-вом слушателей:\n");
	search(*plist);	//поиск городов с одинаковыми слушателями
getch();
}

//функция, которая добавляет город в список городов
void teaminsertion(struct z* artists, long points, char* name, struct teamlist** plist, struct teamlist** endplist, int DA)
{
	int i;
	struct teamlist *newteam, *nextcity, *prevteam=0;
	for(nextcity=*plist; nextcity!=0 && strcmp(nextcity->name,name)<0; prevteam=nextcity, nextcity=nextcity->next);
	//поиск места для вставки команды в список
	if(nextcity && strcmp(nextcity->name,name)==0) return;
	newteam=(struct teamlist*)malloc(sizeof(struct teamlist));
	//выделения места в памяти под город
	strcpy(newteam->name,name);	//присваиваем полю название команды структуры
	//название города с которой работает функция
	newteam->next=nextcity;	//ссылка на следующий город
	newteam->prev=prevteam;	//ссылка на предыдущий город
	newteam->points=0;
	for(i=0;i<DA;i++)	//цикл для подсчета очков города со всех артистов
		//этой команды
		if(strcmp(artists[i].city,name)==0)
			newteam->points+=artists[i].points;
	if(!nextcity)	//если нет следующей команды
			*endplist=newteam;	//конец списка устанавливается на данную команду
	else	nextcity->prev=newteam; //указатель на предыдущую команду у следующей 
	//устанавливается на текущую команды
	if(!prevteam)	//если нет предыдущей команды
		*plist=newteam;	//начало списка устанавливается на текущую команду
	else prevteam->next=newteam;	//указатель на следующую команду
	//у предыдущей устанавливается на текущую команду
	return;
}

//диаграмма, показывающая процентное соотнощение
//кол-ва слушателей артистов
void diagram(struct z* artists, int DA, struct alphalist** glist)
{
	struct alphalist *nextartist;
	int length, ColNum, i;
	long overallstarts=0;	//общее количество слушателей
	char col1[20];
	char col2[20];
	System::ConsoleColor Color;
	Console::ForegroundColor=ConsoleColor::White;
	Console::BackgroundColor=ConsoleColor::Black;
	Console::Clear();
	for(i=0;i<DA;i++)	//подсчет общего количества слушателей
		overallstarts=overallstarts+artists[i].listeners;
	Color=ConsoleColor::Black;
	ColNum=0;
	for(nextartist=*glist,i=0;nextartist!=0;nextartist=nextartist->next,i++)		//цикл, который проходит от начала списка артистов до конца
		{
			sprintf(col1,"%s",nextartist->nikname);	//записываем в col1 НН артиста
			sprintf(col2,"%3.1f%%",(nextartist->listeners*100./overallstarts));
			//записываем в col2 процент количества слушателей относительно общего количества слушателей
			Console::ForegroundColor=ConsoleColor::White;
			Console::BackgroundColor=ConsoleColor::Black;
			Console::CursorLeft=5;	//область вывода НН и процента количества слушателей
			Console::CursorTop=i+1;
			printf(col1);	//вывод НН артиста на экран
			Console::CursorLeft=25;
			printf("%s", col2);	//вывод процента кол-ва слушателей на экран
			Console::BackgroundColor=++Color;	//выбор следующего цвета
			ColNum++;
			Console::CursorLeft=35;
			for(length=0;length<nextartist->listeners*100/overallstarts;length++)
				printf(" ");	//печать столбца диаграммы заданным цветом
			if(ColNum==14)	//выделяем на диаграмму 14 цветов
			{
				Color=ConsoleColor::Black;
				ColNum=0;
			}
		}
	getch();
	return;
}

//функция для поиска команд с
//одинаковым кол-вом очков
void search(struct teamlist* plist)
{ 
	int k=0, i, j, f=0,g=0, counter=0, *A;
	//k - количество команд в списке
	//A - массив с очками команд
	//counter - флаговая переменная, которая не равна 0, если есть
	//команды с одинаковым кол-вом очков
	struct teamlist *curteam, *pointer;
	//pointer - указатель на команду, с которой сравниваются очки
	//curteam - команда, чьи очки сравниваются
	curteam=plist;
	while(curteam)	//подсчет количества команд в списке
	{	
		k++;
		curteam=curteam->next;
	}
	A=(int*)malloc(k*sizeof(int));	//выделение памяти под массив
	for(i=0;i<k;i++)	//заполняем каждый элемент массива
		A[i]=-1;		//числом -1, чтобы далее не было 
	//случайных совпадений по к-ву очков
	i=0;	//обнуляем счетчик
	for(pointer=plist;pointer->next!=0;pointer=pointer->next)
		//цикл от начала списка до конца
		{	
			for(j=0;j<k;j++)				//если в массиве найдется число, равное
				if(A[j]==pointer->points)	//количеству очков команды, на которую
					f++;					//указывает pointer, переменная f изменяет значение
			if(f)							//если f не равна 0, значит, команды с таким к-вом
				{							//очков уже были, и можно пропустить итерацию
					f=0;					//внешнего цикла
					continue;
				}
			A[i]=pointer->points;	//записываем в массив к-во очков команды,
			//с которой сравниваются очки
			Console::CursorLeft=5;
			for (curteam=pointer->next; curteam!=0;curteam=curteam->next)
				//цикл от команды, следующей за pointer, до конца
				if (curteam->points==pointer->points)	//если очки текущей команды
					//совпадают с очками команды, на которую указывает pointer
				{
					g++;	//меняем значение флаговой переменной g
					printf("%s, ", curteam->name);
				}
			if(g)	//если флаговая переменная g не равна 0
				{
					printf("%s ", pointer->name);	//вывод названия команды, с которой
					//сравнивали другие команды
					printf("(%ld)\n", pointer->points);	//вывод кол-ва очков команд
					counter++;\
				}		
			g=0;
			i++;
		}
	if(!counter)	//если нет команд с одинаковым кол-вом очков
	{
		Console::CursorLeft=5;
		printf("Таковых нет");
	}
}