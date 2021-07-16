//I am aware this code is still garbage. Literally my first time doing C.
//Update: I'd like to thank the void linux subreddit for improvements to my code :)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //This adds support for getopts()

#define clRED "\x1b[31m"
#define clRST "\x1b[0m"

void help(void);
void usage_enable(void);
void usage_disable(void);
void usage_start(void);
void usage_stop(void);
void usage_status(void);


char * service;
const char dir[] = "/var/service";
char * sv;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		help();
		exit(1);
	}
	if (strcmp(argv[1],"enable") == 0) { //This just makes a symlink using the ln command.
		if (argc > 2) {
			if (argc > 3) {fprintf(stderr,"You know it isn't gonna read anything past %s, right?\n", argv[3]);} // I should probably make this a feature later
			service = malloc(sizeof(char)*(9 + strlen(argv[2])));// Allocates just enough memory for this variable
			if (!service) {
				fprintf(stderr, "And the words on the screen are,\n You're out of memory!\n OH NO!\n"); 
				exit(1);
			}
			sprintf(service, "/etc/sv/%s", argv[2]);
			sv = malloc(sizeof(char)*(8 + strlen(service) + strlen(dir)));
			if (!sv) {
				fprintf(stderr, "Not enough memory!"); 
				exit(1);
			}
			sprintf(sv, "ln -s %s %s", service, dir);
			int cmd = system(sv); // idk how to use symlinkat() because i'm stupid
			if (cmd != 0) {
				fprintf(stderr, "Command failed to execute due to the reason above\n");
				exit(1);
			}
			printf("Created symlink %s -> %s\n", service, dir);
			free(service); //Frees memory
			free(sv);
			exit(0);
		}
		usage_enable();
		exit(1);
	}
	if (strcmp(argv[1],"disable") == 0) { // This just unlinks a symlink using the unlink command
		if (argc > 2) {
			if (argc > 3) {fprintf(stderr, "You know it isn't gonna read anything past %s, right?\n", argv[3]);}
			service = malloc(sizeof(char)*(14 + strlen(argv[2])));
			if (!service){
				fprintf(stderr, "Not enough memory!");
				exit(1);
			}
			sprintf(service, "/var/service/%s", argv[2]);
			sv = malloc(sizeof(char)*(8+strlen(service)));
			if (!sv) {
				fprintf(stderr, "Not enough memory!");
				exit(1);
			}
			sprintf(sv, "unlink %s", service);
			int cmd = system(sv); //There's probably some cool way to unlink things in C
			if (cmd != 0) {
				fprintf(stderr, "Command failed to execute due to the reason above\n");
				exit(1);
			}
			printf("Unlinked %s\n", service);
			free(service);
			free(sv);
			exit(0);
		}
		usage_disable();
		exit(1);
	}
	if (strcmp(argv[1],"start") == 0) { //This just calls the sv command to start a service
		if (argc > 2) {
			if (argc > 3) {fprintf(stderr, "You know it isn't gonna read anything past %s, right?\n", argv[3]);}
			sv = malloc(sizeof(char)*(10 + strlen(argv[2])));
			if (!sv) {
				fprintf(stderr, "Not enough memory!\n");
				exit(1);
			}
			sprintf(sv, "sv start %s", argv[2]);
			int cmd = system(sv);
			if (cmd != 0) {
				fprintf(stderr, "Command failed to execute due to the reason above\n");
				exit(1);
			}
			printf("Started service %s successfully!\n", argv[2]);
			free(sv);
			exit(0);
		}
		usage_start();
		exit(1);
	}
	if (strcmp(argv[1],"stop") == 0) { //This just calls the sv command to stop a service
		if (argc > 2) {
			if (argc > 3) {fprintf(stderr, "You know it isn't gonna read anything past %s, right?\n", argv[3]);}
			sv = malloc(sizeof(char)*(9 + strlen(argv[2])));
			if (!sv) {
				fprintf(stderr, "Not enough memory!\n");
				exit(1);
			}
			sprintf(sv, "sv stop %s", argv[2]);
			int cmd = system(sv);
			if (cmd != 0) {
				fprintf(stderr, "Command failed to execute due to the reason above\n");
				exit(1);
			}
			printf("Stopped service %s successfully!\n", argv[2]);
			free(sv);
			exit(0);
		}
		usage_stop();
		exit(1);
	}
	if (strcmp(argv[1],"status") == 0) { // This operates on black magic
		if (argc > 2) {
			int c;
			opterr = 0;
			while ( (c = getopt(argc, argv, "a")) != -1){
				switch(c) {
					case 'a':
						printf("This *would* show all the status'\n");
						break;
					default:
						fprintf(stderr, "-%c is not a valid option\n\n", optopt);
						fprintf(stderr, "Infact, -a is the only option for the status option\n");
						break;
				}
			}
			//Cool code would go here
		}
		usage_status();
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
	printf("roonit start "clRED "<service>\n"clRST);
	printf("roonit stop "clRED "<service>\n"clRST);
	printf("roonit status "clRED "<service>\n"clRST);
}

//Usage Messages
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

void usage_start(void) {
	printf("Starts a service\n");
	printf("Example: roonit start wpa_supplicant\n");
	printf("roonit start "clRED"<service>\n"clRST);
}

void usage_stop(void) {
	printf("Stops a service\n");
	printf("Example: roonit stop wpa_supplicant\n");
	printf("roonit stop "clRED"<service>\n"clRST);
}

void usage_status(void) {
	printf("Shows the status of a service\n");
	printf("Example: roonit status wpa_supplicant\n");
	printf("roonit status "clRED"<service>\n"clRST);
	printf("You can also use the -a switch to show the status of all services\n");
}
/*
I'm aware this can just be a bash script
But I really wanted to learn C even if I am stupid
(Go check out https://github.com/bahamas10/vsv it's a smarter roonit and a bash script that proves my point)
*/
