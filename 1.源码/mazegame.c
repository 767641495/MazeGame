#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include "mazeDynamic_stack.h"
int M;	   //迷宫的行数
int N;	   //迷宫的列数
int P = 7; //随机生成迷宫的路和墙的期望比值(P:10-P)
int Maze[15 + 5][15 + 5], vis[15 + 5][15 + 5];
int count = 0;						 //路径总数
int dirr[6];						 //遍历顺序
int minlength = (15 + 5) * (15 + 5); //目前最小的路径长度
int maxread;						 //最大阅读路径数量
char ch;
void Random() //随机生成迷宫的函数
{
	srand(time(NULL));
	memset(vis, 0, sizeof(vis));
	for (int i = 0; i <= M - 1; i++) //按照所给比例生成墙
		for (int j = 0; j <= N - 1; j++)
			if (rand() % 10 >= P)
				Maze[i][j] = 1;
			else
				Maze[i][j] = 0;

	for (int i = 1; i <= M - 2; i++) //如果一条路四周都是墙，那么全部打通
		for (int j = 1; j <= N - 2; j++)
			if (Maze[i][j] == 0 && Maze[i + 1][j] == 1 && Maze[i - 1][j] == 1 && Maze[i][j + 1] == 1 && Maze[i][j - 1] == 1)
				Maze[i + 1][j] = Maze[i - 1][j] = Maze[i][j + 1] = Maze[i][j - 1] = 0;

	for (int i = 1; i <= M - 2; i++) //如果一条路四周都是路，把这条路堵死，防止路径过多
		for (int j = 1; j <= N - 2; j++)
			if (Maze[i][j] == 0 && Maze[i + 1][j] == 0 && Maze[i - 1][j] == 0 && Maze[i][j + 1] == 0 && Maze[i][j - 1] == 0 && Maze[i + 1][j + 1] == 0 && Maze[i - 1][j - 1] == 0 && Maze[i + 1][j - 1] == 0 && Maze[i - 1][j + 1] == 0)
				Maze[i][j] = 1;

	for (int i = 0; i <= M - 1; i++) //四周封墙
		for (int j = 0; j <= N - 1; j++)
			if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
				Maze[i][j] = 1;

	Maze[1][0] = Maze[1][1] = Maze[M - 2][N - 1] = Maze[M - 2][N - 2] = 0;
	//起点右终点左打通
}
Status Judge(PosType e) //判断是否可走
{
	if (e.x >= 0 && e.x < M && e.y >= 0 && e.y < N && (Maze[e.x][e.y] == 0 || Maze[e.x][e.y] == 3) && vis[e.x][e.y] == 0)
		return OK;
	return OVERFLOW;
}
PosType NextPos(PosType e, int dir) //下一步
{
	PosType E;
	switch (dirr[dir])
	{
	case 1:
		E.x = e.x;
		E.y = e.y + 1;
		break; //向右
	case 2:
		E.x = e.x + 1;
		E.y = e.y;
		break; //向下
	case 3:
		E.x = e.x;
		E.y = e.y - 1;
		break; //向左
	default:
		E.x = e.x - 1;
		E.y = e.y; //向上
	}
	return E;
}
void PrintPath(SqStack *s) //打印路径
{
	int isFirst = TRUE;
	SqStack TempStack;
	InitStack(&TempStack);
	SElemType TempValue;
	while (!StackEmpty((*s)))
	{
		Pop(s, &TempValue);
		Push(&TempStack, TempValue);
	}
	while (!StackEmpty((TempStack)))
	{
		Pop(&TempStack, &TempValue);
		Push(s, TempValue);
		if (isFirst)
			isFirst = FALSE;
		else
			printf("->");
		printf("(%d,%d)", TempValue.seat.x, TempValue.seat.y);
	}
	puts("\n");
}
void PrintMaze() //打印迷宫
{
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (Maze[i][j] == 4)
			{
				printf("×");
				continue;
			}
			if (Maze[i][j] == 3)
			{
				printf("●");
				continue;
			}
			if (Maze[i][j] == 2)
			{
				printf("○");
				continue;
			}
			if (Maze[i][j] == 0 && vis[i][j] > 0)
			{
				switch (dirr[vis[i][j]])
				{
				case 1:
					printf("→");
					break;
				case 2:
					printf("↓");
					break;
				case 3:
					printf("←");
					break;
				case 4:
					printf("↑");
				}
			}
			else if (vis[i][j] == -1)
				printf("×"); //不通的路
			else if (Maze[i][j] == 1)
				printf("■"); //迷宫的墙
			else
				printf("□"); //迷宫未走的路
		}
		puts("");
	}
}
void DFS(PosType start, PosType end, SqStack *s, int sta)
//sta=0时,检测是否可通；sta=1时，动态显示路径
{
	if (count == 1)
		return;
	if (start.x == end.x && start.y == end.y)
	{
		vis[end.x][end.y] = 5;
		count = 1;
		if (sta == 1)
		{
			system("cls");
			PrintMaze();
			puts("找到路径:");
			PrintPath(s);
			puts("");
		}
		return;
	}
	for (int i = 1; i <= 4; i++)
	{
		if (count == 1)
			return;
		SElemType nextpos;
		nextpos.seat = NextPos(start, i);
		vis[start.x][start.y] = i;
		if (sta == 1)
		{
			system("cls");
			PrintMaze();
			Sleep(150);
		}
		if (!Judge(nextpos.seat))
		{
			if (i == 4)
			{
				vis[start.x][start.y] = -1;
				if (sta == 1)
				{
					system("cls");
					PrintMaze();
				}
				return;
			}
			continue;
		}
		vis[nextpos.seat.x][nextpos.seat.y] = 1;
		Push(s, nextpos);
		DFS(nextpos.seat, end, s, sta);
		if (vis[nextpos.seat.x][nextpos.seat.y] != -1)
			vis[nextpos.seat.x][nextpos.seat.y] = 0;
		if (vis[nextpos.seat.x][nextpos.seat.y] == 0)
			vis[nextpos.seat.x][nextpos.seat.y] = -1;
		Pop(s, &nextpos);
	}
}
void MulDfs(PosType start, PosType end, SqStack *s, int len) //深度搜索所有路径
{
	if (count >= maxread)
		return;
	if (start.x == end.x && start.y == end.y)
	{
		vis[end.x][end.y] = 5;
		PrintMaze();
		printf("第%d条路径已找到！该路径长度为：%d\n", ++count, len);
		PrintPath(s);
		ch = getch();
		puts("");
		return;
	}
	for (int i = 1; i <= 4; i++)
	{
		SElemType nextpos;
		nextpos.seat = NextPos(start, i);
		if (!Judge(nextpos.seat))
			continue;
		vis[start.x][start.y] = i;
		vis[nextpos.seat.x][nextpos.seat.y] = 1;
		Push(s, nextpos);
		MulDfs(nextpos.seat, end, s, len + 1);
		vis[nextpos.seat.x][nextpos.seat.y] = 0;
		Pop(s, &nextpos);
	}
}
void BestDfs(PosType start, PosType end, SqStack *s, int len) //深度搜索最短路径
{
	if (start.x == end.x && start.y == end.y)
	{
		if (len == minlength)
		{
			vis[end.x][end.y] = 5;
			count++;
			if (count <= maxread)
			{
				PrintMaze();
				printf("第%d条路径已找到！\n", count);
				PrintPath(s);
				puts("");
			}
			return;
		}
		else if (len < minlength)
		{
			minlength = len;
			system("cls");
			if (maxread != 0)
			{
				puts("随机生成的迷宫为：");
				for (int i = 0; i < M; i++)
				{
					for (int j = 0; j < N; j++)
						if (Maze[i][j] == 1)
							printf("■");
						else
							printf("□");
					puts("");
				}
				puts("\n");
				PrintMaze();
				printf("第1条路径已找到！\n");
				PrintPath(s);
				puts("");
			}
			count = 1;
		}
		return;
	}
	for (int i = 1; i <= 4; i++)
	{
		SElemType nextpos;
		nextpos.seat = NextPos(start, i);
		if (!Judge(nextpos.seat))
			continue;
		vis[start.x][start.y] = i;
		vis[nextpos.seat.x][nextpos.seat.y] = 1;
		Push(s, nextpos);
		BestDfs(nextpos.seat, end, s, len + 1);
		vis[nextpos.seat.x][nextpos.seat.y] = 0;
		Pop(s, &nextpos);
	}
}
int main()
{
	int choice[4];
	for (int i = 1; i <= 4; i++)
		dirr[i] = i;
	dirr[5] = 1;
	SqStack S;
	InitStack(&S);
	PosType start, end;
	while (1)
	{
		fflush(stdin);
		system("cls");
		puts("*************************Maze*************************");
		puts("                       游戏目录");
		puts("                   1.设定迷宫大小");
		puts("                   2.自动生成迷宫");
		puts("                   3.文件导入迷宫");
		puts("                   4.修改遍历顺序");
		puts("                   5.动态显示路径");
		puts("                   6.输出所有路径");
		puts("                   7.输出最短路径");
		puts("                   8.手动迷宫游戏");
		puts("                   9.退出迷宫程序");
		puts("******************************************************");
		printf("请输入您的选择:");
		char opt;
		fflush(stdin);
		switch (opt=getch())
		{

		case '1': //1.设定迷宫大小
		{
			memset(choice, 0, sizeof(choice));
			choice[1] = 1;
			while (1)
			{
				system("cls");
				fflush(stdin);
				printf("请输入迷宫的行数[5,15]：");
				if (scanf("%d", &M) == 0 || M > 15 || M < 5)
				{
					fflush(stdin);
					puts("输入非法，请重新输入！");
					ch = getch();
					continue;
				}
				break;
			}
			while (1)
			{
				system("cls");
				fflush(stdin);
				printf("请输入迷宫的列数[5,15]：");
				if (scanf("%d", &N) == 0 || N > 15 || N < 5)
				{
					fflush(stdin);
					puts("输入非法，请重新输入！");
					ch = getch();
					continue;
				}
				break;
			}
			system("cls");
			puts("迷宫设置完毕！\n请按任意键返回主菜单");
			start.x = 1;
			start.y = 0;
			end.x = M - 2;
			end.y = N - 1;
			fflush(stdin);
			ch = getch();
			break;
		}
		case '2': //2.自动生成迷宫
		{
			system("cls");
			if (choice[1] == 0)
			{
				fflush(stdin);
				puts("请先设定迷宫大小！");
				ch = getch();
				continue;
			}
			choice[2] = 1;
			choice[3] = 0;
			do
			{
				Random();
				DFS(start, end, &S, 0);
			} while (count == 0); //随机生成迷宫直到迷宫可通
			memset(vis, 0, sizeof(vis));
			puts("迷宫生成成功！");
			puts("随机生成的迷宫为：");
			for (int i = 0; i < M; i++)
			{
				for (int j = 0; j < N; j++)
					if (Maze[i][j] == 1)
						printf("■");
					else
						printf("□");
				puts("");
			}
			count = 0;
			fflush(stdin);
			puts("\n请按任意键返回主菜单\n");
			ch = getch();
			break;
		}
		case '3': //3.文件导入迷宫
		{
			system("cls");
			choice[1] = 1;
			choice[2] = 0;
			FILE *fp;
			if ((fp = fopen("input.txt", "r")) == NULL)
			{
				system("cls");
				fflush(stdin);
				printf("输入文件不存在!\n请创建一个\"input.txt\"文件！\n");
				ch = getch();
				continue;
			}
			fscanf(fp, "%d%d", &M, &N);
			start.x = 1;
			start.y = 0;
			end.x = M - 2;
			end.y = N - 1;
			int flag = 0;
			for (int i = 0; i <= M - 1; i++)
			{
				for (int j = 0; j <= N - 1; j++)
				{
					Maze[i][j] = -1;
					if (fscanf(fp, "%d", &Maze[i][j]) == EOF || (Maze[i][j] != 0 && Maze[i][j] != 1))
						flag = 1;
					if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
						if (Maze[i][j] == 0 && (!((i == 1 && j == 0) || (i == M - 2 && j == N - 1))))
							flag = 2;
				}
				ch = ' ';
				fscanf(fp, "%c", &ch);
				if (i != M - 1 && ch != '\n')
					flag = 3;
			}
			int test = -19195216;
			fscanf(fp, "%d", &test);
			if (test != -19195216 || flag != 0)
			{
				system("cls");
				printf("该迷宫不合法!\n请按下列要求修改txt文件后再读取迷宫\n");
				puts("迷宫第一行输入行和列，中间以空格分隔；");
				puts("用1表示墙，0表示路；");
				puts("列与列之间以空格分隔，行与行之间换行符分隔，每行末尾没有多余空格；");
				puts("迷宫四周除入口出口外没有其他的路；");
				puts("迷宫至少有一条通路；");
				puts("迷宫末尾没有冗余信息。");
				ch = getch();
				continue;
			}
			DFS(start, end, &S, 0);
			if (count == 0)
			{
				fflush(stdin);
				printf("该迷宫无解!!\n请修改您的迷宫！\n");
				ch = getch();
				continue;
			}
			memset(vis, 0, sizeof(vis));
			count = 0;
			fclose(fp);
			puts("迷宫读取成功！");
			puts("输入文件中的迷宫为：");
			for (int i = 0; i < M; i++)
			{
				for (int j = 0; j < N; j++)
					if (Maze[i][j] == 1)
						printf("■");
					else
						printf("□");
				puts("");
			}
			puts("\n请按任意键返回主菜单\n");
			fflush(stdin);
			ch = getch();
			choice[3] = 1;
			break;
		}
		case '4': //4.修改遍历顺序
		{
			system("cls");
			while (1)
			{
				system("cls");
				puts("请输入遍历顺序对应的编号：1.右 2.下 3.左 4.上");

				puts("请输入第一个：");
				fflush(stdin);
				scanf("%d", &dirr[1]);
				if (dirr[1] != 1 && dirr[1] != 2 && dirr[1] != 3 && dirr[1] != 4)
				{
					fflush(stdin);
					puts("输入非法！请再次输入！\n");
					ch = getch();
					continue;
				}
				fflush(stdin);
				puts("请输入第二个：");
				scanf("%d", &dirr[2]);
				if ((dirr[2] != 1 && dirr[2] != 2 && dirr[2] != 3 && dirr[2] != 4) || dirr[2] == dirr[1])
				{
					fflush(stdin);
					puts("输入非法！请再次输入！\n");
					ch = getch();
					continue;
				}
				fflush(stdin);
				puts("请输入第三个：");
				scanf("%d", &dirr[3]);
				if ((dirr[3] != 1 && dirr[3] != 2 && dirr[3] != 3 && dirr[3] != 4) || dirr[3] == dirr[2] || dirr[3] == dirr[1])
				{
					fflush(stdin);
					puts("输入非法！请再次输入！\n");
					ch = getch();
					continue;
				}
				fflush(stdin);
				puts("请输入第四个：");
				scanf("%d", &dirr[4]);
				if ((dirr[4] != 1 && dirr[4] != 2 && dirr[4] != 3 && dirr[4] != 4) || dirr[4] == dirr[3] || dirr[4] == dirr[2] || dirr[4] == dirr[1])
				{
					fflush(stdin);
					puts("输入非法！请再次输入！\n");
					ch = getch();
					continue;
				}
				fflush(stdin);
				break;
			}
			puts("遍历顺序修改成功！");
			puts("\n请按任意键返回主菜单\n");
			ch = getch();
			break;
		}
		case '5': //5.动态显示路径
		{
			system("cls");
			if (choice[1] == 0)
			{
				fflush(stdin);
				puts("请先设定迷宫大小！");
				ch = getch();
				continue;
			}
			if (choice[2] + choice[3] == 0)
			{
				fflush(stdin);
				puts("请先 随机生成 或 读取迷宫！");
				ch = getch();
				continue;
			}
			DFS(start, end, &S, 1);
			count = 0;
			StackClear(&S);
			memset(vis, 0, sizeof(vis));
			puts("动态显示完毕！");
			fflush(stdin);
			puts("\n请按任意键返回主菜单\n");
			ch = getch();
			break;
		}
		case '6': //6.输出所有路径
		{
			system("cls");
			if (choice[1] == 0)
			{
				fflush(stdin);
				puts("请先设定迷宫大小！");
				ch = getch();
				continue;
			}
			if (choice[2] + choice[3] == 0)
			{
				fflush(stdin);
				puts("请先 随机生成 或 读取迷宫！");
				ch = getch();
				continue;
			}

			while (1)
			{
				puts("请输入您要查看路径的数量上限[1,1500]");
				if (!scanf("%d", &maxread) || maxread < 1 || maxread > 1500)
				{
					fflush(stdin);
					puts("输入非法，请重新输入！");
					ch = getch();
					continue;
				}
				fflush(stdin);
				break;
			}

			puts("随机生成的迷宫为：");
			for (int i = 0; i < M; i++)
			{
				for (int j = 0; j < N; j++)
					if (Maze[i][j] == 1)
						printf("■");
					else
						printf("□");
				puts("");
			}
			puts("\n");
			MulDfs(start, end, &S, 0);
			if (count == maxread)
				printf("\n已显示%d条路径，剩余路径省略！\n", maxread);
			count = 0;
			StackClear(&S);
			memset(vis, 0, sizeof(vis));
			fflush(stdin);
			puts("\n请按任意键返回主菜单\n");
			ch = getch();
			break;
		}
		case '7': //7.输出最短路径
		{
			system("cls");
			minlength = (15 + 5) * (15 + 5);
			if (choice[1] == 0)
			{
				fflush(stdin);
				puts("请先设定迷宫大小！");
				ch = getch();
				continue;
			}
			if (choice[2] + choice[3] == 0)
			{
				fflush(stdin);
				puts("请先 随机生成 或 读取迷宫！");
				ch = getch();
				continue;
			}
			while (1)
			{
				fflush(stdin);
				puts("请输入您要查看路径的数量上限[1,1500]");
				if (!scanf("%d", &maxread) || maxread < 1 || maxread > 1500)
				{
					fflush(stdin);
					puts("输入非法，请重新输入！");
					ch = getch();
					continue;
				}
				fflush(stdin);
				break;
			}
			puts("随机生成的迷宫为：");
			for (int i = 0; i < M; i++)
			{
				for (int j = 0; j < N; j++)
					if (Maze[i][j] == 1)
						printf("■");
					else
						printf("□");
				puts("");
			}
			BestDfs(start, end, &S, 0);
			printf("一共有%d条最短路；最短的路径长度为%d。\n", count, minlength);
			if (count > maxread)
				printf("\n已显示%d条最短路，剩余路径省略！\n", maxread);
			count = 0;
			StackClear(&S);
			memset(vis, 0, sizeof(vis));
			fflush(stdin);
			puts("\n请按任意键返回主菜单\n");
			ch = getch();
			break;
		}
		case '8': //8.手动迷宫游戏
		{
			system("cls");
			fflush(stdin);
			if (choice[1] == 0)
			{
				fflush(stdin);
				puts("请先设定迷宫大小！");
				ch = getch();
				continue;
			}
			if (choice[2] + choice[3] == 0)
			{
				fflush(stdin);
				puts("请先 随机生成 或 读取迷宫！");
				ch = getch();
				continue;
			}
			puts("游戏规则：您需要在规定时间内抵达终点，\n如果超时或者陷入死胡同，所有道路将会塌陷，任务失败。\n同时您走过的所有路将会塌陷，无法再次走入。\n请妥善规划好您的前进路线，争取在最短的时间内抵达终点，获得\"★★★\"评价!\n\"○\"为您目前位置；\"●\"为终点位置。\n如果迷宫的行列数都大于10，那么有可能需要等待一段时间\n");
			fflush(stdin);
			ch = getch();
			puts("\n正在生成总时间，请稍等！");
			PosType now = start;
			Maze[now.x][now.y] = 2;
			Maze[end.x][end.y] = 3;
			maxread = 0;
			BestDfs(start, end, &S, 0);
			int Time = minlength + 5;
			count = 0;
			StackClear(&S);
			memset(vis, 0, sizeof(vis));
			printf("您剩余的时间为：%d\n", Time);
			PrintMaze();
			fflush(stdin);
			puts("请键入 W/S/A/D：");
			while ((ch = getch()))
			{
				if (Maze[now.x][now.y + 1] != 0 && Maze[now.x][now.y + 1] != 3)
					if (Maze[now.x + 1][now.y] != 0)
						if (Maze[now.x - 1][now.y] != 0)
							if (Maze[now.x][now.y - 1] != 0)
							{
								puts("您已无路可走，游戏失败！！！");
								break;
							}

				if (Time == 0)
				{
					printf("您已超时，游戏失败！！！\n");
					break;
				}
				switch (ch)
				{
				case 'W':
				case 'w':
				{
					now.x--;
					if (Judge(now))
					{
						Maze[now.x + 1][now.y] = 4;
						Maze[now.x][now.y] = 2;
						system("cls");
						printf("您剩余的时间为：%d\n", --Time);
						PrintMaze();
						break;
					}
					else
					{
						now.x++;
						continue;
					}
				}
				case 'A':
				case 'a':
				{
					now.y--;
					if (Judge(now))
					{
						Maze[now.x][now.y + 1] = 4;
						Maze[now.x][now.y] = 2;
						system("cls");
						printf("您剩余的时间为：%d\n", --Time);
						PrintMaze();
						break;
					}
					else
					{
						now.y++;
						continue;
					}
				}
				case 'S':
				case 's':
				{
					now.x++;
					if (Judge(now))
					{
						Maze[now.x - 1][now.y] = 4;
						Maze[now.x][now.y] = 2;
						system("cls");
						printf("您剩余的时间为：%d\n", --Time);
						PrintMaze();
						break;
					}
					else
					{
						now.x--;
						continue;
					}
				}
				case 'D':
				case 'd':
				{
					now.y++;
					if (Judge(now))
					{
						Maze[now.x][now.y - 1] = 4;
						Maze[now.x][now.y] = 2;
						system("cls");
						printf("您剩余的时间为：%d\n", --Time);
						PrintMaze();
						break;
					}
					else
					{
						now.y--;
						continue;
					}
				}
				default:
					continue;
				}
				if (now.x == end.x && now.y == end.y)
				{
					puts("恭喜您在规定时间内抵达了终点！");
					printf("您获得的评价为：");
					switch (Time)
					{
					case 5:
						puts("★★★ !");
						break;
					case 4:
					case 3:
						puts("☆★★ !");
						break;
					default:
						puts("☆☆★ !");
					}
					break;
				}
				puts("请键入 W/S/A/D：");
			}
			for (int i = 0; i < M; i++)
				for (int j = 0; j < N; j++)
					if (Maze[i][j] >= 2)
						Maze[i][j] = 0;
			fflush(stdin);
			puts("请按任意键返回主菜单");
			ch = getch();
			continue;
		}
		case '9': //9.退出迷宫程序
		{
			system("cls");
			puts("程序已退出！\n感谢您的使用！\n");
			puts("班级：19184115");
			puts("专业:计算机科学与技术（人工智能与大数据）创新实验班");
			puts("洪智豪 19195216");
			fflush(stdin);
			ch = getch();
			return 0;
		}
		default:
			continue;
		}
	}
}