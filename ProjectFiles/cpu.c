#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_PID_LENGTH 16
#define MAX_COMMAND_LENGTH 1024
#define MAX_COMMAND_DISPLAY_LENGTH 50  

typedef struct {
    char pid[MAX_PID_LENGTH];
    float cpu_usage;
    char command[MAX_COMMAND_LENGTH];
} ProcessInfo;

float getUptimeMinutes() {
    FILE *uptime_output = popen("/usr/bin/uptime", "r");
    if (uptime_output == NULL) {
        perror("Error al ejecutar el comando uptime");
        return -1;
    }

    char buffer[MAX_COMMAND_LENGTH];
    fgets(buffer, sizeof(buffer), uptime_output);
    float uptime_minutes;
    sscanf(buffer, "up %f minutes", &uptime_minutes);
    pclose(uptime_output);
    
    return uptime_minutes;
}

float getAverageCPUUsage(const char *pid) {
    FILE *ps_output = popen("/usr/bin/ps -aux", "r");
    if (ps_output == NULL) {
        perror("Error al ejecutar el comando ps");
        return -1;
    }

    char buffer[MAX_COMMAND_LENGTH];
    fgets(buffer, sizeof(buffer), ps_output);
    fgets(buffer, sizeof(buffer), ps_output);
    float cpu_usage;
    sscanf(buffer, "%f", &cpu_usage);
    pclose(ps_output);

    return cpu_usage;
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        char *target_pid = argv[1];
        float cpu_usage = getAverageCPUUsage(target_pid);
        FILE *ps_output = popen("/usr/bin/ps -aux", "r");
        if (ps_output == NULL) {
            perror("Error al ejecutar el comando ps");
            return EXIT_FAILURE;
        }

        char buffer[MAX_COMMAND_LENGTH];
        fgets(buffer, sizeof(buffer), ps_output);
        int pid_found = 0;

        while (fgets(buffer, sizeof(buffer), ps_output) != NULL) {
            char *token;
            char pid[MAX_PID_LENGTH];
            float cpu_usage;
            char command[MAX_COMMAND_LENGTH];

            token = strtok(buffer, " ");
            int field = 0;
            while (token != NULL) {
                if (field == 1) strcpy(pid, token);
                else if (field == 2) sscanf(token, "%f", &cpu_usage);
                else if (field == 10) strcpy(command, token);
                token = strtok(NULL, " ");
                field++;
            }

            if (strcmp(pid, target_pid) == 0) {
                pid_found = 1;
                printf("PID\tUso de CPU\tComando\n");
                if (strlen(command) > MAX_COMMAND_DISPLAY_LENGTH) {
                    command[MAX_COMMAND_DISPLAY_LENGTH] = '\0';
                }
                printf("%s\t%.2f%%\t\t%s\n", pid, cpu_usage, command);
                break;
            }
        }
        printf("Uso promedio de CPU para el PID %s en los últimos 5 minutos: %.2f%%\n", target_pid, cpu_usage);

        if (!pid_found) {
            printf("El PID especificado (%s) no fue encontrado.\n", target_pid);
        }

        pclose(ps_output);
    } else if (argc == 1) {
        float cpu_usage_total = 0.0;
        FILE *ps_output = popen("/usr/bin/ps -aux --sort -c", "r");
        if (ps_output == NULL) {
            perror("Error al ejecutar el comando ps");
            return EXIT_FAILURE;
        }

        printf("PID\tUso de CPU\tComando\n");

        char buffer[MAX_COMMAND_LENGTH];
        fgets(buffer, sizeof(buffer), ps_output);
        while (fgets(buffer, sizeof(buffer), ps_output) != NULL) {
            char *token;
            char pid[MAX_PID_LENGTH];
            float cpu_usage;
            char command[MAX_COMMAND_LENGTH];

            token = strtok(buffer, " ");
            int field = 0;
            while (token != NULL) {
                if (field == 1) strcpy(pid, token);
                else if (field == 2) sscanf(token, "%f", &cpu_usage);
                else if (field == 10) strcpy(command, token);
                token = strtok(NULL, " ");
                field++;
            }
            cpu_usage_total += cpu_usage;

            if (strlen(command) > MAX_COMMAND_DISPLAY_LENGTH) {
                command[MAX_COMMAND_DISPLAY_LENGTH] = '\0';
            }
            printf("%s\t%.2f%%\t\t%s\n", pid, cpu_usage, command);
        }
        printf("Porcentaje total de CPU utilizado por todos los procesos: %.2f%%\n", cpu_usage_total);
        pclose(ps_output);
    } else {
        printf("Uso: %s [PID]\n", argv[0]);
        printf("No utilice argumentos si quiere mostrar informacion de todos los procesos.\n");
        printf("Utilice el PID para mostrar informacion de un proceso especifico en los ultimos 5 minutos.\n");
        return EXIT_FAILURE;
    }

    return 0;
}
