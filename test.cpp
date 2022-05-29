#define _CRT_SECURE_NO_WARNINGS 1//����scanf����

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <conio.h>
#include <ctime>
#include <cstring>
using namespace std;


struct Timetable		//������Ϣ�ṹ
{
	char no[10];//���
	int hour;//����ʱ�䣬Сʱ
	int minute;//����ʱ�䣬���� 
	char Starting_station[10];//ʼ��վ
	char Last_station[10];//�յ�վ
	float time;//�г�ʱ��
	int fare;//Ʊ�� 
	int max_number;//����ؿ���
	int sold_number;//����Ʊ��
};

//��ʵ�ֵĺ��� 
void Signin();											//����Ա�û���¼��������֤���롣Ҫ��������ַ����е�ȫ���ַ�ת��Ϊ'*'������������
int ReadFromFile(Timetable* timetables, const char* filename); //��filename�ж����ݵ�����timetables�У���ȷ��ȡ�����ļ��а������ε���Ŀ�����򷵻�-1
int AddBus(Timetable* timetables, int n);				//���ӳ��Ρ����복�κţ�������ڸó���Ӧ����������
int WritetoFile(Timetable* timetables, int n, const char* filename);//������timetables�е�����д�뵽filename�С���ȷд�뷵�ط�-1����					
void ShowBusInfo(Timetable* timetables, int n, int idx); //��ʾtimetables�е�idx���±꣩��Ԫ�ص���Ϣ
void ShowTimetable(Timetable* timetables, int n);		//��ʾ����վ�����г�����Ϣ����������ʱ�������������
void Query(Timetable* timetables, int n);				//��ѯ������Ҫ����԰����κ��յ�վ�ֱ���в�ѯ
int find(Timetable* timetables, int n, char* no);			// �����κŲ��ҳ�����Ϣ
int find(Timetable* timetables, int n, char* Last_station, int* b);    //���յ�վ���ҳ�����Ϣ
int DelBus(Timetable* timetables, int n);				//ɾ������
void GenerateLogFileName(const char* filename);				// ���ݵ�������������־������2019-9-19.log�����ļ�
int InitializaionPassageMode(Timetable* timetables, char* LogFileName, const char* filename);//�ӵ�����־�ļ�LogFileName�ж����ݵ�timetables�������Ƿ���ڣ���filename�е����ݶ���timetables�У�ע��Ҫ��sold_number��Ա��Ϊ0������timetables�е���Ϣд��LogFileName��
void TicketOrder(Timetable* timetables, int n);			//��Ʊ
void TicketDelete(Timetable* timetables, int n);			//��Ʊ
int StopSerive(Timetable* timetables, int n, char* no);	//�жϳ��εķ���ʱ���ж��Ƿ�ֹͣ��Ʊ����Ʊ����
void SortbyTime(Timetable* timetables, int n);			//��timetables�е�Ԫ�ذ�����ʱ�������������
int Quit();                                              //�Ƿ��˳�ϵͳ

//�Ѹ����ĺ��� 
void AdminMode(Timetable* timetables, const char* filename);
void PassagerMode(Timetable* timetables, const char* filename);
int MainWindowSelect();									//�����ڽ���
int PassagerWindowSelect();								//�ÿͽ���

//�������� 
void GenerateLogFileName(char* LogFileName);			//���ݵ�ǰ����������־�ļ���
int AdminWindowSelect();								//����Ա���棬�ú����Ѿ���ȫ����
void int2str(int n, char* str);							//������ת��Ϊ�ַ���

int main()												// ������
{
	int UserChoice;
	Timetable timetables[100];
	while (1)
	{
		switch (UserChoice = MainWindowSelect())
		{
		case 1:  AdminMode(timetables, "bus.txt"); break;			//����Աģʽ
		case 2:  PassagerMode(timetables, "bus.txt"); break;		//��ͨ�˿�ģʽ	
		case 3:  if (Quit() != 1)continue;							//�˳�
		}
		if (UserChoice == 3) break;
	}
	return 0;
}

