#define _CRT_SECURE_NO_WARNINGS 1//忽略scanf警告

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <conio.h>
#include <ctime>
#include <cstring>
using namespace std;


struct Timetable		//车辆信息结构
{
	char no[10];//班次
	int hour;//发车时间，小时
	int minute;//发车时间，分钟 
	char Starting_station[10];//始发站
	char Last_station[10];//终点站
	float time;//行车时间
	int fare;//票价 
	int max_number;//最大载客数
	int sold_number;//已售票数
};

//待实现的函数 
void Signin();											//管理员用户登录，用于验证密码。要求将输入的字符串中的全部字符转换为'*'，将密码隐藏
int ReadFromFile(Timetable* timetables, const char* filename); //从filename中读数据到数组timetables中，正确读取返回文件中包含车次的数目，否则返回-1
int AddBus(Timetable* timetables, int n);				//增加车次。输入车次号，如果存在该车次应该重新输入
int WritetoFile(Timetable* timetables, int n, const char* filename);//将数组timetables中的数据写入到filename中。正确写入返回非-1的数					
void ShowBusInfo(Timetable* timetables, int n, int idx); //显示timetables中第idx（下标）个元素的信息
void ShowTimetable(Timetable* timetables, int n);		//显示汽车站中所有车次信息，并按发车时间进行升序排序
void Query(Timetable* timetables, int n);				//查询操作，要求可以按车次和终点站分别进行查询
int find(Timetable* timetables, int n, char* no);			// 按车次号查找车次信息
int find(Timetable* timetables, int n, char* Last_station, int* b);    //按终点站查找车次信息
int DelBus(Timetable* timetables, int n);				//删除车次
void GenerateLogFileName(const char* filename);				// 根据当天日期生成日志名（如2019-9-19.log）的文件
int InitializaionPassageMode(Timetable* timetables, char* LogFileName, const char* filename);//从当天日志文件LogFileName中读数据到timetables，若该是否存在，则将filename中的内容读至timetables中（注意要将sold_number成员置为0）并将timetables中的信息写入LogFileName；
void TicketOrder(Timetable* timetables, int n);			//买票
void TicketDelete(Timetable* timetables, int n);			//退票
int StopSerive(Timetable* timetables, int n, char* no);	//判断车次的发车时间判断是否停止售票或退票服务
void SortbyTime(Timetable* timetables, int n);			//将timetables中的元素按发车时间进行升序排序
int Quit();                                              //是否退出系统

//已给出的函数 
void AdminMode(Timetable* timetables, const char* filename);
void PassagerMode(Timetable* timetables, const char* filename);
int MainWindowSelect();									//主窗口界面
int PassagerWindowSelect();								//旅客界面

//其他函数 
void GenerateLogFileName(char* LogFileName);			//根据当前日期生成日志文件名
int AdminWindowSelect();								//管理员界面，该函数已经完全给出
void int2str(int n, char* str);							//将整数转换为字符串

int main()												// 主函数
{
	int UserChoice;
	Timetable timetables[100];
	while (1)
	{
		switch (UserChoice = MainWindowSelect())
		{
		case 1:  AdminMode(timetables, "bus.txt"); break;			//管理员模式
		case 2:  PassagerMode(timetables, "bus.txt"); break;		//普通顾客模式	
		case 3:  if (Quit() != 1)continue;							//退出
		}
		if (UserChoice == 3) break;
	}
	return 0;
}

void AdminMode(Timetable* timetables, const char* filename)   // 管理员模式
{
	Signin();
	int n = ReadFromFile(timetables, "bus.txt");
	while (1)
	{
		int AdminChoice = AdminWindowSelect();
		switch (AdminChoice)
		{
		case 1:	n = AddBus(timetables, n); WritetoFile(timetables, n, filename);
			system("pause"); break;	//增加车次信息,并及时写到文件中
		case 2:	ShowTimetable(timetables, n); system("pause"); break;	//浏览时刻表
		case 3:	Query(timetables, n); system("pause"); break;	//车辆信息查询	   			
		case 4:	n = DelBus(timetables, n); system("pause");
			WritetoFile(timetables, n, filename); break;//注销车次,并及时写到文件中
		case 5: WritetoFile(timetables, n, filename);
			return;//返回上级菜单,并将信息保存回bus.txt文件
		}
	}
}

void PassagerMode(Timetable* timetables, const char* filename)			// 普通顾客模式
{
	char LogFileName[200];
	GenerateLogFileName(LogFileName);//根据当前日期生成日志文件名
	int n = InitializaionPassageMode(timetables, LogFileName, filename);  //从日志文件中读取当天车次信息至数组中，若该日志文件不存在，则从filename文件中读取,并将filename的内容复制到日志文件中。 
	int PassagerChoice;
	while (1)
	{
		switch (PassagerChoice = PassagerWindowSelect())
		{
		case 1:Query(timetables, n); system("pause"); break;//车辆信息查询
		case 2:TicketOrder(timetables, n); WritetoFile(timetables, n, LogFileName);
			system("pause"); break;//购买车票，并更改文件信息
		case 3:TicketDelete(timetables, n); WritetoFile(timetables, n, LogFileName);
			system("pause"); break;//退回车票，并更改文件信息
		case 4:WritetoFile(timetables, n, LogFileName); system("pause");
			return;//返回上级菜单
		}
	}
}

