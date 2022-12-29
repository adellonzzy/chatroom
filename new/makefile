all:0-server 1-client
INCLUDE :=-I ./../include 
LIB_PATH := -L ./../lib/ -lMySocket -lMySqlite -lsqlite3 -lpthread
OBJ := sqlite_op.c #mysocket.c -lMySqlite
0-server:0-server.c server.c #$(OBJ)
	gcc $^ -o $@ $(INCLUDE) $(LIB_PATH)
1-client:1-client.c #$(OBJ)
	gcc $^ -o $@ $(INCLUDE) $(LIB_PATH)



