#include <sqlite3.h>
#include <stdio.h>


const char *  ALL_PACKAGES_ON_GITHUB=
"SELECT repos.* FROM repos JOIN packages ON repos.id = packages.repo_id WHERE repos.platform = 'GITHUB';";

const char * ALL_PACKAGES_ON_CODEBERG=
"SELECT repos.* FROM repos JOIN packages ON repos.id = packages.repo_id WHERE repos.platform = 'CODEBERG';";

const char * ALL_PROGRAMS_ON_GITHUB=
"SELECT repos.* FROM repos JOIN programs ON repos.id = programs.repo_id WHERE repos.platform = 'GITHUB';";

const char * ALL_PROGRAMS_ON_CODEBERG=
"SELECT repos.* FROM repos JOIN programs ON repos.id = programs.repo_id WHERE repos.platform = 'CODEBERG';";
  
int main() {
  sqlite3 *db_ptr = NULL;

  if(sqlite3_open("zigistry.db", &db_ptr)) {
    printf("Unable to open db");
    return 1;
  } else {
    printf("Database loaded");
  }

  

  return 0;
}