void int2str(int n, char* str)   //将整数转换为字符串
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

void GenerateLogFileName(char* filename)			//根据当天日期生成日志名（如2019-9-19.log）
{
	filename[0] = '\0';
	struct tm* local;				//时间结构体 
	time_t t;  //把当前时间给t
	t = time(NULL);					//NULL在stdio.h中定义为0
	local = localtime(&t);			//获取当前系统时间 

	char year[5], month[3], day[3];	//将年月日分别转换成字符串型

	int2str(int(1900 + local->tm_year), year);
	int2str(1 + local->tm_mon, month);
	int2str(local->tm_mday, day);

	strcat(filename, year);					//生成日志文件名
	strcat(filename, "-");
	strcat(filename, month);
	strcat(filename, "-");
	strcat(filename, day);
	strcat(filename, ".log");
	return;
}

int InitializaionPassageMode(Timetable* timetables, char* LogFileName, const char* filename) //从当天日志文件LogFileName中读数据到timetables，若该是否存在，则将filename中的内容读至timetables中（注意要将sold_number成员置为0）并将timetables中的信息写入LogFileName
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

int Quit()  //询问是否要退出整个系统（y/n）,输入’y’或’Y’时返回1，否则返回0
{
	cin.sync(); //清理cin输入流内的内容
	char a;
	cout << "是否要退出整个系统？（y/n）" << endl;
	cin >> a;
	if (a == 'y')return 1;
	else return 0;
}

int MainWindowSelect()				//主窗口
{
	int num;
	system("cls");
	cout << "\n\t*******************欢迎使用车票管理系统*********************\n";
	cout << "\t\t\t\t请选择您的登录方式：\n";
	cout << "\t\t\t\t1.管理员登录\n";
	cout << "\t\t\t\t2.普通顾客登录\n";
	cout << "\t\t\t\t3.退出\n";
	cout << "\t************************************************************\n";
	cin >> num;
	while (num <= 0 || num > 3)
	{
		cout << "非法输入,请输入需要运行的命令编号\n";
		cin >> num;
	}
	return num;
}

int AdminWindowSelect() 				//管理员菜单界面
{
	system("cls");
	int num;
	cout << "\n\t********************管理员菜单***********************\n";
	cout << "\t\t\t\t1、增加车次信息\n\t\t\t\t2、查看所有车次信息\n\t\t\t\t3、车辆信息查询\n\t\t\t\t4、注销车次\n\t\t\t\t5、返回\n";
	cout << "\t\t\t      请输入需要运行的命令编号\n";
	cout << "\t************************************************************\n";
	cin.sync();
	cin >> num;
	while (num <= 0 || num > 6)
	{
		cout << "非法输入,请输入需要运行的命令编号\n";
		cin >> num;
	};
	return num;
}

int PassagerWindowSelect()				//普通用户登录菜单界面 
{
	system("cls");
	cout << "\n\t****************顾客登录菜单**********************\n";
	cout << "\t\t\t\t1、车辆信息查询\n\t\t\t\t2、购买车票\n\t\t\t\t3、退票\n\t\t\t\t4、返回\n";
	cout << "\t\t\t      请输入需要运行的命令编号\n";
	cout << "\t************************************************************\n";
	int num;
	cin >> num;
	while (num > 4 || num < 1)
	{
		cout << "非法输入，请输入需要运行的命令编号:\n";
		cin >> num;
	}
	return num;
}

void Signin()						//管理员用户登录，用于验证密码。要求将输入的字符串中的全部字符转换为'*'，将密码隐藏
{
	int i = 0;
	string s;
	char code[20];
	system("cls");
	do
	{
		cout << "请输入账号:";
		cin >> s;
		cout << "请输入密码:";
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
		cout << endl << "账号或密码错误!" << endl;
		i = 0;
	} while (1);
}

int AddBus(Timetable* timetables, int n)     //添加车次信息
{
	int i;
	system("cls");
	do
	{
		cout << "请输入班次:";
		cin >> timetables[n].no;
		for (i = 0; i < n; i++)
		{
			if (!strcmp(timetables[n].no, timetables[i].no))
			{
				cout << "该车次已经存在" << endl;
				break;
			}
		}
		if (i == n)break;
	} while (1);

	cout << "请输入发车时间(h m):";
	cin >> timetables[n].hour >> timetables[n].minute;
	cout << "请输入始发站:";
	cin >> timetables[n].Starting_station;
	cout << "请输入终点站:";
	cin >> timetables[n].Last_station;
	cout << "请输入行车时间:";
	cin >> timetables[n].time;
	cout << "请输入最大载客数:";
	cin >> timetables[n].max_number;
	cout << "请输入票价:";
	cin >> timetables[n].fare;
	timetables[n].sold_number = 0;
	cout << "添加成功!" << endl;
	return n + 1;
}

