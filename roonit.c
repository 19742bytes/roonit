//I am aware this code is still garbage. Literally my first time doing C.
//Update: I'd like to thank the void linux subreddit for improvements to my code :)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define clRED "\x1b[31m"
#define clRST "\x1b[0m"

void help(void);
void usage_enable(void);
void usage_disable(void);


char service[256];
const char dir[] = "/var/service";
char sv[256];

int main(int argc, char *argv[]) {
	if (argc < 2 || argc > 3) { //If this OR that
		help();
		exit(1);
	}
	if (strcmp(argv[1],"enable") == 0) { //This just makes a symlink using the ln command.
		if (argc > 2) {
			snprintf(service, sizeof(service), "/etc/sv/%s", argv[2]);
			snprintf(sv, sizeof(sv), "ln -s %s %s", service, dir);
			int cmd = system(sv); // idk how to use symlinkat() because i'm stupid
			if (cmd != 0) {
				fprintf(stderr, "Command failed to execute due to the reason above\n");
				exit(1);
			}
			printf("Created symlink %s -> %s\n", service, dir);
			exit(0);
		}
		usage_enable();
		exit(1);
	}
	if (strcmp(argv[1],"disable") == 0) { // This just unlinks a symlink using the unlink command
		if (argc > 2) {
			snprintf(service, sizeof(service), "/var/service/%s", argv[2]);
			snprintf(sv, sizeof(sv), "unlink %s", service);
			int cmd = system(sv); //There's probably some cool way to unlink things in C
			if (cmd != 0) {
				fprintf(stderr, "Command failed to execute due to the reason above\n");
				exit(1);
			}
			printf("Unlinked %s\n", service);
			exit(0);
		}
		usage_disable();
		exit(1);
	}
	help();
	exit(1);
}


//General Help Message
void help(void) {
	printf("Correct usage of roonit: \n");
	printf("roonit enable "clRED "<service>\n"clRST);
	printf("roonit disable "clRED "<service>\n"clRST);
}

// Message if more than 3 arguments
void usage_enable(void) {
	printf("Enables a service\n");
	printf("Example: roonit enable wpa_supplicant\n");
	printf("roonit enable "clRED"<service>\n"clRST);
}

void usage_disable(void) {
	printf("Disables a service\n");
	printf("Example: roonit disable wpa_supplicant\n");
	printf("roonit disable "clRED"<service>\n"clRST);
}
/*
I'm aware this can just be a bash script
But I really wanted to learn C even if I am stupid
*/
