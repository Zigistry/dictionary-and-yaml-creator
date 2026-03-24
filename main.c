#include <sqlite3.h>
#include <stdio.h>

const char *XML_HEADER =
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<urlset\n"
    "      xmlns='http://www.sitemaps.org/schemas/sitemap/0.9'\n"
    "      xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'\n"
    "      xsi:schemaLocation='http://www.sitemaps.org/schemas/sitemap/0.9\n"
    "            http://www.sitemaps.org/schemas/sitemap/0.9/sitemap.xsd'>\n";

const char *QUERY_ALL_PACKAGES_ON_GITHUB =
    "SELECT repos.* FROM repos JOIN packages ON repos.id = packages.repo_id "
    "WHERE repos.platform = 'GITHUB';";

const char *QUERY_ALL_PACKAGES_ON_CODEBERG =
    "SELECT repos.* FROM repos JOIN packages ON repos.id = packages.repo_id "
    "WHERE repos.platform = 'CODEBERG';";

const char *QUERY_ALL_PROGRAMS_ON_GITHUB =
    "SELECT repos.* FROM repos JOIN programs ON repos.id = programs.repo_id "
    "WHERE repos.platform = 'GITHUB';";

const char *QUERY_ALL_PROGRAMS_ON_CODEBERG =
    "SELECT repos.* FROM repos JOIN programs ON repos.id = programs.repo_id "
    "WHERE repos.platform = 'CODEBERG';";

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

  return 0;
}
