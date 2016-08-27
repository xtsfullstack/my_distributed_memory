gcc upload.c redis_op.c make_log.c -o upload -lfcgi -lhiredis -I/usr/local/include/hiredis
spawn-fcgi -a 127.0.0.1 -p 8083 -f ./upload