void AdminMode(Timetable* timetables, const char* filename)   // ����Աģʽ
{
	Signin();
	int n = ReadFromFile(timetables, "bus.txt");
	while (1)
	{
		int AdminChoice = AdminWindowSelect();
		switch (AdminChoice)
		{
		case 1:	n = AddBus(timetables, n); WritetoFile(timetables, n, filename);
			system("pause"); break;	//���ӳ�����Ϣ,����ʱд���ļ���
		case 2:	ShowTimetable(timetables, n); system("pause"); break;	//���ʱ�̱�
		case 3:	Query(timetables, n); system("pause"); break;	//������Ϣ��ѯ	   			
		case 4:	n = DelBus(timetables, n); system("pause");
			WritetoFile(timetables, n, filename); break;//ע������,����ʱд���ļ���
		case 5: WritetoFile(timetables, n, filename);
			return;//�����ϼ��˵�,������Ϣ�����bus.txt�ļ�
		}
	}
}

void PassagerMode(Timetable* timetables, const char* filename)			// ��ͨ�˿�ģʽ
{
	char LogFileName[200];
	GenerateLogFileName(LogFileName);//���ݵ�ǰ����������־�ļ���
	int n = InitializaionPassageMode(timetables, LogFileName, filename);  //����־�ļ��ж�ȡ���쳵����Ϣ�������У�������־�ļ������ڣ����filename�ļ��ж�ȡ,����filename�����ݸ��Ƶ���־�ļ��С� 
	int PassagerChoice;
	while (1)
	{
		switch (PassagerChoice = PassagerWindowSelect())
		{
		case 1:Query(timetables, n); system("pause"); break;//������Ϣ��ѯ
		case 2:TicketOrder(timetables, n); WritetoFile(timetables, n, LogFileName);
			system("pause"); break;//����Ʊ���������ļ���Ϣ
		case 3:TicketDelete(timetables, n); WritetoFile(timetables, n, LogFileName);
			system("pause"); break;//�˻س�Ʊ���������ļ���Ϣ
		case 4:WritetoFile(timetables, n, LogFileName); system("pause");
			return;//�����ϼ��˵�
		}
	}
}

void int2str(int n, char* str)   //������ת��Ϊ�ַ���
{
	int i = 0, j;
	char c;
	while (n)
	{
		str[i++] = n % 10 + '0';
		n = n / 10;
	}
	str[i] = '\0';

	j = i - 1; i = 0;
	for (; i < j; i++, j--)
	{
		c = str[i]; str[i] = str[j]; str[j] = c;
	}
}

void GenerateLogFileName(char* filename)			//���ݵ�������������־������2019-9-19.log��
{
	filename[0] = '\0';
	struct tm* local;				//ʱ��ṹ�� 
	time_t t;  //�ѵ�ǰʱ���t
	t = time(NULL);					//NULL��stdio.h�ж���Ϊ0
	local = localtime(&t);			//��ȡ��ǰϵͳʱ�� 

	char year[5], month[3], day[3];	//�������շֱ�ת�����ַ�����

	int2str(int(1900 + local->tm_year), year);
	int2str(1 + local->tm_mon, month);
	int2str(local->tm_mday, day);

	strcat(filename, year);					//������־�ļ���
	strcat(filename, "-");
	strcat(filename, month);
	strcat(filename, "-");
	strcat(filename, day);
	strcat(filename, ".log");
	return;
}

int InitializaionPassageMode(Timetable* timetables, char* LogFileName, const char* filename) //�ӵ�����־�ļ�LogFileName�ж����ݵ�timetables�������Ƿ���ڣ���filename�е����ݶ���timetables�У�ע��Ҫ��sold_number��Ա��Ϊ0������timetables�е���Ϣд��LogFileName
{
	int n = 0;
	ifstream fp(LogFileName);
	bool flag = fp.good();
	fp.close();
	if (flag == false)
	{
		n = ReadFromFile(timetables, filename);
		WritetoFile(timetables, n, LogFileName);
	}
	else
	{
		n = ReadFromFile(timetables, LogFileName);
	}
	return n;
}

int Quit()  //ѯ���Ƿ�Ҫ�˳�����ϵͳ��y/n��,���롯y����Y��ʱ����1�����򷵻�0
{
	cin.sync(); //����cin�������ڵ�����
	char a;
	cout << "�Ƿ�Ҫ�˳�����ϵͳ����y/n��" << endl;
	cin >> a;
	if (a == 'y')return 1;
	else return 0;
}

