//Ignacio Andre Ledezma Hidalgo / 402520080
//Wendy Carballo Chavarria / 207270492

#include <stdio.h>      // libreria estandar para funciones de entrada y salida
#include <stdlib.h>     // para utilizar exit() y mostrar mensajes de SUCCESS o FAILURE
#include <unistd.h>     // libreria que permite utilizar los fork() y pipes
#include <sys/types.h>
#include <sys/wait.h>   // para poder utilizar el wait()
#include <sys/stat.h>   // para obtener info de estadisticas, como el stat que se utiliza en el hijo
#include <fcntl.h>      // libreria para manipular datos, con read(), open() y write()

int main(int argc, char *argv[]) {
    if (argc < 2) { // Si solo se llama al programa pero sin argumentos (por ejemplo si no se especifica
        printf("Ingrese: %s <tipo_estadistica>\n", argv[0]); // el tipo de estadistica ya sea disco, cpu, etc)
        exit(EXIT_FAILURE);
    }
    
    if (access(argv[1], X_OK) == -1) { // Validacion para revisar si existe el archivo ejecutable
        perror("Error en access");     // con el nombre proporcionado
        printf("El archivo ejecutable %s no existe o no tiene permisos de ejecución.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Error en pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Error en fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        setenv("PATH", ".", 1); // Setea el PATH actual a donde se deberian encontrar los hijos
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execvp(argv[1], &argv[1]);  // Aumenta la cantidad de argumentos que puede recibir
        perror("Error en exec");
        exit(EXIT_FAILURE);
    } else {
        close(pipefd[1]);
        char buffer[1024];
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytes_read);
        }
        if (bytes_read == -1) {
            perror("Error en read");
            exit(EXIT_FAILURE);
        }
        wait(NULL);
    }

    return 0;
}
