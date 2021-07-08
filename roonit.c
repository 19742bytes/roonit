//I am aware this code is garbage. Literally my first time doing C.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void help();
void enable();
void disable();
void start();


char service[256];
const char dir[256] = "/var/service\0";
char sv[256];

int main(int argc, char *argv[]) {
	if (argc < 2 || argc > 3) { //If this OR that
		help();
	}
    if (strcmp(argv[1],"enable") == 0) { //This just makes a symlink using the ln command.
        if (argc > 2) {
            snprintf(service, sizeof(service), "/etc/sv/%s", argv[2]);
            snprintf(sv, sizeof(sv), "ln -s %s %s", service, dir);
            int cmd = system(sv); // idk how to use symlinkat() because i'm stupid
	    if (cmd != 0) {
               printf("Command failed to execute due to the reason above\n");
               exit(1);
            }
            printf("Created symlink %s -> %s\n", service, dir);
            exit(0);
        }
        enable();
    }
     if (strcmp(argv[1],"disable") == 0) { // This just unlinks a symlink using the unlink command
             if (argc > 2) {
                 snprintf(service, sizeof(service), "/var/service/%s", argv[2]);
                 snprintf(sv, sizeof(sv), "unlink %s", service);
                 system(sv); //There's probably some cool way to unlink things in C
		 if (cmd != 0) {
                    printf("Command failed to execute due to the reason above\n");
                    exit(1);
                 }
                 printf("Unlinked %s\n", service);
                 exit(0);
                     }
             disable();
             }
	help();
}



//General Help Message
void help() {
	printf("Correct usage of roonit: \n");
	printf("roonit enable <service>\n");
	printf("roonit disable <service>\n");
	exit(0);
}

// Message if more than 3 arguments
void enable() {
    printf("Enables a service\n");
    printf("Example: roonit enable wpa_supplicant\n");
    printf("roonit enable <service>\n");
    exit(0);
}



void disable() {
    printf("Disables a service\n");
    printf("Example: roonit disable wpa_supplicant\n");
    printf("roonit disable <service>\n");
    exit(0);
}
/*
I'm aware this can just be a bash script
But I really wanted to learn C even if I am stupid
*/
