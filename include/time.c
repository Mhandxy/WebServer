#include "../server.h"
#include <time.h>

static char* page_begin =
"HTTP/1.0 200 OK\n"
"Content-type: text/html\n"
"\n"
"<html>\n"
" <head>\n"
" <meta http-equiv=\"refresh\" content=\"5\">\n"
" </head>\n"
" <body>\n"
" <p>The current time is ";

static char* page_end =
".</p>\n"
" </body>\n"
"</html>\n";


void module_generate(int client_socket)
{
    time_t now;
    struct tm *timenow;
    time(&now);
    timenow = localtime(&now);
    send(client_socket,page_begin,strlen(page_begin),0);
    //显示时间
    send(client_socket,asctime(timenow),strlen(asctime(timenow)),0);
    send(client_socket,page_end,strlen(page_end),0);
}

