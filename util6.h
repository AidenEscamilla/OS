/*************************************
*File: util6.h
*Author: Jey veerasam (UTD UNIX Professor)
*
*
*
*
*
*
*
*************************************/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

#define MAXLEN 100

bool send_string(FILE *fp, char *str);

bool receive_string(FILE *fp, char *newStr);
