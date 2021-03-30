/* $begin shellmain */
//premade code and csapp.h from Computer Systems: A Programmer's Perspective

#include "csapp.h"
#define MAXARGS   128
#include <string>
#include <regex>
#include <iostream>

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);
char *getcwd(char *buf, size_t size);
void cry (int signal_num);
pid_t pidd[5] = {0,0,0,0,0};

using namespace std;

int main(int argc, char **argv)
{
    char cmdline[MAXLINE]; /* Command line */
    char cwd[MAXLINE];

    while (1) {
        /* Read */
        printf("$ ");
        fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin))
            exit(0);

        /* Evaluate */
        eval(cmdline);
    }
}
/* $end shellmain */

void cry(int signal_num){
    int status;
    //printf("MADE IT IN\n");
    for (int i=0; i<5; i++) {
        //printf("pid: %d\n", pidd[i]);
        if(pidd[i] != 0) {
            int result = waitpid(pidd[i], &status, WNOHANG);
            if (result < 0)
                printf("waitpid error\n");
            else if (result > 0) {
                pidd[i] = 0;
                //printf("successful kill\n");
            }
            else if (result == 0){
                //printf("there is a runninf child still\n");
                //child did not exit because it was not done
            }
        }
    }
    return;
}


int redirection_lt(char *argv[], char *cmdline){ //input
    char * filename;
    int fn;

    *std::remove(cmdline, cmdline+strlen(cmdline), '\n') = '\0';
    char *myargs[10];
    int i = 0;
    int j = 0;

    for(int i=0; i <= 10; i++) {
        myargs[i] = argv[i];
    }

    for(i = 0; myargs[i] != NULL; i++) {
        if(strcmp(myargs[i], "<") == 0) {

            for(j = i; myargs[j-1] != NULL; j++) {
                myargs[j] = myargs[j+1];

            }
        break;
        }
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(stderr,"fork failed\n");
        exit(1);
    }
    else if (pid == 0) { //child
        
        if (execvp(argv[0], myargs) < 0) {
            printf("execve error\n");
            printf("errno: %d\n", errno);
            exit(1);
        }
    }
    else{
        int status;
        if (waitpid(pid, &status, 0) < 0){
            printf("waitpid error");
        }
    }
    return 0;
}



int redirection_gt(char *argv[], char *cmdline){
    char * filename;
    int fn;

    *std::remove(cmdline, cmdline+strlen(cmdline), '\n') = '\0';
    char *myargs[10];
    int i = 0;
    int j = 0;

    for(int i=0; i <= 10; i++) {
        myargs[i] = argv[i];
    }


    for(i = 0; myargs[i] != NULL; i++) {
        if(strcmp(myargs[i], "<") == 0) {

            for(j = i; myargs[j-1] != NULL; j++) {
                myargs[j] = myargs[j+1];

            }
            break;
        }
    }
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr,"fork failed\n");
        exit(1);
    }
    else if (pid == 0) { //child


       
        if (execvp(argv[0], myargs) < 0) {
            printf("execve error\n");
            printf("errno: %d\n", errno);
            exit(1);
        }

    }
    else{
        int status;
        if (waitpid(pid, &status, 0) < 0){
            printf("waitpid error");
        }
    }



    return 0;
}

