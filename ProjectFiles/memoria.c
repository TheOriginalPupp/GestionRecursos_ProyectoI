#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> //Para trabajar con directorios
#include <sys/types.h> // Para definiciones de tipos
#include <sys/stat.h>  // Para estadísticas de archivos


int main(int argc, char *argv[]) {
    //Variables
    DIR *dir;
    int pid;
    unsigned long vm, rss; // Variables Virtual memory y Resident set size
    unsigned long vm_total = 0, rss_total = 0;
    //Variables para seleccionar
    int use_real = 0;
    int use_virtual = 0;

    //Guarda el tamanio de pagina del sistema
    int page_size = getpagesize();

    //Navegar en los directorios de /proc
    struct dirent *entry;
    struct stat statbuf;
    FILE *fp;
    char path[1024], line[256];

    //Revisa los argumentos pasados al programa (usa real si -r y virtual si -v)
    for (int i = 1; i < argc; i++) {  
        if (strcasecmp(argv[i], "-r") == 0) {
            use_real = 1;  
        }
        else if (strcasecmp(argv[i], "-v") == 0) {
            use_virtual = 1;  
        }
    }

    // Abrir directorio /proc 
    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Error al abrir el archivo /proc");
        exit(EXIT_FAILURE);
    }

    // Variables para leer la memoria total desde /proc/meminfo 
    FILE *fp_meminfo = fopen("/proc/meminfo", "r");
    char buffer[256];
    unsigned long mem_total = 0;

     // Leer la memoria total del sistema 
    if (fp_meminfo == NULL) {
        perror("Error al abrir el archivo /proc/meminfo");
        exit(EXIT_FAILURE);
    }

    //Procesar las entradas en /proc
    while ((entry = readdir(dir)) != NULL) {
        sprintf(path, "/proc/%s", entry->d_name);
        if (stat(path, &statbuf) == 0) {
            if (S_ISDIR(statbuf.st_mode)) {
                pid = atoi(entry->d_name);
                if (pid > 0) {
                    sprintf(path, "/proc/%d/statm", pid);
                    fp = fopen(path, "r");
                    if (fp) {
                        if (fgets(line, sizeof(line), fp)) {
                            sscanf(line, "%lu %lu", &vm, &rss);
                            vm_total += vm * page_size;
                            rss_total += rss * page_size;
                        }
                        fclose(fp);
                    }
                }
            }
        }
    }
    closedir(dir);

    while (fgets(buffer, sizeof(buffer), fp_meminfo)) {
        if (strncmp(buffer, "MemTotal:", 9) == 0) {
            sscanf(buffer, "MemTotal: %lu kB", &mem_total);
            break;
        }
    }
    fclose(fp_meminfo);
    //Convertir de kB a bytes
    mem_total *= 1024; 

    //Convertir a GB y mostrar segun los argumentos
    if (use_real) {
        // Mostrar solo la memoria real
        printf("Memoria real total utilizada: %.2f GB (%.2f%%)\n", rss_total / (double)(1024 * 1024 * 1024), (rss_total * 100.0) / mem_total);
    } else if (use_virtual) {
        // Mostrar solo la memoria virtual
        printf("Memoria virtual total utilizada: %.2f GB (%.2f%%)\n", vm_total / (double)(1024 * 1024 * 1024), (vm_total * 100.0) / mem_total);
    } else {
        // Mostrar ambas memorias
        printf("Memoria total del sistema: %.2f GB\n", mem_total / (double)(1024 * 1024 * 1024));
        printf("Memoria virtual total utilizada: %.2f GB (%.2f%%)\n", vm_total / (double)(1024 * 1024 * 1024), (vm_total * 100.0) / mem_total);
        printf("Memoria real total utilizada: %.2f GB (%.2f%%)\n", rss_total / (double)(1024 * 1024 * 1024), (rss_total * 100.0) / mem_total);
    }

    return 0;
}

