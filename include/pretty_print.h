#pragma once

#define KNRM "\e[0m"
#define BOLD "\e[1m"
#define KRED "\e[31m"
#define KGRN "\e[32m"
#define KYEL "\e[33m"
#define KBLU "\e[34m"
#define KMAG "\e[35m"
#define KCYN "\e[36m"
#define KWHT "\e[37m"

#include <short_types.h>

void print_code(char *msg, u32 code);
