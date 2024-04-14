#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//Falta agregar comentarios

#define MAX_PID_LENGTH 16
#define MAX_COMMAND_LENGTH 1024

typedef struct {
    char pid[MAX_PID_LENGTH];
    float cpu_usage;
    char command[MAX_COMMAND_LENGTH];
} ProcessInfo;

int main() {
    const char* top_command = "top -b -n 1";

    ProcessInfo process_info[1024];
    int num_processes = 0;

    FILE* top_output = popen(top_command, "r");
    if (top_output == NULL) {
        perror("Error al ejecutar el comando top");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_COMMAND_LENGTH];
    float suma_cpu_total = 0.0;
    for (int i = 0; i < 7; ++i) {
        if (fgets(buffer, sizeof(buffer), top_output) == NULL) {
            perror("Error al leer la salida del comando top");
            exit(EXIT_FAILURE);
        }
    }
    
    while (fgets(buffer, sizeof(buffer), top_output) != NULL && num_processes < 1024) {
        char pid[MAX_PID_LENGTH];
        float cpu_usage;
        char command[MAX_COMMAND_LENGTH];
        
        sscanf(buffer, "%s %*s %*s %*s %*s %*s %*s %*s %f %*s %*s %s", pid, &cpu_usage, command);
        
        int pid_duplicado = 0; //este for se utiliza para asegurarse de que si un proceso apareciera en la lista
        for (int i = 1; i < num_processes; i++) { //mas de una vez, no se cuente para el total de uso de CPU
            if (strcmp(process_info[i].pid, pid) == 0) {
                pid_duplicado = 1;
                break;
            }
        }
        
        if(!pid_duplicado) {
            strcpy(process_info[num_processes].pid, pid);
            process_info[num_processes].cpu_usage = cpu_usage;
            strcpy(process_info[num_processes].command, command);
            num_processes++;
        }
    }
    
    for (int i = 1; i < num_processes; i++) {
        suma_cpu_total += process_info[i].cpu_usage;
    }

    fclose(top_output);

    printf("PID\tUso de CPU\tComando\n");
    for (int i = 1; i < num_processes; i++) {
        if(process_info[i].cpu_usage < 0.5){
           printf("%s\t%.4f%%\t\t%s\n", process_info[i].pid, process_info[i].cpu_usage, process_info[i].command);
        }
        else printf("%s\t%.2f%%\t\t%s\n", process_info[i].pid, process_info[i].cpu_usage, process_info[i].command);
    }
    printf("Porcentaje de utilizacion de CPU total: %.2f%%\n", suma_cpu_total);
    
    return 0;
}
