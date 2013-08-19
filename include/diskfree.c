#include "../server.h"

static char* page_start = 
"HTTP/1.0 200 OK\n"
"Content-type: text/html\n"
"\n"
"<html>\n"
" <head>\n"
" <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"
" </head>"
" <body>\n"
" <pre>\n";

static char* page_end =
" </pre>\n"
" </body>\n"
"</html>\n";


void module_generate(int client_socket)
{
    int buffer_len=128;
    char* response = (char*) malloc(buffer_len);
    //查看空闲磁盘空间信息
    FILE* stream = popen("df", "r");
    send(client_socket,page_start,strlen(page_start),0);
    while (!feof(stream))
    {
        memset(response,0,buffer_len*sizeof(char));
        fgets(response, buffer_len, stream);
        send(client_socket,response,strlen(response),0);
    }
    send(client_socket,page_end,strlen(page_end),0);
    pclose(stream);
    free(response);
}

