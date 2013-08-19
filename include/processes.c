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

char* lt="&lt;";
void module_generate(int client_socket)
{
    char response;
    //查看所有进程
    FILE* stream = popen("ps aux", "r");
    send(client_socket,page_start,strlen(page_start),0);
    response=fgetc(stream);
    while (!feof(stream))
    {
        if(response!='<')
            send(client_socket,&response,sizeof(char),0);
        else
            send(client_socket,lt,4*sizeof(char),0);
        response=fgetc(stream);
    }
    send(client_socket,page_end,strlen(page_end),0);
    pclose(stream);
}

