#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int use_mibibytes = 0;
    int use_gibibytes = 0;
    if (argc == 2) {
      if (strcasecmp(argv[1], "mib") == 0) {
        use_mibibytes = 1;
      } 
      else if (strcasecmp(argv[1], "gib") == 0) {
        use_gibibytes = 1;
      } 
    }

    struct statvfs fs_info;

    if (statvfs("/", &fs_info) == -1) {
        perror("Error al obtener información del sistema de archivos");
        exit(EXIT_FAILURE);
    }

    unsigned long long total_space = fs_info.f_blocks * fs_info.f_frsize;
    unsigned long long free_space = fs_info.f_bfree * fs_info.f_frsize;
    unsigned long long used_space = total_space - free_space;

    double free_space_units, used_space_units;
    if (use_gibibytes) {
        free_space_units = free_space / (1024.0 * 1024.0 * 1024.0);
        used_space_units = used_space / (1024.0 * 1024.0 * 1024.0);
    } else if (use_mibibytes) {
        free_space_units = free_space / (1024.0 * 1024.0);
        used_space_units = used_space / (1024.0 * 1024.0);
    } else {
        free_space_units = free_space / 1024.0;
        used_space_units = used_space / 1024.0;
    }

    double used_space_percent = (used_space * 100.0) / total_space;
    double free_space_percent = (free_space * 100.0) / total_space;

    if (use_gibibytes) {
        printf("Espacio Utilizado de disco: %.2f%% (%.2f gibibytes)\n", used_space_percent, used_space_units);
        printf("Espacio Libre en disco: %.2f%% (%.2f gibibytes)\n", free_space_percent, free_space_units);
    } else if (use_mibibytes) {
        printf("Espacio Utilizado de disco: %.2f%% (%.2f mebibytes)\n", used_space_percent, used_space_units);
        printf("Espacio Libre en disco: %.2f%% (%.2f mebibytes)\n", free_space_percent, free_space_units);
    } else {
        printf("Espacio Utilizado de disco: %.2f%% (%.2f kilobytes)\n", used_space_percent, used_space_units);
        printf("Espacio Libre en disco: %.2f%% (%.2f kilobytes)\n", free_space_percent, free_space_units);
    }

    return 0;
}
