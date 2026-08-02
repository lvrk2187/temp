#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define SIZE_OF_STRING 8
#define SIZE_OF_ALL_POSSIBLE_CHARACTERS 62

const char starting_c_code[] = "#include <stdio.h>\n\nint main() {\n\n\treturn 0;\n}";
const char starting_makefile[] = "SRCS = $(wildcard *c)\n\nrun:\n\tgcc $(SRCS) && ./a.out";

const char possible_characters[] = {
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m',
    'n','o','p','q','r','s','t','u','v','w','x','y','z',
    '1','2','3','4','5','6','7','8','9','0'
};

char* build_name() {
  srand(time(NULL));

  char *name = malloc((SIZE_OF_STRING + 1) * sizeof(char));
  name[SIZE_OF_STRING] = '\0';

  for (int i = 0; i < SIZE_OF_STRING; i++) {
    name[i] = possible_characters[rand() % (int) SIZE_OF_ALL_POSSIBLE_CHARACTERS];
  }
  
  return name;
}

int main(int argc, char **argv) {

  char* name;
  char buffer[256];
  struct stat stat_of_file; 
  
  do {
    name = build_name();
  } while (stat(name, &stat_of_file) != -1);

  mkdir(name, S_IRWXU);

  snprintf(buffer, 32, "./%s/%s.c", name, name);
  int if_c_file_created = creat(buffer, S_IRWXU);
  if (if_c_file_created == -1) printf("[ERROR] : Creating C File");
  
  FILE* c_file = fopen(buffer, "w");
  if (c_file == NULL) printf("[ERROR] : Writing C File");

  fputs(starting_c_code, c_file); 
  fclose(c_file);

  snprintf(buffer, 32, "./%s/makefile", name);
  int if_makefile_created = creat(buffer, S_IRWXU);
  if (if_makefile_created == -1) printf("[ERROR] : Creating Makefile");

  FILE* makefile = fopen(buffer, "w");
  if (makefile == NULL) printf("[ERROR] : Writing Makefile");

  fputs(starting_makefile, makefile);
  fclose(makefile);
  
  free(name);  

  return 0;
}
