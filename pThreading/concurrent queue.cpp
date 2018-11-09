#define _CRT_SECURE_NO_WARNINGS
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <string.h>
#include <windows.h>
/*-----------------------------------------------------------*/
//My defined Data structure
template <class T>
class  dynamicArray
{
public:

	typedef T * iterator;

	dynamicArray()
	{
		my_capacity = 0;
		my_size = 0;
		buffer = 0;
	}
	dynamicArray(unsigned int size)
	{
		my_capacity = size;
		my_size = size;
		buffer = new T[size];
	}
	dynamicArray(unsigned int size, const T & initial)
	{
		my_size = size;
		my_capacity = size;
		buffer = new T[size];
		for (unsigned int i = 0; i < size; i++)
			buffer[i] = initial;
	}
	dynamicArray(const dynamicArray<T> & v)
	{
		my_size = v.my_size;
		my_capacity = v.my_capacity;
		buffer = new T[my_size];
		for (unsigned int i = 0; i < my_size; i++)
			buffer[i] = v.buffer[i];
	}
	~dynamicArray()
	{
		delete[] buffer;
	}

	unsigned int capacity() const
	{
		return my_capacity;
	}
	unsigned int size() const
	{
		return my_size;
	}
	iterator begin()
	{
		return buffer;
	}
	iterator end()
	{
		return buffer + size();
	}
	T & front()
	{
		return buffer[0];
	}
	T & back()
	{
		return buffer[my_size - 1];
	}
	void push_back(const T & v)
	{
		if (my_size >= my_capacity)
			reserve((my_capacity + 1) * 2);
		buffer[my_size++] = v;
	}
	void pop_back()
	{
		my_size--;
	}
	void reserve(unsigned int capacity)
	{
		if (buffer == 0)
		{
			my_size = 0;
			my_capacity = 0;
		}
		T * Newbuffer = new T[capacity];
		unsigned int l_Size = capacity < my_size ? capacity : my_size;

		for (unsigned int i = 0; i < l_Size; i++)
			Newbuffer[i] = buffer[i];

		my_capacity = capacity;
		delete[] buffer;
		buffer = Newbuffer;
	}
	void resize(unsigned int size)
	{
		reserve(size);
		my_size = size;
	}
	T & operator[](unsigned int index)
	{
		return buffer[index];
	}
	dynamicArray<T> & operator=(const dynamicArray<T> & v)
	{
		delete[] buffer;
		my_size = v.my_size;
		my_capacity = v.my_capacity;
		buffer = new T[my_size];
		for (unsigned int i = 0; i < my_size; i++)
			buffer[i] = v.buffer[i];
		return *this;
	}
	void clear()
	{
		my_capacity = 0;
		my_size = 0;
		buffer = 0;
	}
private:
	unsigned int my_size;
	unsigned int my_capacity;
	T * buffer;
};
/*------------------------------------------------------------------------*/
//Globals
dynamicArray<dynamicArray<char *>> userProfiles, posts; // will be removed
dynamicArray<char *> userColums, postsColums;
dynamicArray<int> userIDs, postID;
/*-------------------------------------------------------------------------*/
void tokenizer(char line[], dynamicArray<char *>& tokens)
{
	char * token = (char *)malloc(30 * sizeof(char));
	int k = 0;
	token[0] = '\0';
	int i = 0;
	while (line[i] == ' ')
		i++;
	for (; line[i] != '\n' && line[i] != '\0'; i++)
	{
		if (line[i] == ' ' || line[i] == '\t' || line[i] == ',')
		{
			if (token[0] != '\0')
			{
				token[k] = '\0';
				tokens.push_back(token);
				token = (char *)malloc(30 * sizeof(char));
				token[0] = '\0';
				k = 0;
			}
			else
				continue;
		}
		else
		{
			token[k++] = line[i];
		}
	}
	if (token[0] != '\0')
	{
		token[k] = '\0';
		tokens.push_back(token);
	}
}

int searchById(dynamicArray<int> ids, int id)
{
	for (int i = 0; i<ids.size(); i++)
	{
		if (ids[i] == id)
			return i;
	}
	return -1;
}