int MainWindowSelect()				//������
{
	int num;
	system("cls");
	cout << "\n\t*******************��ӭʹ�ó�Ʊ����ϵͳ*********************\n";
	cout << "\t\t\t\t��ѡ�����ĵ�¼��ʽ��\n";
	cout << "\t\t\t\t1.����Ա��¼\n";
	cout << "\t\t\t\t2.��ͨ�˿͵�¼\n";
	cout << "\t\t\t\t3.�˳�\n";
	cout << "\t************************************************************\n";
	cin >> num;
	while (num <= 0 || num > 3)
	{
		cout << "�Ƿ�����,��������Ҫ���е�������\n";
		cin >> num;
	}
	return num;
}

int AdminWindowSelect() 				//����Ա�˵�����
{
	system("cls");
	int num;
	cout << "\n\t********************����Ա�˵�***********************\n";
	cout << "\t\t\t\t1�����ӳ�����Ϣ\n\t\t\t\t2���鿴���г�����Ϣ\n\t\t\t\t3��������Ϣ��ѯ\n\t\t\t\t4��ע������\n\t\t\t\t5������\n";
	cout << "\t\t\t      ��������Ҫ���е�������\n";
	cout << "\t************************************************************\n";
	cin.sync();
	cin >> num;
	while (num <= 0 || num > 6)
	{
		cout << "�Ƿ�����,��������Ҫ���е�������\n";
		cin >> num;
	};
	return num;
}

int PassagerWindowSelect()				//��ͨ�û���¼�˵����� 
{
	system("cls");
	cout << "\n\t****************�˿͵�¼�˵�**********************\n";
	cout << "\t\t\t\t1��������Ϣ��ѯ\n\t\t\t\t2������Ʊ\n\t\t\t\t3����Ʊ\n\t\t\t\t4������\n";
	cout << "\t\t\t      ��������Ҫ���е�������\n";
	cout << "\t************************************************************\n";
	int num;
	cin >> num;
	while (num > 4 || num < 1)
	{
		cout << "�Ƿ����룬��������Ҫ���е�������:\n";
		cin >> num;
	}
	return num;
}

void Signin()						//����Ա�û���¼��������֤���롣Ҫ��������ַ����е�ȫ���ַ�ת��Ϊ'*'������������
{
	int i = 0;
	string s;
	char code[20];
	system("cls");
	do
	{
		cout << "�������˺�:";
		cin >> s;
		cout << "����������:";
		while (1)
		{
			code[i] = _getch();
			if (code[i] == 13)
			{
				break;
			}
			else
			{
				cout << "*";
				i++;
			}
		}
		code[i] = '\0';
		if (s == "admin" && !strcmp(code, "123"))
		{
			return;
		}
		cout << endl << "�˺Ż��������!" << endl;
		i = 0;
	} while (1);
}

int AddBus(Timetable* timetables, int n)     //��ӳ�����Ϣ
{
	int i;
	system("cls");
	do
	{
		cout << "��������:";
		cin >> timetables[n].no;
		for (i = 0; i < n; i++)
		{
			if (!strcmp(timetables[n].no, timetables[i].no))
			{
				cout << "�ó����Ѿ�����" << endl;
				break;
			}
		}
		if (i == n)break;
	} while (1);

	cout << "�����뷢��ʱ��(h m):";
	cin >> timetables[n].hour >> timetables[n].minute;
	cout << "������ʼ��վ:";
	cin >> timetables[n].Starting_station;
	cout << "�������յ�վ:";
	cin >> timetables[n].Last_station;
	cout << "�������г�ʱ��:";
	cin >> timetables[n].time;
	cout << "����������ؿ���:";
	cin >> timetables[n].max_number;
	cout << "������Ʊ��:";
	cin >> timetables[n].fare;
	timetables[n].sold_number = 0;
	cout << "��ӳɹ�!" << endl;
	return n + 1;
}

