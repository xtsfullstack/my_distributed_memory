
#include "redis_op.h"

int main(void)
{
	redisContext *c = redisConnect("127.0.0.1", 6379);
	if(c->err)
	{
		redisFree(c);
		printf("Connect to redisServer failed\n");
		return -1;
	}

	printf("Connect to redisServer success!\n");
#if 0
	const char *command1 = "SET key1 test1";

	redisReply *r =  (redisReply*)redisCommand(c, command1);
	if(r == NULL)
	{
		printf("execute command failed!\n");
		redisFree(c);
		return -1;
	}

	if(!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK") == 0))
	{
		printf("failed to execute command[%s]\n", command1);
		freeReplyObject(r);
		redisFree(c);
		return -1;
	}

	printf("succeed to ececute command[%s]\n",	command1);
#endif

	rop_set_key(c, "key1", "test2");

	redisReply* r = (redisReply*)redisCommand(c, "%s %s", "strlen", "key1");
	if(r->type != REDIS_REPLY_INTEGER)
	{
		printf("failed to execute command[%s]", "strlen key1");
		freeReplyObject(r);
		redisFree(c);
		return -1;
	}	

	int length = r->integer;
	freeReplyObject(r);

	printf("the length of 'key1' is %d\n", length);
	printf("succeed to execute command[%s]\n", "strlen key1");

#if 0
	const char *command3 = "get key1";
	r = (redisReply*)redisCommand(c, command3);
	if(r->type != REDIS_REPLY_STRING)
	{
		printf("failed to execute command[%s]", command3);
		freeReplyObject(r);
		redisFree(c);
		return -1;
	}
	
	printf("the value of key1 is %s\n", r->str);
	freeReplyObject(r);
	printf("succeed to execute command[%s]\n", command3);
#endif
	
	char value[1024] = { 0 };
	int valuelen = 0;
	rop_get_key(c, "key1", value, &valuelen);
	printf("we get the value of key1 = %s, len = %d\n", value, valuelen);

	const char * command4 = "get key2";
	r = (redisReply*)redisCommand(c, command4);
	if(r->type != REDIS_REPLY_STRING)
	{
		printf("failed to execute command[%s]\n", command4);
		freeReplyObject(r);
		redisFree(c);
		return -1;
	}

	printf("the value of key2 is %s\n", r->str);
	freeReplyObject(r);
	printf ("succeed to execute command[%s]\n", command4);

	return 0;
}