int compareStrings(char *str1, char *str2)
{
	int i;
	for (i = 0; str1[i] != '\0'; i++)
	{
		if (str1[i] != str2[i])
			return 0;
	}
	if (str1[i] == str2[i])
		return 1;
	return 0;
}
int searchByColumn(dynamicArray<char *>tableColumns, char * str)
{
	for (int i = 0; i<tableColumns.size(); i++)
	{
		if (compareStrings(tableColumns[i], str) == 1)
			return i;
	}
	return -1;
}

int maxi(dynamicArray<int>& arr)
{
	int m = arr[0];
	for (int i = 1; i<arr.size(); i++)
		if (m<arr[i])
			m = arr[1];
	return m;
}
/*------------------------------------------------------------------------------*/

void insertIntoUDB(int id, dynamicArray<char *>& newColumns, dynamicArray<char *>& values)
{
	userIDs.push_back(id);
	dynamicArray<char *> newMember;
	for (int i = 0; i < userColums.size(); i++)
	{
		char * tmp = (char *)malloc(sizeof(char) * 10);
		tmp[0] = '\0';
		newMember.push_back(tmp);
	}

	for (int i = 0; i < newColumns.size(); i++)
	{
		int col = searchByColumn(userColums, newColumns[i]);
		if (col == -1)
		{
			printf("\nInvalid Column name\n");
			return;
		}
		newMember[col] = values[i];
	}
	userProfiles.push_back(newMember);
	return;
}
void updateUDB(int ind, dynamicArray<char *>& newColumns, dynamicArray<char *>& values)
{
	for (int i = 0; i<newColumns.size(); i++)
	{
		int col = searchByColumn(userColums, newColumns[i]);
		if (col == -1)
		{
			printf("\nInvalid Column name\n");
			return;
		}
		userProfiles[ind][col] = values[i];
	}
}

void putUDB(int id,  dynamicArray<char *>& newColumns, dynamicArray<char *>& values)
{
	printf("\nPut method call\n");
	int ind = searchById(userIDs, id);
	if (ind == -1)
	{
		insertIntoUDB(id, newColumns, values);
		return;
	}
	
	updateUDB(ind, newColumns, values);
	return;
}

void getUDB(int id)
{
	int ind = searchById(userIDs, id);
	if (ind == -1)
	{
		printf("\nID not found\n");
		return;
	}
	for (int i = 0; i < userColums.size(); i++)
	{
		if (userProfiles[ind][i][0] != '\0')
			printf("%s  ", userProfiles[ind][i]);
		else
			continue;
	}
	printf("\n");
}

void deleteRowUDB(int id)
{
	int ind = searchById(userIDs, id);
	if (ind == -1)
	{
		printf("\nID not found\n");
		return;
	}
	for (int i = ind; i<userProfiles.size() - 1; i++)
	{
		userProfiles[i] = userProfiles[i + 1];
		userProfiles[i] = userProfiles[i + 1];
	}
	userProfiles.pop_back();
}

void updateUsersProfiles()
{
	printf("\nUser Menu:\n");
	printf("\n1.get\n2.put\n3.Delete\n");
	int op;
	scanf("%d", &op);
	if (op == 1)
	{
		int id;
		printf("\nEnter the ID:\n");
		scanf("%d", &id);
		getUDB(id);
	}
	else if (op == 2)
	{
		printf("\nEnter the id: ");
		int id;
		scanf("%d", &id);
		fflush(stdin);
		printf("\nEnter the column names:\n");
		char line[100];
		scanf("%s", line);
		dynamicArray<char *>tokens;
		dynamicArray<char *> values;
		tokenizer(line, tokens);
		printf("\nEnter their values:\n");
		scanf("%s", line);
		tokenizer(line, values);
		putUDB(id, tokens, values);
	}
	else if (op == 3)
	{
		int id;
		scanf("%d", &id);
		deleteRowUDB(id);
	}
}
/*---------------------------------------------------------------------------------------*/
void insertIntoPDB(int id, dynamicArray<char *>& newColumns, dynamicArray<char *>& values)
{
	postID.push_back(id);
	dynamicArray<char *> newMember;
	for (int i = 0; i < postsColums.size(); i++)
	{
		char * tmp = (char *)malloc(sizeof(char) * 10);
		tmp[0] = '\0';
		newMember.push_back(tmp);
	}

	for (int i = 0; i < newColumns.size(); i++)
	{
		int col = searchByColumn(postsColums, newColumns[i]);
		if (col == -1)
		{
			printf("\nInvalid Column name\n");
			return;
		}
		newMember[col] = values[i];
	}
	posts.push_back(newMember);
	return;
}
void updatePDB(int ind, dynamicArray<char *>& newColumns, dynamicArray<char *>& values)
{
	for (int i = 0; i<newColumns.size(); i++)
	{
		int col = searchByColumn(postsColums, newColumns[i]);
		if (col == -1)
		{
			printf("\nInvalid Column name\n");
			return;
		}
		posts[ind][col] = values[i];
	}
}

