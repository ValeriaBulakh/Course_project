#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <windows.h>

typedef struct mbr {//��������� �������
	char name[8];//���
	int ride[8];//������ �������
	int result = 0;//����� �������
	int place;//�����
} mbr;

void put_table(int size, mbr* arr);//����� ��������������� �������
void sort_bubble(int size, mbr* arr);//���������� ���������
int count_result(int size, int j, mbr* arr); //���������� ����������� ����� �������
int best_in_ride(int size, int i, mbr* arr); //������ ������� � ����������� ������
int read_from_file(int size, mbr* arr, char* fname); //������ ������� �� �����

int main() {

	system("chcp 1251");

	int flag = 0;//���� ������ �� ����
	int ptr; // ���������� ����, ���� �����
	int fr, to;// �� ��
	int size = 0;//����� ������ �����
	char fname[30];//��� �����
	mbr* membr;//������ ������
	char s[200];//������ �����
	FILE* inf;//file

	printf("�����������\n��������� ��������� � ������� ���������� ������ ������������ �� ������ ��������\n\n");
	puts("������� ��� �����");
	scanf("%s", &fname);
	inf = fopen(fname, "r");
	if (inf == NULL) {
		printf("����� �� ����������");
		return 0;
	}
	while (!feof(inf)) {
		fgets(s, 200, inf);
		size++;
	}
	fclose(inf); //����� �����

	membr = (mbr*)malloc(size * sizeof(mbr));// ������ ������

	if (read_from_file(size, membr, fname) == NULL) {
		printf("������ � ������ ���������� � ����");
		return 0;
	}
	for (int j = 0; j < size; j++) {
		printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", membr[j].name, membr[j].ride[0], membr[j].ride[1], membr[j].ride[2], membr[j].ride[3], membr[j].ride[4], membr[j].ride[5], membr[j].ride[6], membr[j].ride[7]);

	}

	sort_bubble(size, membr);
	printf("������ ���� �������������\n\n");


	//���� �������� ����
	while (flag == 0) 
	{
		printf("������� ������� - 1\n������� ������� � ���������� ������� - 2\n�������� ������� � ���� - 3\n������ ������� - 4\n�������� ������ ������� - 5\n��������� - 0\n");
		scanf("%d", &ptr);
		switch (ptr) {
		case 1: {
			system("cls");
			put_table(size, membr);
			break;
		}
		case 2: {
			printf("������� ����� �������� ���������\n");
			scanf("%d", &fr);
			printf("������� ������ �������� ���������\n");
			scanf("%d", &to);
			for (int i = 0; i < size; i++) {
				if (membr[i].result >= fr && membr[i].result <= to)
					printf("\n%s %d", membr[i].name, membr[i].result);
			}
			printf("\n\n");
			break;
		}
		case 3: {
			FILE* sec;//����� ���� ��� ������
			char fnamesec[60];//� ���
			puts("������� ��� �����\n");
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
			printf("������� ��������\n\n");
			if (sec != NULL) fclose(sec);
			break;
		}
		case 4: {
			int h;//����� ������
			system("cls");
			put_table(size, membr);
			puts("������� ����� ������\n");
			scanf("%d", &h);
			printf("������ � %d ������ ���� %s\n\n", h, membr[best_in_ride(size, h - 1, membr)].name);
			break;
		}
		case 5: {
			int m = 0;//����� �������
			int n = 0;//����� ������
			int n2;//����� �������� �������
			printf("������� ����� ������� � ����� ������\n");
			scanf("%d %d", &m, &n);
			printf("������� ����� ��������\n");
			scanf("%d", &n2);
			membr[m - 1].ride[n - 1] = n2;
			membr[m - 1].result = count_result(size, m - 1, membr);
			sort_bubble(size, membr);
			put_table(size, membr);
			break;
		}
		case 0: { //�����
			flag = 1;
			break;
		}
		default: {
			system("cls");
			printf("������ ����������� ������\n");
			break;
		}
		}
	}
	free(membr);
	return 1;
}

void put_table(int size, mbr* arr) 
{
	int place = 1;//������� ����
	
	printf("�������/����� ������\n");
	printf("N\t1\t2\t3\t4\t5\t6\t7\t8\t����\t�����\t\n");

	arr[0].place = place;//������� ��� ������������� �� �����������
	for (int i = 0; i < size; i++) {
		printf("%s\t", arr[i].name);
		if (i != 0 && arr[i - 1].result < arr[i].result) {//��������, ����� �� 2 � ����� ������ ���� �����
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
