#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define SIZE_OF_STRING 8
#define SIZE_OF_ALL_POSSIBLE_CHARACTERS 62
#define DEFAULT_SIZE 512
#define DEFAULT_MODE S_IRWXU | S_IRWXG | S_IRWXO

const char starting_c_code[] = "#include <stdio.h>\n\nint main() {\n\n\treturn 0;\n}";
const char starting_makefile[] = "SRCS = $(wildcard *c)\n\nrun:\n\tgcc $(SRCS) && ./a.out";

char *home_path;
char registry_dir[DEFAULT_SIZE];

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

void inital_directory_setup() {
  char buffer[DEFAULT_SIZE];
  home_path = getenv("HOME");
  struct stat stat_of_storing_txt_file;
  struct stat stat_of_if_directory_exists;

  if (home_path == NULL) {
    home_path = malloc(sizeof(char) * (1 << 4));
    strncpy(home_path, ".", 2);
  }

  snprintf(buffer, DEFAULT_SIZE, "%s/.ctemp", home_path);

  if ((stat(buffer, &stat_of_if_directory_exists) == -1 && S_ISDIR(stat_of_if_directory_exists.st_mode))) {
    mkdir(buffer, DEFAULT_MODE);
  }

  snprintf(buffer, DEFAULT_SIZE, "%s/registry.txt", buffer);

  if(stat(buffer, &stat_of_storing_txt_file) == -1) {
    creat(buffer, DEFAULT_MODE);
  }

  strncpy(registry_dir, buffer, DEFAULT_SIZE);
}

void store_in_registry(char *dir) {
  
  FILE *registry = fopen(registry_dir, "a");
  fputs(dir, registry);
  fputs("\n", registry);
  fclose(registry);
}

void create_file() {
  
  char* name;
  char buffer[DEFAULT_SIZE];
  struct stat stat_of_file;

  do {
      name = build_name();
  } while (stat(name, &stat_of_file) != -1);

  mkdir(name, S_IRWXU);

  snprintf(buffer, DEFAULT_SIZE, "./%s/%s.c", name, name);
  int if_c_file_created = creat(buffer, DEFAULT_MODE);
  if (if_c_file_created == -1) printf("[ERROR] : Creating C File");

  FILE* c_file = fopen(buffer, "w");
  if (c_file == NULL) printf("[ERROR] : Writing C File");

  fputs(starting_c_code, c_file); 
  fclose(c_file);

  snprintf(buffer, DEFAULT_SIZE, "./%s/makefile", name);
  int if_makefile_created = creat(buffer, DEFAULT_MODE);
  if (if_makefile_created == -1) printf("[ERROR] : Creating Makefile");

  FILE* makefile = fopen(buffer, "w");
  if (makefile == NULL) printf("[ERROR] : Writing Makefile");

  fputs(starting_makefile, makefile);
  fclose(makefile);


  free(name);  
}




int main(int argc, char **argv) {

  
  // mkdir("~/.ctemp", DEFAULT_MODE);

  // generating actual temporary file
  // printf("%d", S_IRWXU);
  // create_file();
  inital_directory_setup();
  
  return 0;
}
