#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef struct server_module { 
    void* handle; 
    const char* name; 
    void (*generate_function) (int); 
}smodule,*psmodule;
//全局函数
extern void server_run (struct in_addr local_address, uint16_t port);
extern psmodule module_open (const char* module_name);
extern void module_close (psmodule module);
//全局变量
extern int verbose;
extern const char* modir;
