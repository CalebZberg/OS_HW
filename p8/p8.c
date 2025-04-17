#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

long total_size = 0;

void list_directory(const char *path, int depth) {
    DIR *dir = opendir(path);
    if (!dir) return;

    struct dirent *entry;
    struct stat file_stat;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' && depth > 0) continue;
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char *fullpath;
        asprintf(&fullpath, "%s/%s", path, entry->d_name);

        if (lstat(fullpath, &file_stat) == -1) {
            free(fullpath);
            continue;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            for (int i = 0; i < depth; i++) printf("  ");
            printf("dir %s\n", fullpath);
            list_directory(fullpath, depth + 1);
        } else if (S_ISREG(file_stat.st_mode)) {
            total_size += file_stat.st_size;
            for (int i = 0; i < depth; i++) printf("  ");
            printf("%8ld:%s\n", file_stat.st_size, entry->d_name);
        }

        free(fullpath);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *start_dir = (argc > 1) ? argv[1] : ".";
    printf("dir %s\n", start_dir);
    list_directory(start_dir, 1);
    printf("\nTotal file space used:%ld\n", total_size);
    return 0;
}
