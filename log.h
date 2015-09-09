#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

#define LOG_OPENDOOR_FILE "Opendoorlog.txt"

extern void Fprint_LogString(const char *buffer,const char* Outname,const char* Mode)  ;


#endif