#include "server.h"
#include <signal.h>

/* HTTP response, header, and body template, indicating that the method was not understood. */
static char* bad_method_response_template =
"HTTP/1.0 501 Method Not Implemented\n"
"Content-type: text/html\n"
"\n"
"<html>\n"
" <body>\n"
" <h1>Method Not Implemented</h1>\n"
" <p>The method %s is not implemented by this server.</p>\n"
" </body>\n"
"</html>\n";

void server (int client_socket)
{
    //request
    int buffer_len=1024;
    char* request = (char*) malloc(buffer_len);
    memset(request, 0, buffer_len*sizeof(char));
    read (client_socket, request, buffer_len);
    //method&&module
    char* method = (char*) malloc(8);
    char* URL = (char*) malloc(128);
    sscanf(request, "%s %s ", method, URL);
    char* module_name = URL+1;
    //GET
    if(strcmp(method,"GET")==0)
    {
        if(!strcmp(module_name,"favicon.ico")) return;//浏览器会自动请求favicon.ico的图标
        //详细输出
        if(verbose)
            printf("Server >> Request:\n%s\nServer >> Module: %s\n",request, module_name);
        psmodule m=module_open(module_name);
        (*m->generate_function)(client_socket);
        module_close(m);
        if(verbose)
            printf("************************************************\n");
    }
    else//处理非get请求
    {
        //详细输出
        if(verbose)
        {
            printf("%s\nServer >> %s is not implemented\n************************************************\n",
                    request, method);
        }
        //response
        char* response = (char*) malloc(buffer_len);
        memset(response, 0, buffer_len*sizeof(char));
        sprintf(response,bad_method_response_template,method);
        send(client_socket,response,strlen(response),0);
        free(response);
    }
    free(request);
    free(method);
    free(URL);
}

void server_run (struct in_addr local_address, uint16_t port)
{
    /* Create the socket. */
    int server_socket_fd;
    server_socket_fd = socket (PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_add;
    server_add.sin_family = AF_INET;
    server_add.sin_addr = local_address;
    server_add.sin_port = port;
    /* Indicate that this is a server. */
    bind(server_socket_fd,(struct sockaddr*)&server_add, sizeof(server_add));

    /* Listen for connections. */
    listen (server_socket_fd,5);
    /* 防止僵尸进程 */
    signal(SIGCHLD,SIG_IGN);
    /* Repeatedly accept connections, spinning off one server() to deal with each client. Continue until a client sends a “quit” message. */
    while(1){
        int client_socket_fd;
        struct sockaddr_in client_add;
        socklen_t client_len = sizeof(client_add);
        /* Accept a connection. */
        client_socket_fd = accept (server_socket_fd,(struct sockaddr*)&client_add, &client_len);
        /* Handle the connection. */
        pid_t  child_pid=0;
        child_pid = fork ();
        if (child_pid != 0)
        {
            close (client_socket_fd);
        }
        else 
        {
            server (client_socket_fd);
            /* Close our end of the connection. */
            close (client_socket_fd);
            exit(0);
        }
    }
    /* Remove the socket file. */
    close (server_socket_fd);
}

