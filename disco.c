#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <string.h>

int main(int argc, char *argv[]) {
    //Variables
    int use_mibibytes = 0;
    int use_gibibytes = 0;

    //Revisa los argumentos pasados al programa (usa mebibytes si -tm y gibibytes si -tg)
    for (int i = 1; i < argc; i++) {  
        if (strcasecmp(argv[i], "-tm") == 0) {
            use_mibibytes = 1;  
        }
        else if (strcasecmp(argv[i], "-tg") == 0) {
            use_gibibytes = 1;  
        }
    }

    // Variable para informacion sobre el sistema de archivos
    struct statvfs fs_info;

    // Llama a statvfs para obtener informacion de la raiz "/"
    if (statvfs("/", &fs_info) == -1) {
        perror("Error al obtener información del sistema de archivos");
        exit(EXIT_FAILURE);
    }

    //Calculos en bytes
    unsigned long long total_space = fs_info.f_blocks * fs_info.f_frsize;
    unsigned long long free_space = fs_info.f_bfree * fs_info.f_frsize;
    unsigned long long used_space = total_space - free_space;

    //Declaracion de variables
    double free_space_units, used_space_units;
    //Calculos para convertir de bytes a otra unidad
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

    //Pasarlo a porcentaje
    double used_space_percent = (used_space * 100.0) / total_space;
    double free_space_percent = (free_space * 100.0) / total_space;

    //Mostrar resultado
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
