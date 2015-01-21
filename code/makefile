
CC= gcc
MYSQL_CFLAGS= -I/usr/include/mysql -DBIG_JOINS=1 -fno-strict-aliasing -g
MYSQL_LIBS= -L/usr/lib/arm-linux-gnueabihf -lmysqlclient -lpthread -lz -lm -lrt -ldl
main_menu: main_menu.c
	$(CC) -o main_menu $(MYSQL_CFLAGS) main_menu.c $(MYSQL_LIBS)
clean:
	rm -f main_menu