void putPDB(int id, dynamicArray<char *>& newColumns, dynamicArray<char *>& values)
{
	
	int ind = searchById(postID, id);
	if (ind == -1)
	{
		insertIntoPDB(id, newColumns, values);
		return;
	}

	updatePDB(ind, newColumns, values);
	return;
}

void getPDB(int id)
{
	int ind = searchById(postID, id);
	if (ind == -1)
	{
		printf("\nID not found\n");
		return;
	}
	for (int i = 0; i < postsColums.size(); i++)
	{
		if (posts[ind][i][0] != '\0')
			printf("%s  ", posts[ind][i]);
		else
			continue;
	}
}

void deleteRowPDB(int id)
{
	int ind = searchById(postID, id);
	if (ind == -1)
	{
		printf("\nID not found\n");
		return;
	}
	for (int i = ind; i<posts.size() - 1; i++)
	{
		posts[i] = posts[i + 1];
		posts[i] = posts[i + 1];
	}
	posts.pop_back();
}

void updatePosts()
{
	printf("\Posts Menu:\n");
	printf("\n1.get\n2.put\n3.Delete\n");
	int op;
	scanf("%d", &op);
	if (op == 1)
	{
		int id;
		printf("\nEnter the ID:\n");
		scanf("%d", &id);
		getPDB(id);
	}
	else if (op == 2)
	{
		printf("\nEnter the id: ");
		int id;
		scanf("%d", &id);
		fflush(stdin);
		printf("\nEnter the column names:\n");
		char line[100];
		scanf("%s", line);
		dynamicArray<char *>tokens;
		dynamicArray<char *> values;
		tokenizer(line, tokens);
		printf("\nEnter their values:\n");
		scanf("%s", line);
		tokenizer(line, values);
		putPDB(id, tokens, values);
	}
	else if (op == 3)
	{
		int id;
		scanf("%d", &id);
		deleteRowPDB(id);
	}
}
//DB module end
/*---------------------------------------------------------------------*/
//API's
void get(int tableID, int id)
{
	printf("\n------------------In get id = %d ------------------------------\n", id);
	if (tableID == 1)
	{
		getUDB(id);
	}
	else
	{
		getPDB(id);
	}
}
void put(int tableID, int id, char columns[], char values[])
{
	printf("\n------------------In put id = %d ------------------------------\n", id);
	if (tableID == 1)
	{
		dynamicArray<char *>newColumns, newValues;
		tokenizer(columns, newColumns);
		tokenizer(values, newValues);
		putUDB(id, newColumns, newValues);
	}
	else
	{
		dynamicArray<char *>newColumns, newValues;
		tokenizer(columns, newColumns);
		tokenizer(values, newValues);
		putUDB(id, newColumns, newValues);
		putPDB(id, newColumns, newValues);
	}
}

void deleteRow(int tableID, int id)
{
	if (tableID == 1)
	{
		deleteRowUDB(id);
	}
	else
	{
		deleteRowPDB(id);
	}
}
/*------------------------------------------------------------------------------*/

#define QUEUE_MAX_SIZE 15
#define NUMBER_OF_THREADS 4
pthread_rwlock_t rwLock = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t queueLock = PTHREAD_MUTEX_INITIALIZER;

typedef struct clintNode
{
	int id;
	int type;
	int tableId;
	char colums[50];
	char values[50];
};

