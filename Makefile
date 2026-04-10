all:
	gcc src/main.c src/admin.c src/user.c src/config.c -o app

run:
	./app

clean:
	rm -f app
