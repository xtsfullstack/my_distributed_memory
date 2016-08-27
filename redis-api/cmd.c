#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "redis_op.h"

int main01(void)
{

	redisContext* conn =  rop_connectdb_nopwd("127.0.0.1", "6379");

	char buf[1][VALUES_ID_SIZE] = { 0 };
	char bub[1][VALUES_ID_SIZE] = { 0 };
	int len = 0;

	strncat(buf[0], "123", 4);

	rop_list_push_append(conn, "mykey", buf, 1);

	rop_range_list(conn, "mykey", 0, 1, bub, &len);

	printf("bub = %s, len = %d\n", bub[0], len);

	rop_disconnect(conn);
	
	return 0;
}

int main(void)
{
	//缓存从redis数据库中取出来的数据
	char bub[5][VALUES_ID_SIZE] = { 0 };
	int len = 0;
	
	//创建连接 redis数据库
	redisContext* conn =  rop_connectdb_nopwd("127.0.0.1", "6379");
	
	//单条数据插入链表
	rop_list_push(conn, "FILE_INFO_LIST", "day05流程.txt");
		
	
	//根据
	rop_range_list(conn, "FILE_INFO_LIST", 0/*其实位置*/, 1/*取出元素个数*/, bub, &len);

	printf("bub = %s, len = %d\n", bub[0], len);

	rop_disconnect(conn);
	
	return 0;
}
