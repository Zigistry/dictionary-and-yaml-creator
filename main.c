#include <sqlite3.h>
#include <stdio.h>

#define BASE_URL "https://zigistry.dev/"

const char *XML_HEADER =
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<urlset\n"
    "      xmlns='http://www.sitemaps.org/schemas/sitemap/0.9'\n"
    "      xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'\n"
    "      xsi:schemaLocation='http://www.sitemaps.org/schemas/sitemap/0.9\n"
    "            http://www.sitemaps.org/schemas/sitemap/0.9/sitemap.xsd'>\n";

const char *QUERY_ALL_PACKAGES_ON_GITHUB =
    "SELECT repos.id FROM repos JOIN packages ON repos.id = packages.repo_id "
    "WHERE repos.platform = 'github';";

const char *QUERY_ALL_PACKAGES_ON_CODEBERG =
    "SELECT repos.id FROM repos JOIN packages ON repos.id = packages.repo_id "
    "WHERE repos.platform = 'codeberg';";

const char *QUERY_ALL_PROGRAMS_ON_GITHUB =
    "SELECT repos.id FROM repos JOIN programs ON repos.id = programs.repo_id "
    "WHERE repos.platform = 'github';";

const char *QUERY_ALL_PROGRAMS_ON_CODEBERG =
    "SELECT repos.id FROM repos JOIN programs ON repos.id = programs.repo_id "
    "WHERE repos.platform = 'codeberg';";


typedef struct {
    const char *key;
    float value;
} hashmap;

// the maximum needed limit is 50,000 urls for a sitemap.
hashmap hm[50000];

int process_each_row(void *data, int argc, char **argv, char **column_names) {
    printf("Repo ID: %s\n", argv[0]);
    return 0;
}

int main() {
  sqlite3 *db_ptr = NULL;
  FILE *sitemap_fp = fopen("sitemap.xml", "w");

  fprintf(sitemap_fp, "%s", XML_HEADER);

  if (sqlite3_open("zigistry.db", &db_ptr)) {
    printf("Unable to open db");
    return 1;
  } else {
    printf("Database loaded");
  }

  unsigned int url_count = 0;

  hm[url_count++] = (hashmap){BASE_URL, 1.0f};
  hm[url_count++] = (hashmap){BASE_URL "programs", 0.95f};
  hm[url_count++] = (hashmap){BASE_URL "graph", 0.95f};
  hm[url_count++] = (hashmap){BASE_URL "about", 0.90f};
  hm[url_count++] = (hashmap){BASE_URL "help", 0.90f};
  char *error_message = NULL;

  int code = sqlite3_exec(db_ptr, QUERY_ALL_PACKAGES_ON_GITHUB, process_each_row, 0, &error_message);
  if (code != SQLITE_OK) {
      printf("sql error: %s\n", error_message);
      sqlite3_free(error_message);
      return 1;
  }
  if(error_message) {
      printf("%s", error_message);
      return 1;
  }
  
  return 0;
}
