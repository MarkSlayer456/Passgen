#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

char *password;
char chars[52] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
char ext_chars[77] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-=+()*&^%$#@!~<>?,.:/[]{}";

int ext = 0;

int main(int argc, char *argv[]) {
	if(argc <= 2) {
		if(argc == 2) {
			char *file = malloc(4096);
		    memset(file, '\0', 4096);
		    uid_t uid = getuid();
		    struct passwd *pw = getpwuid(uid);
		    strcat(file, pw->pw_dir);
		    strcat(file, "/.passwords/");
		    mkdir(file, 0700);
		    strcat(file, argv[1]);
		    strcat(file, ".passwd");
		    int f = open(file, O_RDONLY | O_APPEND);
		    if(f < 0) {
		    	fprintf(stderr, "No password found for %s\n", argv[1]);
		    	return 0;
		    }
		    char *buf = malloc(sizeof(char) * 1024); // a password will never be longer than this
		    read(f, buf, 1024);
		    printf("password for %s is : %s\n", argv[1], buf);
		    close(f);
		    return 0;
		}
		fprintf(stderr, "Not enough arguments\nUsage: passgen <length> <account> [-s]\n");
		return 0;
	}
	int passSize = 8;
	password = malloc(sizeof(char) * passSize);
	for(int i = 0; i < argc; i++) {
		if(argv[i][0] == '-') {
			if(argv[i][1] == 's') {
				ext = 1;
			}
		}
	}
	int size = atoi(argv[1]); 
	int j = 0;
	time_t t;
	srand((unsigned) time(&t));
	if(ext) {
		while(j <= size) {
			char c = ext_chars[rand() % strlen(ext_chars)];
			if(j == passSize) {
				passSize *= 2;
				password = realloc(password, sizeof(char) * passSize);
			}
			if(j == size) {
				password[j] = '\0';	
			} else {
				password[j] = c;
			}
			j++;
		}
	} else {
		while(j <= size) {
			char c = chars[rand() % strlen(chars)];
			if(j == passSize) {
				passSize *= 2;
				password = realloc(password, sizeof(char) * passSize);
			}
			if(j == size) {
				password[j] = '\0';	
			} else {
				password[j] = c;
			}
			j++;
		}
	}

	char *file = malloc(4096);
    memset(file, '\0', 4096);
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    strcat(file, pw->pw_dir);
    strcat(file, "/.passwords/");
    mkdir(file, 0700);
    strcat(file, argv[2]);
    strcat(file, ".passwd");
    int f = open(file, O_WRONLY | O_APPEND | O_CREAT, 0600);
    if(f < 0) {
    	fprintf(stderr, "Unable to open file\n");
    	return 0;
    }
    write(f, password, strlen(password)+1);
    close(f);
	printf("new password created for your %s account : %s\n", argv[2], password);
	
}