int DelBus(Timetable* timetables, int n)              //ɾ��������Ϣ
{
	int i;
	char no[20];
	system("cls");
	cout << "������Ҫɾ���ĳ���:";
	cin >> no;

	i = find(timetables, n, no);
	if (i == -1)
	{
		cout << "�޸ó�����Ϣ!" << endl;
		return n;
	}
	cout << "�Ƿ�ɾ��?(Y/N):";
	cin >> no;
	if (!strcmp("Y", no) || !strcmp("y", no))
	{
		for (; i < n - 1; i++)
		{
			timetables[i] = timetables[i + 1];
		}
		cout << "ɾ���ɹ�!" << endl;
		return n - 1;
	}
	cout << "�Ѿ�ȡ��ɾ��!" << endl;
	return n;
}

void TicketDelete(Timetable* timetables, int n)          //��Ʊ
{
	int i, k;
	char no[20];
	system("cls");
	cout << "������Ҫ��Ʊ�ĳ���:";
	cin >> no;

	i = find(timetables, n, no);
	if (i == -1)
	{
		cout << "�޸ó�����Ϣ!" << endl;
		return;
	}
	if (StopSerive(timetables, n, no) == 1 || timetables[i].sold_number == 0)
	{
		cout << "���ɹ�Ʊ!" << endl;
	}
	else
	{
		cout << "������Ҫ��Ʊ������:";
		cin >> k;
		if (k <= timetables[i].sold_number)
		{
			timetables[i].sold_number -= k;
			cout << "��Ʊ�ɹ���" << endl;
		}
		else
		{
			cout << "��Ʊ��������!" << endl;
		}
	}
}

void TicketOrder(Timetable* timetables, int n)  //��Ʊ      
{
	int i, k;
	char no[20];
	system("cls");
	cout << "������Ҫ����ĳ���:";
	cin >> no;

	i = find(timetables, n, no);
	if (i == -1)
	{
		cout << "�޸ó�����Ϣ!" << endl;
		return;
	}
	if (StopSerive(timetables, n, no) == 1 || timetables[i].max_number - timetables[i].sold_number <= 0)
	{
		cout << "���ɹ�Ʊ!" << endl;
	}
	else
	{
		cout << "������Ҫ���������:";
		cin >> k;
		if (k + timetables[i].sold_number <= timetables[i].max_number)
		{
			timetables[i].sold_number += k;
			cout << "��Ʊ�ɹ���" << endl;
		}
		else
		{
			cout << "��Ʊ����!" << endl;
		}
	}
}

int find(Timetable* timetables, int n, char* no)				//�����ν��в�ѯ�����ҵ����򷵻ظó������������е��±ꣻ���򷵻�-1
{
	int i;
	for (i = 0; i < n; i++)
	{
		if (!strcmp(timetables[i].no, no))
		{
			return i;
		}
	}
	return -1;
}

int find(Timetable* timetables, int n, char* Last_station, int* b)      //���յ�վ���ң����ҵ�������b�м�¼�յ�վΪLast_station�����г����������ж�Ӧ���±꣬�����ظ�����վ�յ�վΪLast_station�ĳ����������򷵻�-1
{
	int i, k = 0;
	for (i = 0; i < n; i++)
	{
		if (!strcmp(timetables[i].Last_station, Last_station))
		{
			b[k++] = i;
		}
	}
	return k;
}

int StopSerive(Timetable* timetables, int n, char* no)         //�жϸó����Ƿ�ֹͣ����Ʊ��1��ֹͣ��0����ֹͣ 
{
	struct tm* local;       //ʱ��ṹ�� 
	time_t t;				//�ѵ�ǰʱ���t
	t = time(NULL);
	local = localtime(&t);      /////��ȡ��ǰϵͳʱ�� 
	int i = find(timetables, n, no);

	if ((local->tm_hour * 60 + local->tm_min) + 10 < (timetables[i].hour * 60 + timetables[i].minute))
		return 0;
	return 1;
}

void SortbyTime(Timetable* timetables, int n)    //��timetables�е�Ԫ�ذ�����ʱ�������������
{
	int i, j, t1, t2;
	Timetable temp;
	for (i = 0; i < n; i++)
	{
		t1 = timetables[i].hour * 100 + timetables[i].minute;
		for (j = i + 1; j < n; j++)
		{
			t2 = timetables[j].hour * 100 + timetables[j].minute;
			if (t1 > t2)
			{
				temp = timetables[i];
				timetables[i] = timetables[j];
				timetables[j] = temp;
			}
		}
	}
}

