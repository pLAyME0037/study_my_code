#include <stdio.h>

#include <sys/types.h>
#include <dirent.h>

#define EXIT_FAILURE 1

int main(void) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("/mnt/disk2/mythings/study_my_code/c/webc/display/")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_name == "./" && ent->d_name == "../") continue;
            else printf ("%s\n", ent->d_name);
        }
        closedir(dir);
    } else {
        perror("");
        return EXIT_FAILURE;
    }

    return 0;
}