typedef struct queueNode
{
	clintNode data;
	struct queueNode* next;
};

typedef struct threadData
{
	int flag;
	clintNode data;
};

typedef struct ConcurrentQueue
{
	queueNode * start, * end;
	threadData thread[NUMBER_OF_THREADS];
};

ConcurrentQueue concurentQueue;

void * threadMethod(void * args)
{
	int id = *((int *)args);
	while (1)
	{
		if (concurentQueue.thread[id].data.type != 0)
		{
			if (concurentQueue.thread[id].data.type == 1)
			{
				pthread_rwlock_rdlock(&rwLock);
				get(concurentQueue.thread[id].data.tableId, concurentQueue.thread[id].data.id);
				pthread_rwlock_unlock(&rwLock);
			}
			else if(concurentQueue.thread[id].data.type == 2)
			{
				pthread_rwlock_wrlock(&rwLock);
				put(concurentQueue.thread[id].data.tableId, concurentQueue.thread[id].data.id, concurentQueue.thread[id].data.colums, concurentQueue.thread[id].data.values);
				pthread_rwlock_unlock(&rwLock);
			}
			else
			{
				pthread_rwlock_wrlock(&rwLock);
				deleteRow(concurentQueue.thread[id].data.tableId, concurentQueue.thread[id].data.id);
				pthread_rwlock_unlock(&rwLock);
			}
			concurentQueue.thread[id].flag = 0;
			concurentQueue.thread[id].data.type = 0;
		}
	}
	return NULL;
}

void init()
{
	pthread_t id[NUMBER_OF_THREADS];
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		int * j = (int *)malloc(sizeof(int));
		j[0] = i;
		concurentQueue.thread[i].flag = 0;
		pthread_create(&id[i], NULL, threadMethod, (void *)j);
	}
}

void * schedular(void * args)
{
	while (1)
	{
		while(concurentQueue.start == NULL)
			sched_yield();
		pthread_mutex_lock(&queueLock);
		for (int i = 0; i < NUMBER_OF_THREADS; i++)
		{
			if (concurentQueue.thread[i].flag == 0)
			{
				concurentQueue.thread[i].data = concurentQueue.start->data;
				if (concurentQueue.start == concurentQueue.end)
				{
					concurentQueue.start = NULL;
					concurentQueue.end = NULL;
				}
				else
					concurentQueue.start = concurentQueue.start->next;
				break;
			}
		}
		pthread_mutex_unlock(&queueLock);
	}
	return NULL;
}

void addToQueue(clintNode node)
{
	pthread_mutex_lock(&queueLock);
	if (concurentQueue.start == concurentQueue.end)
	{
		queueNode * newNode = (queueNode *)malloc(sizeof(queueNode));
		newNode->data = node;
		newNode->next = NULL;
		if (concurentQueue.start == NULL)
		{
			concurentQueue.start = newNode;
			concurentQueue.end = newNode;
		}
		else
		{
			concurentQueue.end->next = newNode;
			concurentQueue.end = newNode;
		}
	}
	pthread_mutex_unlock(&queueLock);
}

void * clinet(void * args)
{
	for (int i = 0; i < 100; i++)
	{
		Sleep(100);
		clintNode putNode;
		strcpy(putNode.colums, "emailId name mobile dob profileLink followers");
		strcpy(putNode.values, "syed@ azhar 9515 98 link many");
		putNode.type = 2;
		putNode.tableId = 1;
		putNode.id = i;
		addToQueue(putNode);
		Sleep(100);
		clintNode getNode;
		getNode.type = 1;
		getNode.tableId = 1;
		getNode.id = i;
		addToQueue(getNode);
	}
	return NULL;
}

int main()
{
	char schema[100] = "emailId name mobile dob profileLink followers";
	tokenizer(schema, userColums);
	char pschema[200] = "userID text photoLink command views likes shares hashTags";
	tokenizer(pschema, postsColums);
	init();
	pthread_t id1, id2;
	pthread_create(&id1, NULL, schedular, NULL);
	pthread_create(&id2, NULL, clinet, NULL);
	pthread_join(id2, NULL);
	_getch();
	return 0;
}
