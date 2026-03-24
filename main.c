#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
unsigned int url_count = 0;

char resulting_string[500];
int process_each_row_packages_on_gh(void *data, int argc, char **argv,
                                    char **column_names) {
  const char *route_starting_part = (const char *)data;
  // the id starts with cb/ or gh/
  // I will be removing cb/ with codeberg/ and gh/ with github/
  // "packages/github/"
  size_t final_string_size =
      snprintf(NULL, 0, "%s%s", route_starting_part, argv[0] + 3) + 1;

  char *resulting_string = malloc(final_string_size);

  snprintf(resulting_string, final_string_size, "%s%s",
           route_starting_part, argv[0] + 3);

  hm[url_count++] = (hashmap){resulting_string, 0.8f};
  return 0;
}

int main() {
  sqlite3 *db_ptr = NULL;

  if (sqlite3_open("zigistry.db", &db_ptr)) {
    printf("Unable to open db");
    return 1;
  } else {
    printf("Database loaded");
  }

  hm[url_count++] = (hashmap){"", 1.0f}; // Added for / route
  hm[url_count++] = (hashmap){"programs", 0.95f};
  hm[url_count++] = (hashmap){"graph", 0.95f};
  hm[url_count++] = (hashmap){"about", 0.90f};
  hm[url_count++] = (hashmap){"help", 0.90f};
  char *error_message = NULL;

  int code = sqlite3_exec(db_ptr, QUERY_ALL_PACKAGES_ON_GITHUB,
                          process_each_row_packages_on_gh,
                          (void *)"packages/github/", &error_message);
  if (code != SQLITE_OK) {
    printf("sql error: %s\n", error_message);
    sqlite3_free(error_message);
    return 1;
  }
  code = sqlite3_exec(db_ptr, QUERY_ALL_PACKAGES_ON_CODEBERG,
                      process_each_row_packages_on_gh,
                      (void *)"packages/codeberg/", &error_message);
  if (code != SQLITE_OK) {
    printf("sql error: %s\n", error_message);
    sqlite3_free(error_message);
    return 1;
  }
  code = sqlite3_exec(db_ptr, QUERY_ALL_PROGRAMS_ON_GITHUB,
                      process_each_row_packages_on_gh,
                      (void *)"programs/github/", &error_message);
  if (code != SQLITE_OK) {
    printf("sql error: %s\n", error_message);
    sqlite3_free(error_message);
    return 1;
  }
  code = sqlite3_exec(db_ptr, QUERY_ALL_PROGRAMS_ON_CODEBERG,
                      process_each_row_packages_on_gh,
                      (void *)"programs/codeberg/", &error_message);
  if (code != SQLITE_OK) {
    printf("sql error: %s\n", error_message);
    sqlite3_free(error_message);
    return 1;
  }

  FILE *sitemap_fp = fopen("sitemap.xml", "w");

  fprintf(sitemap_fp, "%s", XML_HEADER);

  for (int i = 0; i < url_count; i++) {
    fprintf(sitemap_fp,
            "<url>\n"
            "<loc>https://zigistry.dev/%s</loc>\n"
            "<priority>%.2f</priority>\n"
            "</url>\n",
            hm[i].key, hm[i].value
    );
  }

  fprintf(sitemap_fp, "</urlset>\n");

  return 0;
}
