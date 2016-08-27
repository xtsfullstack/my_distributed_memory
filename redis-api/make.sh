#gcc test_main.c redis_op.c make_log.c -lhiredis -I/usr/local/include/hiredis
gcc cmd.c redis_op.c make_log.c -o cmd -lhiredis -I/usr/local/include/hiredis

