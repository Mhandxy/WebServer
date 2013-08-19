#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "server.h"

//
const char* program_name;
const char* const short_options = "a:hm:p:v";
const struct option long_options[] = {
    { "address", 1, NULL, 'a' },
    { "help", 0, NULL, 'h' },
    { "modir", 1, NULL, 'm' },
    { "port", 1, NULL, 'p' },
    { "verbose", 0, NULL, 'v' },
    { NULL, 0, NULL, 0 }
};
//全局变量
int verbose = 0;
const char* modir = "lib/";
//打印输出
void PrintUsage (FILE* stream, int exit_code)
{
    fprintf (stream, "Usage: %s options [ inputfile ... ]\n", program_name);
    fprintf (stream,
            " -a --address set address.\n"
            " -h --help    show help.\n"
            " -m --modir   set module directory.\n"
            " -p --port    set port.\n"
            " -v --verbose show verbose messages.\n");
    exit (exit_code);
}

int main(int argc,char* argv[])
{
    //init
    program_name=argv[0];
    int next_option;
    const char* address = "localhost";
    int port = 8008;
    //get parament
    do {
        next_option = getopt_long (argc, argv, short_options, long_options, NULL);
        switch (next_option) {
            case 'a': /* -a or --address */
                address = optarg;
                break;
            case 'h': /* -h or --help */
                PrintUsage (stdout, 0);
            case 'm': /* -m or --modir */
                modir = optarg;
                break;
            case 'p': /* -p or --port */
                port = atoi(optarg);
                break;
            case 'v': /* -v or --verbose */
                verbose = 1;
                break;
            case '?': /* The user specified an invalid option. */
                /* Print usage information to standard error, and exit with exit code one (indicating abnormal termination). */
                PrintUsage (stderr, 1);
            case -1: /* Done with options. */
                break;
            default: /* Something else: unexpected. */
                abort ();
        }
    }
    while (next_option != -1);
    //running
    printf("*****************ServerRunning******************\n");
    printf("address: %s\n",address);
    printf("port:    %d\n",port);
    printf("modir:   %s\n",modir);
    if(verbose)
        printf("verbose: %s\n************************************************\n",(verbose)? "true":"false");
    //transfer localhost to 127.0.0.1
    if(strcmp("localhost",address)==0)
        address="127.0.0.1";
    struct in_addr addr;
    //服务器地址及端口
    addr.s_addr = inet_addr(address);
    server_run(addr,htons(port));
    return 0;
}
