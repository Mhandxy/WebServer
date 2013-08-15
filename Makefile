server: main.c server.h server.c common.c module.c module
	gcc -o server main.c server.c common.c module.c -ldl

module: libtime libdiskfree libissue libprocesses
	@echo "modules complete"

libtime: libdir
	@gcc -shared -fPIC include/time.c      -o lib/libtime.so
libdiskfree: libdir
	@gcc -shared -fPIC include/diskfree.c  -o lib/libdiskfree.so
libissue: libdir
	@gcc -shared -fPIC include/issue.c     -o lib/libissue.so
libprocesses: libdir
	@gcc -shared -fPIC include/processes.c -o lib/libprocesses.so

libdir:
	@mkdir -p lib
clean:
	@rm -f server
test: server
	./server -v 
