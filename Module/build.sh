gcc -I/usr/include/luajit-2.1 -D"COMPAT53_PREFIX=LUA_COMPAT" -fPIC -shared StructBind.c -o StructBind.so compat-5.3.o
