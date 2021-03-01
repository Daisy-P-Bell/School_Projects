#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ptrace.h>
#include <wait.h>
#include <sys/user.h>
#include <sys/personality.h>
#include <stdint.h>
#include <errno.h>

#define ELF_MAGIC_NUMBER "\x7F\x45\x4C\x46"
#define REG_RIP 1 //64 bit instruction pointer
#define  REG_ECX 2 //32 bit version of rcx      I want edx at some point
#define  REG_EDX 3
#define REG_RDX 4


//Base in Ida: 0x55FAB94DA000
//             0x55D356D17000

//Base in Linux (no ASLR): 0x555555554000
//

//difference   0xA563F86000
//             0x7E017C3000


// BP In IDA 000055FAB94E201A
//           000055AC98491A7A
//           000055555555BA7A

//#define BREAK_POINT_ADDRESS 0x55555555C01A
//#define BREAK_POINT_ADDRESS 0x55555555BA7A
#define BREAK_POINT_ADDRESS 0x55555555BA76

int unpoke_int3(pid_t pid, uint64_t location, uint64_t replaced_data) {
    uint64_t data;
    int status = 0;
    data = ptrace(PTRACE_PEEKDATA, pid, location, NULL);
    data = ((data & ~0xfful) | replaced_data);
    status = ptrace(PTRACE_POKEDATA, pid, location, data);
    return status;
}


void fatal(char * msg){
    fprintf(stderr, "ERROR: %s\n", msg);
    exit(-1);
}

void validate_exe_path(const char * path){
    int fd;
    char magic_number[4];

    fd = open(path, O_RDONLY);
    if(fd == -1){
        fatal("Invalid path");
    }

    read(fd, magic_number, 4);
    if(memcmp(magic_number, ELF_MAGIC_NUMBER, 4) != 0){
        fatal("Not an ELF file");
    }
}

void alarm_signal_set(pid_t pid, int on) {
    sigset_t sig_mask_set;
    sigemptyset(&sig_mask_set);
    // Manpage says sizeof(siget_t) but it needs size 8 to work?!
    ptrace(PTRACE_GETSIGMASK, pid, sizeof(int64_t), &sig_mask_set);
    if (on) {
        sigdelset(&sig_mask_set, SIGALRM);
    } else {
        sigaddset(&sig_mask_set, SIGALRM);
    }
    ptrace(PTRACE_SETSIGMASK, pid, sizeof(int64_t), &sig_mask_set);
}

/*void info_registers(pid_t pid){
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);

    printf("rip 0x%016llx\n", regs.rip); //hexiecimal version of a long long

}
*/



uint64_t poke_int3(pid_t pid, uint64_t location){

    uint64_t data, save, int3, data_with_int3;

    data = ptrace(PTRACE_PEEKDATA, pid, location, NULL);
    save = data & 0xfful;  //ul is unsigned long
    int3 = 0xcc;
    data_with_int3 = ((data & ~0xfful) | int3);  //int 3 is what overwrites and writes the value, this is what you would change to set it to 1   <- I don't know if that's actually true

    if(ptrace(PTRACE_POKEDATA, pid, location, data_with_int3) == -1){
        //printf("Oh dear, something went wrong with read()! %s\n", strerror(errno));
        fatal("Poking int3 failed");
    }

    return save;

}

uint64_t get_register(pid_t pid, int reg) {
    uint64_t value;
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    switch (reg) {
        case REG_RIP:
            value = regs.rip;
            break;
        case REG_RDX:
            value = regs.rdx;
            break;
        default:
            value = 0;
            break;
    }
    return value;
}