int DelBus(Timetable* timetables, int n)              //删除车次信息
{
	int i;
	char no[20];
	system("cls");
	cout << "请输入要删除的车次:";
	cin >> no;

	i = find(timetables, n, no);
	if (i == -1)
	{
		cout << "无该车次信息!" << endl;
		return n;
	}
	cout << "是否删除?(Y/N):";
	cin >> no;
	if (!strcmp("Y", no) || !strcmp("y", no))
	{
		for (; i < n - 1; i++)
		{
			timetables[i] = timetables[i + 1];
		}
		cout << "删除成功!" << endl;
		return n - 1;
	}
	cout << "已经取消删除!" << endl;
	return n;
}

void TicketDelete(Timetable* timetables, int n)          //退票
{
	int i, k;
	char no[20];
	system("cls");
	cout << "请输入要退票的车次:";
	cin >> no;

	i = find(timetables, n, no);
	if (i == -1)
	{
		cout << "无该车次信息!" << endl;
		return;
	}
	if (StopSerive(timetables, n, no) == 1 || timetables[i].sold_number == 0)
	{
		cout << "不可购票!" << endl;
	}
	else
	{
		cout << "请输入要退票的数量:";
		cin >> k;
		if (k <= timetables[i].sold_number)
		{
			timetables[i].sold_number -= k;
			cout << "退票成功！" << endl;
		}
		else
		{
			cout << "退票数量不对!" << endl;
		}
	}
}

void TicketOrder(Timetable* timetables, int n)  //售票      
{
	int i, k;
	char no[20];
	system("cls");
	cout << "请输入要购买的车次:";
	cin >> no;

	i = find(timetables, n, no);
	if (i == -1)
	{
		cout << "无该车次信息!" << endl;
		return;
	}
	if (StopSerive(timetables, n, no) == 1 || timetables[i].max_number - timetables[i].sold_number <= 0)
	{
		cout << "不可购票!" << endl;
	}
	else
	{
		cout << "请输入要购买的数量:";
		cin >> k;
		if (k + timetables[i].sold_number <= timetables[i].max_number)
		{
			timetables[i].sold_number += k;
			cout << "购票成功！" << endl;
		}
		else
		{
			cout << "余票不足!" << endl;
		}
	}
}

int find(Timetable* timetables, int n, char* no)				//按车次进行查询，若找到，则返回该车次所在数组中的下标；否则返回-1
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

int find(Timetable* timetables, int n, char* Last_station, int* b)      //按终点站查找，若找到，则在b中记录终点站为Last_station的所有车次在数组中对应的下标，并返回该汽车站终点站为Last_station的车次数；否则返回-1
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

int StopSerive(Timetable* timetables, int n, char* no)         //判断该车次是否停止售退票，1：停止，0：不停止 
{
	struct tm* local;       //时间结构体 
	time_t t;				//把当前时间给t
	t = time(NULL);
	local = localtime(&t);      /////获取当前系统时间 
	int i = find(timetables, n, no);

	if ((local->tm_hour * 60 + local->tm_min) + 10 < (timetables[i].hour * 60 + timetables[i].minute))
		return 0;
	return 1;
}

void SortbyTime(Timetable* timetables, int n)    //将timetables中的元素按发车时间进行升序排序
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

void ShowTimetable(Timetable* timetables, int n)             //显示该车站中的所有车次信息，并显示该车是否停止服务
{
	int i;
	system("cls");
	cout << "车次\t发车时间\t起点\t终点\t行车时间\t载客量\t售出\t服务状态" << endl;
	for (i = 0; i < n; i++)
	{
		ShowBusInfo(timetables, n, i);
		cout << endl;
	}
}

void ShowBusInfo(Timetable* timetables, int n, int idx)   //显示timetables数组中第idx（下标）车次信息，并显示该车是否停止服务
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
		cout << "停止服务  ";
	}
	else {
		cout << "正在运行  ";
	}
}

int ReadFromFile(Timetable* timetables, const char* filename)   //从filename中读数据到数组timetables中，正确读取返回文件中包含车次的数目，否则返回-1
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

int WritetoFile(Timetable* timetables, int n, const char* filename)   //将数组timetables中的数据写入到filename中。正确写入返回非-1的数
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

void Query(Timetable* timetables, int n)  //能够分别按车次或终点站查询车次，查到则显示车次信息，否则给出提示。
{
	int choice;
	int i, k, b[100];
	char no[20];
	system("cls");
	cout << "1:按车次查询" << endl;
	cout << "2:按终点站查询" << endl;
	cout << "请输入选择:";
	cin >> choice;
	if (choice == 1)
	{
		cout << "请输入要查询的车次:";
		cin >> no;
		k = find(timetables, n, no);
		if (k == -1)
		{
			cout << "无该车次信息!" << endl;
			return;
		}
		ShowBusInfo(timetables, n, k);
	}
	if (choice == 2)
	{
		cout << "请输入要查询的终点站:";
		cin >> no;
		SortbyTime(timetables, n);
		k = find(timetables, n, no, b);
		if (k == 0)
		{
			cout << "无该终点站信息!" << endl;
			return;
		}
		for (i = 0; i < k; i++)
			ShowBusInfo(timetables, n, b[i]);
	}
}

