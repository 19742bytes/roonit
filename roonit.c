//I am aware this code is still garbage. Literally my first time doing C.
//Update: I'd like to thank the void linux subreddit for improvements to my code :)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //This adds support for getopts()
#include <stdbool.h> // support for bool type?

//Defines colors so I don't have to
#define clRED "\x1b[31m" 
#define clGREEN "\x1b[32m"
#define clYELLOW "\x1b[33m" //gonna implement these sooner or later
#define clRST "\x1b[0m"



//Prototyping functions
void help(void);
void usage_enable(void);
void usage_disable(void);
void usage_start(void);
void usage_stop(void);
void usage_status(void);
int aSize(char * cmd1); //This function will an integer from a command (usually from wc -l)
char * command(char * cmd1);//This function will return anything from the stdout of a command
//There probably better ways to do the above. But if it works, it isn't *that* stupid


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
			bool all = false;
			int c;
			struct Service{
				char * name;
				char * on; 
				char * boot; 
			};
			opterr = 0;
			while ( (c = getopt(argc, argv, "a")) != -1){
				switch(c) {
					case 'a':
						all = true;
						break;
					default: 
						fprintf(stderr, "-%c is not a valid option\n\n", optopt);
						fprintf(stderr, "Infact, -a is the only option for the status option");
						exit(1);
						break;
				}
			}
			if (all == true){
				int i;
				char * s1 = command("ls /etc/sv|sort -d");
				struct Service all[aSize("ls /etc/sv|wc -l")-1];
				char delimit[] = "\n";
				char wontcount[] = "/etc/sv/";
				char wontcount2[] = "/supervise/stat";
				char * supertemp;
				char * supertemp2;
				char *current = strtok(s1, delimit);
				printf("%*s %*s %s\n", aSize("ls /etc/sv|wc -L"), "Service", 10, "Status", "Boot");
				for (i = 0; i < aSize("ls /etc/sv|wc -l"); i++) { //broken?
					all[i].name = current;
					//There's probably a better way to do this
					supertemp = (char*)malloc(sizeof(char)*(strlen(wontcount) + strlen(current) + strlen(wontcount2)));
					if (!supertemp) {fprintf(stderr, "Not enough memory!\n"); exit(1);}
					sprintf(supertemp, "%s%s%s", wontcount, current, wontcount2);
					if (access(supertemp, F_OK) == 0){
						supertemp2 = (char*)malloc(sizeof(char)*(6 + strlen(wontcount) + strlen(current)));
						sprintf(supertemp2, "%s%s/down", wontcount, current);
						if (access(supertemp2,  F_OK) == 0) { //checks for a down file
							all[i].boot = "disabled";
						}
						else {
							all[i].boot = "enabled";
						}
						free(supertemp2);
						supertemp2 = (char*)malloc(sizeof(char)*(4 +strlen(supertemp)));
						sprintf(supertemp2, "cat %s", supertemp);
						if (strcmp(command(supertemp2),"run\n") == 0) {
							all[i].on = "running";
						}
						else {
							all[i].on = "stopped";
						}
						free(supertemp2);
					}
					else {
						all[i].on = "stopped";
						all[i].boot = "disabled";
					}
					free(supertemp);
					current = strtok(NULL, delimit);
					printf("%*s %*s %s\n", aSize("ls /etc/sv|wc -L"), all[i].name, 10, all[i].on, all[i].boot);
				}
				exit(0);
			}
			else { //This will just call the "sv status" command
				sv = (char*)malloc(sizeof(char)*(strlen(argv[2] + strlen("sv status"))));
				if (!sv) {fprintf(stderr, "Not enough memory!\n"); exit(1);}
				sprintf(sv, "sv status %s", argv[2]);
				int cmd = system(sv);
				if (cmd != 0) {fprintf(stderr, "Failed to check status!\n"); exit(1);}
				exit(0);

			}
			fprintf(stderr, "I don't understand how you ended up here\nBut here's the help message anyways:\n");
			usage_status();
			exit(1);
		}
		usage_status();
		exit(1);
	}
	help();
	exit(1);
}
//Useful function(s)

int aSize(char * cmd1) {
	FILE *tmp1;
	char * cmd2;
	char * res;
	long size;
	const char append[] = "> whywouldanyonenameafilethis";
	cmd2 = (char*)malloc(sizeof(char)*(strlen(cmd1) + strlen(append)));
	if (!cmd2){fprintf(stderr, "Not enough memory!\n"); exit(2);}
	sprintf(cmd2, "%s %s", cmd1, append);
	int succ = system(cmd2);
	if (succ != 0) {fprintf(stderr, "Failed to create array!\n"); exit(2);}
	//I fucking stole the following code from a guide
	tmp1 = fopen("whywouldanyonenameafilethis", "r");
	if (tmp1 == NULL) {fprintf(stderr, "Failed to open temporary file!\n"); exit(2);}
	fseek(tmp1, 0L, SEEK_END);
	size = ftell(tmp1);
	fseek(tmp1, 0L, SEEK_SET);
	res = (char*)calloc(size, sizeof(char));
	if (res == NULL){fprintf(stderr, "Not enough memory!\n"); exit(2);}
	fread(res, sizeof(char), size, tmp1);
	fclose(tmp1);
	//End of stolen code. From: http://www.fundza.com/c4serious/fileIO_reading_ALL/index.html
	free(cmd2);
	remove("whywouldanyonenameafilethis");
	return atoi(res);
}
//I don't understand popen() rn. But this spaghetti works

char * command(char * cmd1) {
	FILE *tmp1;
	char * cmd2;
	char * res;
	long size;
	const char append[] = "> whywouldanyonenameafilethis";
	cmd2 = (char*)malloc(sizeof(char)*(strlen(cmd1) + strlen(append)));
	if (!cmd2){fprintf(stderr, "Not enough memory!\n"); exit(2);}
	sprintf(cmd2, "%s %s", cmd1, append);
	int succ = system(cmd2);
	if (succ != 0) {fprintf(stderr, "Failed to create array!\n"); exit(2);}
	//I fucking stole the following code from a guide
	tmp1 = fopen("whywouldanyonenameafilethis", "r");
	if (tmp1 == NULL) {fprintf(stderr, "Failed to open temporary file!\n"); exit(2);}
	fseek(tmp1, 0L, SEEK_END);
	size = ftell(tmp1);
	fseek(tmp1, 0L, SEEK_SET);
	res = (char*)calloc(size, sizeof(char));
	if (res == NULL){fprintf(stderr, "Not enough memory!\n"); exit(2);}
	fread(res, sizeof(char), size, tmp1);
	fclose(tmp1);
	//End of stolen code. From: http://www.fundza.com/c4serious/fileIO_reading_ALL/index.html
	free(cmd2);
	remove("whywouldanyonenameafilethis");
	return res;
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
	printf("Shows the status of a service in the /var/service directory\n");
	printf("Example: roonit status wpa_supplicant\n");
	printf("roonit status "clRED"<service>\n"clRST);
	printf("You can also use the -a switch to show the status of all services\n");
	printf("roonit status -a\n");
}
/*
I'm aware this can just be a bash script
But I really wanted to learn C even if I am stupid
(Go check out https://github.com/bahamas10/vsv it's a smarter roonit and a bash script that proves my point)
*/
