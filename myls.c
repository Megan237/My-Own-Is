#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <limits.h>

//void print_header() {
//    printf("%-6s %-3s %-8s %-8s %-8s %-24s %-24s %-6s %-s\n", "Serial", "Links", "User ID", "Group ID", "Size", "Last Access Time", "Last Modify Time", "Blocks", "Permissions");
//}

void print_file_info(const char *filename) {
    struct stat file_stat;
    
    if (stat(filename, &file_stat) == -1) {
        perror("Error reading file information");
        exit(EXIT_FAILURE);
    }

    printf("Serial: %-6ld  Links: %-3ld User ID: %-8ld Group ID: %-8ld Size: %-8ld Last Access Time: %-24s Last Modify Time: %-24s Blocks: %-6ld Permissions: ",
           (long)file_stat.st_ino,
           (long)file_stat.st_nlink,
           (long)file_stat.st_uid,
           (long)file_stat.st_gid,
           (long)file_stat.st_size,
           ctime(&file_stat.st_atime),
           ctime(&file_stat.st_mtime),
           (long)file_stat.st_blocks);

    // Print permissions
    printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x\n" : "-\n\n\n");
    
    
}


void list_files(const char *dirname) {
    // Convert the given path to an absolute path
    char absolute_path[PATH_MAX];
    if (realpath(dirname, absolute_path) == NULL) {
        perror("Error resolving path");
        exit(EXIT_FAILURE);
    }

    DIR *dir = opendir(absolute_path);
    if (!dir) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            // Only print regular files, not directories or other types
            // Use the absolute path to print file information
            char file_path[PATH_MAX];
            snprintf(file_path, sizeof(file_path), "%s/%s", absolute_path, entry->d_name);
            print_file_info(file_path);
        }
    }

    closedir(dir);
}

          

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory1> [<directory2> ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        printf("Listing files in directory: %s\n", argv[i]);
        list_files(argv[i]);
        printf("\n");
    }

    return 0;
}


