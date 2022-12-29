INCLUDE :=-I ./../../include 
LIB_PATH := -L ./../../lib/ -lMySocket -lMySqlite -lsqlite3 -lpthread
OBJ := sqlite_op.c #mysocket.c -lMySqlite

1-client:*.c #$(OBJ)
	gcc $^ -o $@ $(INCLUDE) $(LIB_PATH)



#gcc -shared -fPIC *.c -o libMyServer.so -I ./../../include/
#mv libMyServer.so ./../../lib/
#gcc server.c -L ./../lib/ -lMyServer -lMySocket -lMySqlite -lsqlite3 -lpthread -o 0-server -I ./../include/