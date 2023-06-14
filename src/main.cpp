#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>//用于函数SetConsoleOutputCP(65001);更改cmd编码为utf8
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//节点结构体
struct LinkNode
{
	//数据域
	void * data;
	//指针域
	struct LinkNode * next;
};

//链表结构体
struct LList
{
	//头节点
	struct LinkNode pHeader;
	//链表长度
	int m_size;
};

typedef void * LinkList;


//初始化链表
LinkList init_LinkList()
{
	 struct LList * myList = (struct LList *)malloc(sizeof(struct LList));

	 if (myList == NULL)
	 {
		 return NULL;
	 }

	 myList->pHeader.data = NULL;
	 myList->pHeader.next = NULL;
	 myList->m_size = 0;

	 return myList;
}

//插入链表
void insert_LinkList(LinkList list, int pos, void * data)
{
	if (list == NULL)
	{
		return;
	}
	if ( data == NULL)
	{
		return;
	}
	//将list还原成 struct LList数据类型
	struct LList * myList = (struct LList *)list;
	if (pos < 0 || pos >myList->m_size)
	{
		//无效位置 强制做尾插
		pos = myList->m_size;
	}

	//找到插入节点的前驱节点位置
	struct LinkNode * pCurrent = &myList->pHeader;

	for (int i = 0; i < pos;i++)
	{
		pCurrent = pCurrent->next;
	}
	//pCurrent 要插入节点的前驱

	//创建新节点

    struct LinkNode * newNode = (struct LinkNode *)malloc(sizeof(struct LinkNode));
	newNode->data = data;
	newNode->next = NULL;

	//建立节点关系
	newNode->next = pCurrent->next;
	pCurrent->next = newNode;

	//更新链表长度
	myList->m_size++;

}

//遍历链表
void foreach_LinkList(LinkList list, void(*myForeach)(void *))
{
	if (list ==NULL)
	{
		return;
	}

	struct LList * mylist = (struct LList *)list;

	struct LinkNode* pCurrent = mylist->pHeader.next;

	for (int i = 0; i < mylist->m_size;i++)
	{
		myForeach(pCurrent->data);
		pCurrent = pCurrent->next;
	}

}


//删除链表  按位置
void removeByPos_LinkList(LinkList list, int pos)
{
	if ( list == NULL)
	{
		return;
	}

	struct LList * mylist = (struct LList *)list;

	if (pos < 0 || pos > mylist->m_size - 1)
	{
		return;
	}

	//找到待删除节点的前驱节点
	struct LinkNode * pCurrent = &mylist->pHeader;
	for (int i = 0; i < pos;i++)
	{
		pCurrent = pCurrent->next;
	}

	//记录待删除的节点
	struct LinkNode * pDel = pCurrent->next;

	//重新建立节点关系
	pCurrent->next = pDel->next;

	free(pDel);
	pDel = NULL;

	//更新链表长度
	mylist->m_size--;
}

//按照值删除链表
void removeByValue_LinkList(LinkList list , void * data ,  int(*myCompare)(void * ,void *) )
{
	if (list == NULL)
	{
		return;
	}
	if ( data == NULL)
	{
		return;
	}

	struct LList * mylist = (struct LList *)list;
	//创建两个辅助指针
	struct LinkNode * pPrev = &mylist->pHeader;
	struct LinkNode * pCurrent = pPrev->next;

	for (int i = 0; i < mylist->m_size;i++)
	{
		//pCurrent->data  data 将两个指针比较利用回调 交给用户
		if (myCompare (pCurrent->data,data))
		{
			pPrev->next = pCurrent->next;

			free(pCurrent);
			pCurrent = NULL;

			mylist->m_size--;
			break;
		}

		//辅助指针后移
		pPrev = pCurrent;
		pCurrent = pCurrent->next;
	}
}

//清空链表
void clear_LinkList(LinkList list)
{
	if (list == NULL)
	{
		return;
	}

	struct LList * mylist = (struct LList *)list;

	struct LinkNode * pCurrent = mylist->pHeader.next;

	for (int i = 0; i < mylist->m_size;i++)
	{
		struct LinkNode * pNext = pCurrent->next;

		free(pCurrent);

		pCurrent = pNext;
	}

	mylist->pHeader.next = NULL;
	mylist->m_size = 0;

}

//返回链表长度
int  size_LinkList(LinkList list)
{
	if (list == NULL)
	{
		return -1;
	}

	struct LList * mylist = (struct LList *)list;

	return mylist->m_size;
}

//销毁链表
void destroy_Linklist(LinkList list)
{
	if (list == NULL)
	{
		return ;
	}

	//清空链表
	clear_LinkList(list);

	free(list);

	list = NULL;

}




//测试 
struct Person
{
	char name[64];
	int age;
};

void myPrintPerson(void * data)
{
	struct Person * p = (struct Person *)data;
	printf("姓名：%s  年龄：%d\n", p->name, p->age);
}

int myComparePerson(void * data1, void *data2)
{
	struct Person * p1 = (struct Person *)data1;
	struct Person * p2 = (struct Person *)data2;

	return strcmp(p1->name, p2->name) == 0 && p1->age == p2->age;
}

void test01()
{
	//准备数据
	struct Person p1 = { "亚瑟", 18 };
	struct Person p2 = { "妲己", 20 };
	struct Person p3 = { "安琪拉", 19 };
	struct Person p4 = { "凯", 21 };
	struct Person p5 = { "孙悟空", 999 };
	struct Person p6 = { "李白", 999 };

	//初始化链表
	LinkList mylist = init_LinkList();

	printf("链表长度为：%d\n", size_LinkList(mylist));

	//插入数据
	insert_LinkList(mylist, 0, &p1);
	insert_LinkList(mylist, 0, &p2);
	insert_LinkList(mylist, -1, &p3);
	insert_LinkList(mylist, 0, &p4);
	insert_LinkList(mylist, 1, &p5);
	insert_LinkList(mylist, 0, &p6);



	// 李白 凯 孙悟空 妲己 亚瑟 安琪拉

	//遍历
	foreach_LinkList(mylist, myPrintPerson);

	printf("链表长度为：%d\n", size_LinkList(mylist));

	//测试删除 
	removeByPos_LinkList(mylist, 4);

	printf("------------------\n");

	foreach_LinkList(mylist, myPrintPerson);
	printf("链表长度为：%d\n", size_LinkList(mylist));

	struct Person p = { "孙悟空", 999 };
	removeByValue_LinkList(mylist, &p, myComparePerson);

	printf("------------------\n");

	foreach_LinkList(mylist, myPrintPerson);
	printf("链表长度为：%d\n", size_LinkList(mylist));

	//测试清空
	clear_LinkList(mylist);

	//返回链表长度
	printf("链表长度为：%d\n", size_LinkList(mylist));

	//销毁
	destroy_Linklist(mylist);
	mylist = NULL;
}

int main(){
	SetConsoleOutputCP(65001);
	test01();

	system("pause");
	return EXIT_SUCCESS;
}