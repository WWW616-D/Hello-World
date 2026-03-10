#include"works.h"
int main()
{
	a:
	printf("请输入你要看的项目:\n24点:1\n八皇后:2\n迷宫:3\n文本词频统计并更改:4\n");
	int choice;
	scanf("%d", &choice);
	switch(choice)
	{
		case 1:
			TwentyFourPoint();
			break;
		case 2:
			EightQueensShow();
			break;
		case 3:
			MazeShow();
			break;
		case 4:
			WordsCountAndChange();
			break;	
		default:
			printf("这是一个无效的选择\n");
			break;
	}
	printf("输入1继续观看,输入0退出:\n");
	int next;
	scanf("%d", &next);
	system("cls");
	if (next)
		goto a;
	return 0;
}