uint64_t set_register(pid_t pid, int reg, uint64_t new_value) {
    uint64_t former_value;
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    switch (reg) {
        case REG_RIP:
            former_value = regs.rip;
            regs.rip = new_value;
            break;
        case REG_RDX:
            former_value = regs.rdx;
            regs.rdx = new_value;
            break;
        default:
            former_value = 0;
            break;
    }
    ptrace(PTRACE_SETREGS, pid, NULL, &regs);
    return former_value;
}
uint32_t get_register32(pid_t pid, int reg) {
    uint32_t value;
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    switch (reg) {
        case REG_ECX: // note: rcx is the 64 bit register whereas ecx is the lower 32 bits of rcx
            value = (uint32_t) regs.rcx & 0x00000000FFFFFFFF;
            break;

        case REG_EDX: // note: rcx is the 64 bit register whereas ecx is the lower 32 bits of rcx
            value = (uint32_t) regs.rdx & 0x00000000FFFFFFFF;
            break;

            default:
            value = 0;
            break;
    }
    return value;
}
uint32_t set_register32(pid_t pid, int reg, uint32_t new_value) {
    struct user_regs_struct regs;
    uint32_t former_value;
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    switch (reg) {
        case REG_ECX: // note: rcx is the 64 bit register whereas ecx is the lower 32 bits of rcx
            former_value = (uint32_t) regs.rcx & 0x00000000FFFFFFFF;
            regs.rcx = (regs.rcx & 0xFFFFFFFF00000000) | new_value;
            break;
        case REG_EDX: // note: rcx is the 64 bit register whereas ecx is the lower 32 bits of rcx
            former_value = (uint32_t) regs.rdx & 0x00000000FFFFFFFF;
            regs.rdx = (regs.rdx & 0xFFFFFFFF00000000) | new_value;
            break;
        default:
            former_value = 0;
            break;
    }
    ptrace(PTRACE_SETREGS, pid, NULL, &regs);
    return former_value;
}

void single_step(pid_t pid) {
    int status;
    alarm_signal_set(pid, 0);
    ptrace(PTRACE_SINGLESTEP, pid, (char *) 1, 0);
    if (waitpid(pid, &status, 0) == -1) {
        fatal("Inferior changed state in an unpredicted way during single step!?");
    }
    alarm_signal_set(pid, 1);
}

void wait_for_inferior(pid_t pid){

    int status;
    int first_stop = 1;
    uint64_t save = 0, ip;
    uint32_t save32reg;

    while(1) {

        if (waitpid(pid, &status, 0) == -1) {
            fatal("Error: Couldn't wait for inferior");
        }

        if(WIFEXITED(status)){
            printf("xdeminuer exited.\n");
            return;
        }
        else if(WIFSTOPPED(status)){

            if(WSTOPSIG(status) == SIGALRM){

                printf(".");
                fflush(stdout);
                ptrace(PTRACE_CONT, pid, (char *) 1, SIGALRM);
                continue;

            }
            else if(WSTOPSIG(status) == SIGSEGV){

                fatal("Ya done messed up, SEGFAULT.\n");

            }
            else if(WSTOPSIG(status) == SIGTRAP){

                printf("it's a trap.\n");

                if(first_stop){  //initial trap

                    first_stop = 0;
                    //alarm_signal_set(pid, 0);  //this turns off the alarm signal for timer
                    save = poke_int3(pid, BREAK_POINT_ADDRESS);     //interrupt 3
                    //printf("it's a trap.\n");


                }
                else{
                    //uint64_t tmp = get_register(pid, REG_RIP);
                    ip = get_register(pid, REG_RIP);
                    if(ip - 1 == BREAK_POINT_ADDRESS){
                        unpoke_int3(pid, BREAK_POINT_ADDRESS, save);
                        set_register(pid, REG_RIP, BREAK_POINT_ADDRESS);

                        if(get_register32(pid, REG_EDX) == 0) {
                            set_register32(pid, REG_EDX, 1);  //WAS SET_REGISTER32
                            get_register32(pid, REG_EDX);
                            printf("help");
                        }
                        //uint64_t test = get_register(pid, REG_EDX);
                        single_step(pid); //will want to get rid of this

                        //set_register32(pid, REG_ECX, save32reg); //will want to get rid of this

                        save = poke_int3(pid, BREAK_POINT_ADDRESS);
                        //printf("it's a trap.\n");

                    }
                }

            }
            else {
                printf("Warning: unhandled stop signal. \n");
            }
        }
        ptrace(PTRACE_CONT, pid, (char *) 1, 0);
    }
}




void attach_exe(char * path, char ** argv){

    pid_t pid;
    //char * argv[] = {path, NULL};
    validate_exe_path(path);

    pid = fork();
    switch(pid){

        case 0: //inferior
            ptrace(PT_TRACE_ME, 0, NULL, 0);
            personality(ADDR_NO_RANDOMIZE);
            if(execv(path, argv) == -1){
                fatal("Execv failed.");
            }
            break;

        case -1: //error with fork

            break;

        default: //in tracer
            wait_for_inferior(pid);
            //info_registers(pid);
            break;


    }

}

int main(int argc, char *argv[]) {


    if(argc < 2){
        fatal("Requires xdemineur with optional arguements.");
    }


    attach_exe(argv[1], argv+1);


    return 0;
}