int pipe_attempt(char *argv[], char *cmdline){
    *std::remove(cmdline, cmdline+strlen(cmdline), '\n') = '\0';
    char *myargs1[10];
    char *myargs2[10];
    char *myargs3[10];

    int i = 0;
    int j = 0;

    for(int i=0; i <= 10; i++) {
        myargs1[i] = argv[i];
    }
    i =0;
    for(int i=0; i <= 10; i++) {
        myargs2[i] = argv[i];
    }
    for(int k=0; k <= 10; k++) {
        myargs3[k] = nullptr;
    }

    for(i = 0; myargs1[i] != NULL; i++) {
        if(strcmp(myargs1[i], "|") == 0) {

            for(int ii = i; ii <= 10; ii++) {
                myargs1[ii] = nullptr;
            }
            break;
        }
    }
    i = 1;
    j = 0;
    int n = 0;
    for(i = 1; myargs2[i] != NULL; i++) {
        if(strcmp(myargs2[i], "|") == 0) {
            for(int ii = i; ii >= 0; ii--) {
                myargs2[ii] = nullptr;
            }
            for(int i=0; i <= 10; i++) {
                if(myargs2[i] != NULL) {
                    myargs3[n] = argv[i];
                    n++;
                }
            }
            break;
        }
    }

    int pipee[2];
    pid_t pid1, pid2;
    if(pipe(pipee) < 0){
     printf("pipe failed ");
    }


    pid1 = fork();
    if (pid1 < 0) {
        fprintf(stderr,"fork failed\n");
        exit(1);
    }
    else if (pid1 == 0) { //child
        close(pipee[0]);
        dup2(pipee[1], STDOUT_FILENO);
        close(pipee[1]);

        if (execvp(myargs1[0], myargs1) < 0) {
            printf("execve error\n");
            printf("errno: %d\n", errno);
            exit(1);
        }

    }
    else{

        pid2 = fork();
        if (pid2 < 0) {
            fprintf(stderr,"fork failed\n");
            exit(1);
        }
        else if (pid2 == 0) { //child
            close(pipee[1]);
            dup2(pipee[0], STDIN_FILENO);
            close(pipee[0]);

            if (execvp(myargs3[0], myargs3) < 0) {
                printf("execve error\n");
                printf("errno: %d\n", errno);
                exit(1);
            }

        }
        else {

            int status;
            close(pipee[1]);
            if (waitpid(pid1, &status, 0) < 0) {
                printf("waitpid error");
            }
            if (waitpid(pid2, &status, 0) < 0) {
                printf("waitpid error");
        }
           // printf("escaped");
        }
    }

    return 0;
}
/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline)
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    char buff[MAXLINE];
    std::string path_copy;

    path_copy = getenv("PATH");
    //path_copy = "/bin:/usr/bin";
    path_copy = "/usr/bin";
    //path_copy = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin";

    std::string new_path;
    std::string path_copyy;
    path_copyy = path_copy;
    std::string command_path;


    //char * argv_copy = strcpy(argv_copy,argv[0]);     //THIS CAUSES A SEG FAULT :(
    char * new_argv;
    char result[MAXLINE];
    int pos = 0;
    strcpy(buf, cmdline);
    bg = parseline(buf, argv);
    if (argv[0] == NULL)
        return;   /* Ignore empty lines */
    regex b("\\.\\/.+|\\.\\.\\/.+|\\/.+|.+\\/.+");
    regex c("\\.\\>.+|\\.\\.\\>.+|\\>.+|.+\\>.+");
    regex d("\\.\\<.+|\\.\\.\\<.+|\\<.+|.+\\<.+");
    regex p("\\.\\|.+|\\.\\.\\|.+|\\|.+|.+\\|.+");
    if(regex_search(cmdline,p) == true){
        pipe_attempt(argv, cmdline);
        return;
    }

    if(regex_search(cmdline,c) ==  true){
        //printf("match\n");
        if(regex_search(cmdline,d) ==  true){
            return;
        }
        redirection_gt(argv, cmdline);
        return;
    }
    if(regex_search(cmdline,d) ==  true){
        //printf("match\n");
        if(regex_search(cmdline,c) ==  true){
            return;
        }
        redirection_lt(argv, cmdline);
        return;
    }

    if (!regex_match(argv[0],b)){
        //printf("matched");


        while(!path_copy.empty()){

            signal(SIGCHLD, cry);

            const auto p = path_copy.find(':');

            //printf("find value: %d \n", path_copyy.find(":", pos) );
            new_path = "";
            new_path = path_copyy.substr(pos,(path_copyy.find(":", pos)-pos) );
            path_copy = path_copy.substr(path_copy.find(":") + 1 );
            //pos = /*pos + */new_path.length() +1;
            pos = path_copyy.find(":", pos)+1;
            //printf("POS: %d \n",pos);


            if(p == std::string::npos){
                //new_path = path_copyy.substr(pos, string::npos);
                command_path = new_path + "/" + argv[0];
                //std::cout << command_path << "\n";

                char *cstr = &command_path[0];
                if (!builtin_command(argv)) {
                    if ((pid = fork()) == 0) {   /* Child runs user job */
                        if (execve(cstr, argv, environ) < 0) {
                            if(errno == EACCES){
                                printf("permission denied\n");
                            }
                            else if(errno == ENOEXEC || errno == EPERM || errno == EINVAL ){
                                printf("exec format error\n");
                            }
                            else {
                                printf("%s: Command not found.\n", argv[0]);
                                //printf("errno %d", errno);
                            }
                            exit(0);
                        }


                    }

                    /* Parent waits for foreground job to terminate */
                    if (!bg) {
                        int status;
                        if (waitpid(pid, &status, 0) < 0)
                            //unix_error("waitfg: waitpid error");
                            printf("waitpid error");
                    }
                    else {
                        //printf("%d %s\n", pid, cmdline);
                        for (int i = 0; i < 5; i++) {
                            if (pidd[i] == 0) {
                                //printf("PID?: %d\n", pid);
                                pidd[i] = pid;
                                break;
                            }
                        }
                    }
                }



                break;
            }


            command_path = new_path + "/" + argv[0];
            //std::cout << command_path << "\n";

            char *cstr = &command_path[0];
            if (!builtin_command(argv)) {
                if ((pid = fork()) == 0) {   /* Child runs user job */

                    if (execve(cstr, argv, environ) < 0) {
                        if(errno == EACCES){
                            printf("permission denied");
                        }
                        else if(errno == ENOEXEC || errno == EPERM || errno == EINVAL ){
                            printf("exec format error");
                        }
                        else {
                            printf("%s: Command not found.\n", argv[0]);
                            printf("errno %d", errno);
                        }                        exit(0);
                    }

                }

                /* Parent waits for foreground job to terminate */
                if (!bg) {
                    int status;
                    if (waitpid(pid, &status, 0) < 0)
                        //unix_error("waitfg: waitpid error");
                        printf("waitpid error");
                }
                else {
                    //printf("%d %s\n", pid, cmdline);
                    for (int i = 0; i < 5; i++) {
                        if (pidd[i] == 0) {
                            //printf("PID?: %d\n", pid);
                            pidd[i] = pid;
                            break;
                        }
                    }
                }
            }
        }
        return;
    }

    if (!builtin_command(argv)) {
        if ((pid = fork()) == 0) {   /* Child runs user job */
            if (execve(argv[0], argv, environ) < 0) {
                if(errno == EACCES){
                    printf("permission denied");
                }
                else if(errno == ENOEXEC || errno == EPERM || errno == EINVAL ){
                    printf("exec format error");
                }
                else {
                    printf("%s: Command not found.\n", argv[0]);
                    //printf("errno %d", errno);
                }                exit(0);
            }
        }

        /* Parent waits for foreground job to terminate */
        if (!bg) {
            int status;
            if (waitpid(pid, &status, 0) < 0)
                printf("waitpid error");
        }
        else {
            //printf("%d %s\n", pid, cmdline);
            for (int i = 0; i < 5; i++) {
                if (pidd[i] == 0) {
                    //printf("PID?: %d\n", pid);
                    pidd[i] = pid;
                    break;
                }
            }
        }
    }
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv )
{
    if (!strcmp(argv[0], "exit")) /* exit command */
        exit(0);
    else if (!strcmp(argv[0], "cd")) {/* exit command */
        chdir(argv[1]);
        if(errno == ENOTDIR){
            printf("not a directory\n");
        }
        if(errno == ENOENT){
            printf("no such file or directory\n");
        }
        if(errno == EACCES){
            printf("permission denied\n");
        }
        return 1;   }                  /* Not a builtin command */
    else if (!strcmp(argv[0], "pwd")) { /* exit command */
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        printf( cwd);
        printf("\n");
        return 1;                     /* Not a builtin command */
    }
    else if (!strcmp(argv[0], "version")){ /* exit command */
        printf("Daisy Bell, dbell, version 1.1, hours worked on project:3\n");
        return 1;  }                   /* Not a builtin command */
    else if (!strcmp(argv[0], "&"))   /* Ignore singleton & */
        return 1;
    return 0;                     /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv)
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
        buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;

    if (argc == 0)  /* Ignore blank line */
        return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
        argv[--argc] = NULL;

    return bg;
}
/* $end parseline */
