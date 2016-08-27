gcc upload.c -o upload -lfcgi
spawn-fcgi -a 127.0.0.1 -p 8083 -f ./upload
