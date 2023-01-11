#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <windows.h>

typedef struct mbr {//структура команды
	char name[8];//имя
	int ride[8];//массив заездов
	int result = 0;//сумма времени
	int place;//место
} mbr;

void put_table(int size, mbr* arr);//вывод отсортированной таблицы
void sort_bubble(int size, mbr* arr);//сортировка пузырьком
int count_result(int size, int j, mbr* arr); //вычисление контрольной суммы времени
int best_in_ride(int size, int i, mbr* arr); //лучшая команда в горнолыжном заезде
int read_from_file(int size, mbr* arr, char* fname); //чтение таблицы из файла

int main() {

	system("chcp 1251");

	int flag = 0;//флаг выхода из меню
	int ptr; // управление меню, скан чисел
	int fr, to;// от до
	int size = 0;//длина строки файла
	char fname[30];//имя файла
	mbr* membr;//массив команд
	char s[200];//строка файла
	FILE* inf;//file

	printf("Приветствие\nПрограмма просмотра и анализа статистики команд соревнований по лыжной эстафете\n\n");
	puts("Введите имя файла");
	scanf("%s", &fname);
	inf = fopen(fname, "r");
	if (inf == NULL) {
		printf("Файла не существует");
		return 0;
	}
	while (!feof(inf)) {
		fgets(s, 200, inf);
		size++;
	}
	fclose(inf); //число строк

	membr = (mbr*)malloc(size * sizeof(mbr));// массив команд

	if (read_from_file(size, membr, fname) == NULL) {
		printf("Ошибка в данных занесенных в файл");
		return 0;
	}
	for (int j = 0; j < size; j++) {
		printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", membr[j].name, membr[j].ride[0], membr[j].ride[1], membr[j].ride[2], membr[j].ride[3], membr[j].ride[4], membr[j].ride[5], membr[j].ride[6], membr[j].ride[7]);

	}

	sort_bubble(size, membr);
	printf("Табица была отсторирована\n\n");


	//цикл главного меню
	while (flag == 0) 
	{
		printf("Вывести таблицу - 1\nВывести команды с диапазоном времени - 2\nЗаписать таблицу в файл - 3\nЛучшая команда - 4\nИзменить данные таблицы - 5\nЗавершить - 0\n");
		scanf("%d", &ptr);
		switch (ptr) {
		case 1: {
			system("cls");
			put_table(size, membr);
			break;
		}
		case 2: {
			printf("Введите левое значение диапазона\n");
			scanf("%d", &fr);
			printf("Введите правое значение диапазона\n");
			scanf("%d", &to);
			for (int i = 0; i < size; i++) {
				if (membr[i].result >= fr && membr[i].result <= to)
					printf("\n%s %d", membr[i].name, membr[i].result);
			}
			printf("\n\n");
			break;
		}
		case 3: {
			FILE* sec;//новый файл для записи
			char fnamesec[60];//её имя
			puts("Введите имя файла\n");
			scanf("%s", &fnamesec);
			sec = fopen(fnamesec, "w");
			if (sec == NULL) {
				printf("ERROR\n");
				break;
			}
			for (int i = 0; i < size; i++) {
				fprintf(sec, "%s ", membr[i].name);
				for (int j = 0; j < 7; j++)
				{
					fprintf(sec, "%d ", membr[i].ride[j]);
				}
				fprintf(sec, "\n");
			}

			system("cls");
			printf("Успешно записано\n\n");
			if (sec != NULL) fclose(sec);
			break;
		}
		case 4: {
			int h;//номер заезда
			system("cls");
			put_table(size, membr);
			puts("Введите номер заезда\n");
			scanf("%d", &h);
			printf("Лучшей в %d заезде была %s\n\n", h, membr[best_in_ride(size, h - 1, membr)].name);
			break;
		}
		case 5: {
			int m = 0;//номер команды
			int n = 0;//номер заезда
			int n2;//новое значение времени
			printf("Введите номер команды и номер заезда\n");
			scanf("%d %d", &m, &n);
			printf("Введите новое значение\n");
			scanf("%d", &n2);
			membr[m - 1].ride[n - 1] = n2;
			membr[m - 1].result = count_result(size, m - 1, membr);
			sort_bubble(size, membr);
			put_table(size, membr);
			break;
		}
		case 0: { //выход
			flag = 1;
			break;
		}
		default: {
			system("cls");
			printf("Введен неизвестный символ\n");
			break;
		}
		}
	}
	free(membr);
	return 1;
}

void put_table(int size, mbr* arr) 
{
	int place = 1;//счетчик мест
	
	printf("Команда/Номер заезда\n");
	printf("N\t1\t2\t3\t4\t5\t6\t7\t8\tОчки\tМесто\t\n");

	arr[0].place = place;//таблица уже отсортирована по возрастанию
	for (int i = 0; i < size; i++) {
		printf("%s\t", arr[i].name);
		if (i != 0 && arr[i - 1].result < arr[i].result) {//проверка, делят ли 2 и более команд одно место
			arr[i].place = ++place;
		}
		else
			arr[i].place = place;

		for (int j = 0; j < 8; j++) {
			printf("%d\t", arr[i].ride[j]);
		}
		printf(" %d\t", arr[i].result);
		printf("%d\t\n", arr[i].place);
	}
	printf("\n");
}
void sort_bubble(int size, mbr* arr) {
	mbr d;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - 1; j++) {
			if (arr[j].result > arr[j + 1].result) {
				d = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = d;
			}
		}
	}
}

int count_result(int size, int j, mbr* arr) {
	int sum = 0;
	for (int i = 0; i < 8; i++) {
		sum += arr[j].ride[i];
	}
	return sum;
}

int best_in_ride(int size, int i, mbr* arr) {
	int min = arr[0].ride[i];//min time
	int mi = 0;//min ID
	for (int j = 1; j < size; j++) {
		if (arr[j].ride[i] < min) {
			min = arr[j].ride[i];
			mi = j;
		}
	}
	return mi;
}
int read_from_file(int size, mbr* arr, char* fname) {
	FILE* inf;
	int point;
	int i = 0;

	inf = fopen(fname, "r");
	if (inf == NULL) {
		printf("ERROR");
		return 0;
	}
	while (!feof(inf)) {
		fscanf(inf, "%s %d %d %d %d %d %d %d %d", &arr[i].name, &arr[i].ride[0], &arr[i].ride[1], &arr[i].ride[2], &arr[i].ride[3], &arr[i].ride[4], &arr[i].ride[5], &arr[i].ride[6], &arr[i].ride[7]);
		arr[i].result = count_result(size, i, arr);
		i++;
	}
	fclose(inf);
	return 1;
}
