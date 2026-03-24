#include"library.h"
record* GetRecord()
{
	FILE* file = fopen("D:\\代码\\我的期末作业\\图书馆管理系统\\借书记录.txt", "r");
	if (file!=NULL)
	{
		printf("文件打开失败\n");
	}
	record* head = NULL;
	record* tail = NULL;
	char line[512];
	fgets(line, sizeof(line), file);
	while (fgets(line, sizeof(line), file))
	{
		record* records = (record*)malloc(sizeof(record));
		records->next = NULL;
		if (sscanf(line, "%d|%d|%31[^|]|%d|%31[^|]|%31[^|]|%31[^|]|%d",
			&records->recordid,
			&records->bookid,
			records->name,
			&records->accountid,
			records->time,
			records->ShouldBackTime,
			records->TrueBackTime,
			&records->flag)!=8)
		{
			free(records);
			break;
		}
		if (head == NULL)
		{
			head = records;
			tail = records;
		}
		else
		{
			tail->next = records;
			tail = records;
		}
	}
	return head;
}
void ShowRecord(record* head)
{
	while (head!=NULL)
	{
		printf("流水编号: %d | 图书ID: %d | 书名: %s\n",
			head->recordid, head->bookid, head->name);
		printf("借书人账户ID: %d | 借出时间: %s\n",
			head->accountid, head->time);
		printf("应还时间: %s | 实还时间: %s | 业务标记(0未归还,1正常归还,2逾期且已处理)%d\n",
			head->ShouldBackTime, head->TrueBackTime, head->flag);
		head = head->next;
		printf("\n");
	}
}
enum SearchRecordType GetSearchRecordType(char* input)
{
	if (strcmp(input, "1") == 0) return BY_RECORDID;
	if (strcmp(input, "2") == 0) return BY_BOOKNAME;
	if (strcmp(input, "3") == 0) return BY_USERID;
	if (strcmp(input, "4") == 0) return BY_OUTTIME;
	if (strcmp(input, "4") == 0) return BY_BACKTIME;
	return 0;
}
record* FindRecord(int power,account* sir) 
{
	char input[10];
	enum SearchRecordType type;
	startinput:
	if (power == 2)
	{
		printf("你好管理员\n");
		printf("请输入查找方式:\n1:流水编号范围\n2:书名\n3:用户id\n4:借出日期\n5:归还日期\n");
		scanf("%s", input);
		type = GetSearchRecordType(input);
		if (type == 0)
		{
			printf("输入形式错误，请重新输入:\n");
			goto startinput;
		}
	}
	else
	{
		printf("现在为你展示你的借书记录:\n");
		type = BY_USERID;
	}
	record* head = RecordHead;
	record* result = NULL;
	int IdDown;
	int IdUp;
	char name[32];
	int UserId;
	char OutTime[20];
	char BackTime[20];
	if (type == BY_RECORDID)
	{
		printf("你将输入流水id范围来查询一些借书表的信息，如果你仅想查询其中一条，输入相同的下界和上界即可\n");
		printf("请输入你要查找的id下界:\n");
		scanf("%d", &IdDown);
		printf("请输入你要查找的id上界:\n");
		scanf("%d", &IdUp);
	}
	else if (type == BY_BOOKNAME)
	{
		printf("请输入你要查找记录的书名:\n");
		scanf("%s", name);
	}
	else if (type == BY_USERID)
	{
		if (power == 2)
		{
			printf("请输入你要查找的读者id:\n");
			scanf("%d", &UserId);
		}
		else
		{
			UserId = sir->id;
		}
	}
	else if (type == BY_OUTTIME)
	{
		printf("请输入你要查找的借出时间:(形式如2026.1.7)\n");
		scanf("%s", OutTime);
	}
	else
	{
		printf("请输入你要查找的归还时间:(形式如2026.1.7)\n");
		scanf("%s", BackTime);
	}
	while (head != NULL)
	{
		if (type == BY_RECORDID)
		{
			if (IdDown <= head->recordid && head->recordid<= IdUp)
			{
				record* newnode = (record*)malloc(sizeof(record));
				memcpy(newnode, head, sizeof(record));
				newnode->next = NULL;
				if (result == NULL)
				{
					result = newnode;
				}
				else
				{
					newnode->next = result;
					result = newnode;
				}
			}
		}
		else if (type == BY_BOOKNAME)
		{
			if (!strcmp(name, head->name))
			{
				record* newnode = (record*)malloc(sizeof(record));
				memcpy(newnode, head, sizeof(record));
				newnode->next = NULL;
				if (result == NULL)
				{
					result = newnode;
				}
				else
				{
					newnode->next = result;
					result = newnode;
				}
			}
		}
		else if (type == BY_USERID)
		{
			if (UserId==head->accountid)
			{
				record* newnode = (record*)malloc(sizeof(record));
				memcpy(newnode, head, sizeof(record));
				newnode->next = NULL;
				if (result == NULL)
				{
					result = newnode;
				}
				else
				{
					newnode->next = result;
					result = newnode;
				}
			}
		}
		else if (type == BY_OUTTIME)
		{
			int len = strlen(OutTime);
			char TempOutTime[20];
			strncpy(TempOutTime, head->time, len);
			if (!strcmp(OutTime, TempOutTime))
			{
				record* newnode = (record*)malloc(sizeof(record));
				memcpy(newnode, head, sizeof(record));
				newnode->next = NULL;
				if (result == NULL)
				{
					result = newnode;
				}
				else
				{
					newnode->next = result;
					result = newnode;
				}
			}
		}
		else if (type == BY_BACKTIME)
		{
			int len = strlen(BackTime);
			char TempBackTime[20];
			strncpy(TempBackTime, head->time, len);
			if (!strcmp(OutTime, TempBackTime))
			{
				record* newnode = (record*)malloc(sizeof(record));
				memcpy(newnode, head, sizeof(record));
				newnode->next = NULL;
				if (result == NULL)
				{
					result = newnode;
				}
				else
				{
					newnode->next = result;
					result = newnode;
				}
			}
		}
		head = head->next;
	}
	if (result == NULL)
	{
		printf("该记录不存在\n");
	}
	return result;
}
void StorageRecord()
{
	FILE* file = fopen("D:\\代码\\我的期末作业\\图书馆管理系统\\借书记录.txt", "w");
	fprintf(file, "流水编号 | 图书编号 | 书籍名称 | 借书用户编号 | 借出时间 | 应还时间 | 实际归还时间 | 业务标记:0为未归还，1为正常归还，2为逾期归还\n");
	while (RecordHead!=NULL)
	{
		fprintf(file, "%d|%d|%s|%d|%s|%s|%s|%d\n",
			RecordHead->recordid,
			RecordHead->bookid,
			RecordHead->name,
			RecordHead->accountid,
			RecordHead->time,
			RecordHead->ShouldBackTime,
			RecordHead->TrueBackTime, 
			RecordHead->flag);
		RecordHead = RecordHead->next;
	}
	fclose(file);
}