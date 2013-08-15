#include "server.h"
#include <dlfcn.h>

/* HTTP response, header, and body template, indicating that the requested document was not found. */
static char* not_found_response_template =
"HTTP/1.0 404 Not Found\n"
"Content-type: text/html\n"
"\n"
"<html>\n"
" <body>\n"
" <h1>Not Found</h1>\n"
" <p>The requested URL was not found on this server.</p>\n"
" </body>\n"
"</html>\n";

//未找到错误
void not_found_response(int client_socket)
{
    send(client_socket,not_found_response_template,
            strlen(not_found_response_template),0);
}
//加载模块
psmodule module_open (const char* module_name)
{
    psmodule m = (psmodule) malloc(sizeof(smodule));
    m->name = module_name;
    char* dir = (char*) malloc(128);
    memset(dir,0,128*sizeof(char));
    //注意modir格式
    if(strlen(dir)==0||dir[strlen(dir)-1]!='/')
        sprintf(dir,"%s/lib%s.so",modir,module_name);
    else 
        sprintf(dir,"%slib%s.so",modir,module_name);
    //加载so
    m->handle = dlopen (dir, RTLD_LAZY);
    if(m->handle == NULL)
        m->generate_function = not_found_response;
    else
        m->generate_function = dlsym(m->handle,"module_generate");
    free(dir);
    return m;
}
//安全释放模块
void module_close (psmodule m)
{
    if(m==NULL) return;
    if(m->handle!=NULL) dlclose(m->handle);
    free(m);
    if(verbose)
        printf("Server >> Done\n");
}