void ShowTimetable(Timetable* timetables, int n)             //��ʾ�ó�վ�е����г�����Ϣ������ʾ�ó��Ƿ�ֹͣ����
{
	int i;
	system("cls");
	cout << "����\t����ʱ��\t���\t�յ�\t�г�ʱ��\t�ؿ���\t�۳�\t����״̬" << endl;
	for (i = 0; i < n; i++)
	{
		ShowBusInfo(timetables, n, i);
		cout << endl;
	}
}

void ShowBusInfo(Timetable* timetables, int n, int idx)   //��ʾtimetables�����е�idx���±꣩������Ϣ������ʾ�ó��Ƿ�ֹͣ����
{
	cout << timetables[idx].no << "  ";
	cout << "\t";
	cout << timetables[idx].hour << ":";
	cout << timetables[idx].minute << "  ";
	cout << "\t\t";
	cout << timetables[idx].Starting_station << "  ";
	cout << "\t";
	cout << timetables[idx].Last_station << "  ";
	cout << "\t";
	cout << timetables[idx].time << "  ";
	cout << "\t\t";
	cout << timetables[idx].max_number << "  ";
	cout << "\t";
	cout << timetables[idx].sold_number << "  ";
	cout << "\t";
	if (StopSerive(timetables, n, timetables[idx].no) == 1)
	{
		cout << "ֹͣ����  ";
	}
	else {
		cout << "��������  ";
	}
}

int ReadFromFile(Timetable* timetables, const char* filename)   //��filename�ж����ݵ�����timetables�У���ȷ��ȡ�����ļ��а������ε���Ŀ�����򷵻�-1
{
	int i = 0;
	ifstream fp(filename);
	if (fp.is_open() == false)return 0;
	fp >> timetables[i].no >> timetables[i].hour >> timetables[i].minute >> timetables[i].Starting_station >> timetables[i].Last_station >> timetables[i].time >> timetables[i].fare >> timetables[i].max_number >> timetables[i].sold_number;
	while (!fp.eof())
	{
		i++;
		fp >> timetables[i].no >> timetables[i].hour >> timetables[i].minute >> timetables[i].Starting_station >> timetables[i].Last_station >> timetables[i].time >> timetables[i].fare >> timetables[i].max_number >> timetables[i].sold_number;
	}
	fp.close();
	return i;
}

int WritetoFile(Timetable* timetables, int n, const char* filename)   //������timetables�е�����д�뵽filename�С���ȷд�뷵�ط�-1����
{
	int i;
	ofstream fp(filename);
	if (fp.is_open() == false)return -1;
	for (i = 0; i < n; i++)
	{
		fp << timetables[i].no << " ";
		fp << timetables[i].hour << " ";
		fp << timetables[i].minute << " ";
		fp << timetables[i].Starting_station << " ";
		fp << timetables[i].Last_station << " ";
		fp << timetables[i].time << " ";
		fp << timetables[i].fare << " ";
		fp << timetables[i].max_number << " ";
		fp << timetables[i].sold_number << endl;
	}
	fp.close();
	return 1;
}

void Query(Timetable* timetables, int n)  //�ܹ��ֱ𰴳��λ��յ�վ��ѯ���Σ��鵽����ʾ������Ϣ�����������ʾ��
{
	int choice;
	int i, k, b[100];
	char no[20];
	system("cls");
	cout << "1:�����β�ѯ" << endl;
	cout << "2:���յ�վ��ѯ" << endl;
	cout << "������ѡ��:";
	cin >> choice;
	if (choice == 1)
	{
		cout << "������Ҫ��ѯ�ĳ���:";
		cin >> no;
		k = find(timetables, n, no);
		if (k == -1)
		{
			cout << "�޸ó�����Ϣ!" << endl;
			return;
		}
		ShowBusInfo(timetables, n, k);
	}
	if (choice == 2)
	{
		cout << "������Ҫ��ѯ���յ�վ:";
		cin >> no;
		SortbyTime(timetables, n);
		k = find(timetables, n, no, b);
		if (k == 0)
		{
			cout << "�޸��յ�վ��Ϣ!" << endl;
			return;
		}
		for (i = 0; i < k; i++)
			ShowBusInfo(timetables, n, b[i]);
	}
}

