#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h> // Para trabajar con directorios
#include <sys/types.h> // Para definiciones de tipos
#include <sys/stat.h>  // Para estadísticas de archivos
#define MAX_PROC_PATH 312

int main(int argc, char *argv[]) {
    FILE *fp_meminfo;
    char buffer[256];
    unsigned long mem_total = 0, mem_free = 0, mem_available = 0, swap_free = 0, swap_total = 0;

    fp_meminfo = fopen("/proc/meminfo", "r");
    if (fp_meminfo == NULL) {
        perror("Error al abrir el archivo /proc/meminfo");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp_meminfo)) {
        if (strncmp(buffer, "MemTotal:", 9) == 0) {
            sscanf(buffer, "MemTotal: %lu kB", &mem_total);
        } else if (strncmp(buffer, "MemFree:", 8) == 0) {
            sscanf(buffer, "MemFree: %lu kB", &mem_free);
        } else if (strncmp(buffer, "MemAvailable:", 13) == 0) {
            sscanf(buffer, "MemAvailable: %lu kB", &mem_available);
        } else if (strncmp(buffer, "SwapTotal:", 10) == 0) {
            sscanf(buffer, "SwapTotal: %lu kB", &swap_total);
        } else if (strncmp(buffer, "SwapFree:", 9) == 0) {
            sscanf(buffer, "SwapFree: %lu kB", &swap_free);
        }
    }
    fclose(fp_meminfo);

    double total_gb = mem_total / (1024.0 * 1024.0);
    double used_gb = (mem_total - mem_free) / (1024.0 * 1024.0);
    double free_gb = mem_free / (1024.0 * 1024.0);
    double total_gb_virtual = swap_total / (1024.0 * 1024.0);
    double used_gb_virtual = (swap_total - swap_free) / (1024.0 * 1024.0);
    double free_gb_virtual = swap_free / (1024.0 * 1024.0);
    
    double used_percentage = (used_gb / total_gb) * 100.0;
    double free_percentage = (free_gb / total_gb) * 100.0;
    double used_percentage_virtual = (used_gb_virtual / total_gb_virtual) * 100.0;
    double free_percentage_virtual = (free_gb_virtual / total_gb_virtual) * 100.0;
    
    if (argc <= 1 || (argc > 1 && strcmp(argv[1], "-v") != 0 && strcmp(argv[1], "-r") != 0)) {
        printf("Memoria total del sistema: %.2fGB\n", total_gb);
        printf("Memoria total real utilizada: %.2fGB (%.2f%%)\n", used_gb, used_percentage);
        printf("Memoria total real libre: %.2fGB (%.2f%%)\n", free_gb, free_percentage);
        printf("Memoria total virtual: %.2fGB\n", total_gb_virtual);
        printf("Memoria total virtual utilizada: %.6fGB (%.2f%%)\n", used_gb_virtual, used_percentage_virtual);
        printf("Memoria total virtual libre: %.2fGB (%.2f%%)\n", free_gb_virtual, free_percentage_virtual);
        printf("Uso con argumentos: %s [-r] [-v]\n", argv[0]);
        return EXIT_SUCCESS;
    }
    
    if (strcmp(argv[1], "-r") == 0) {
        DIR *proc_dir = opendir("/proc");
        if (proc_dir == NULL) {
            perror("Error al abrir el directorio /proc");
            return EXIT_FAILURE;
        }
        
        printf("\nPID\tNombre\t\t\tTamano(kB)\n");
        
        struct dirent *entry;
        double total = 0.0;
        while ((entry = readdir(proc_dir)) != NULL) {
            if (entry->d_type == DT_DIR && atoi(entry->d_name) != 0) {
                char *proc_path = malloc(MAX_PROC_PATH);
                FILE *status_file;
                char bufferProc[1024];
                char name[256];
                char *rss = NULL;
                
                snprintf(proc_path, MAX_PROC_PATH, "/proc/%s/status", entry->d_name);

                status_file = fopen(proc_path, "r");
                if (status_file == NULL) {
                    perror("Error al abrir el archivo de estado del proceso");
                    continue;
                }

                while (fgets(bufferProc, sizeof(bufferProc), status_file) != NULL) {
                    if (strncmp(bufferProc, "Name:", 5) == 0) {
                        sscanf(bufferProc, "Name: %[^\n]", name);
                    } 
                    else if (strncmp(bufferProc, "VmRSS:", 6) == 0) {
                        char rssBuffer[32];
                        sscanf(bufferProc, "VmRSS: %s", rssBuffer);
                        rss = strtok(rssBuffer, " ");
                    }
                }
                if (rss != NULL) {
                printf("%s\t%-20s\t%s\t", entry->d_name, name, rss);
                double process_size_kb = atof(rss);
                double process_percentage = ((process_size_kb / (used_gb * 1024))) / 10;
                printf("(%.2f%%)\n", process_percentage);
                total += process_size_kb;
            }
                fclose(status_file);
            }
        }
        closedir(proc_dir);
        printf("Memoria total del sistema: %.2fGB\n", total_gb);
        printf("Memoria total real utilizada: %.2fGB (%.2f%%)\n", used_gb, used_percentage);
        printf("Memoria total real libre: %.2fGB (%.2f%%)\n", free_gb, free_percentage);
    }
    
    if (strcmp(argv[1], "-v") == 0) {
        printf("Memoria total virtual: %.2fGB\n", total_gb_virtual);
        printf("Memoria total virtual utilizada: %.6fGB (%.2f%%)\n", used_gb_virtual, used_percentage_virtual);
        printf("Memoria total virtual libre: %.2fGB (%.2f%%)\n", free_gb_virtual, free_percentage_virtual);
    }
    
    return 0;
}