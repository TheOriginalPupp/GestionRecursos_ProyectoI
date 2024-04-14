# GestionRecursos_ProyectoI
Proyecto de Gestion de Recursos, que permite obtener estadisticas de recursos del sistema.
Sistemas Operativos 2024 UNA

Requerimientos:
1) Se necesitan tener 4 codigos, uno para el proceso padre, y tres para los procesos hijos.
2) Cada proceso hijo debe mostrar una estadistica diferente.
3) Por ejemplo, el hijo 1 debe mostrar la utilizacion de CPU para todos los procesos o para un proceso especifico (segun el PID) en los ultimos 5 minutos.
4) Para el hijo 2, es necesario obtener el porcentaje de memoria virtual para todos los procesos (esto incluye PID, nombre y porcentaje de utilizacion); de la misma manera mostrar la misma informacion para la memoria real.
5) Para el hijo 3, es necesario obtener el porcentaje de utilizacion del disco ya sea en MiB o GiB; tambien debe ser capaz de mostrar el porcentaje total de espacio libre en MiB o GiB.
6) El proceso Padre lo que hace es utilizar un archivo MAKEFILE. Esta llamada al proceso principal va a tener ARGS para determinar el tipo de estadistica a resolver (ya sea CPU, Memoria o Disco), y de ser necesario el process ID.
7) A la hora de compilar el MAKEFILE se generan ejecutables, los cuales se guardan en USR/LOCAL/BIN.
8) Este MAKEFILE actualiza el PATH para que agregue LOCAL. Es necesario asegurarse de que el proceso padre TENGA EL PATH ACTUALIZADO.
9) Si se hace una llamada, por ejemplo a la estadistica CPU con el comando ./PADRE CPU 1234, este no debe realizar la llamada al fork() del hijo, sino que el PADRE propiamente es quien las imprime.

Comandos:
- ./programa cpu (muestra la informacion de todos los procesos que estan corriendo en el CPU con el comando top, y el porcentaje de utilizacion de CPU total)
- ./programa cpu [PID] (muestra la informacion del proceso con ese PID en los ultimos 5 minutos)
- ./programa mem (por definir virtual)
- ./programa mem (por definir real)
- ./programa disco (muestra el porcentaje de espacio utilizado y espacio libre del disco en KB)
- ./programa disco -tm (muestra el porcentaje de espacio utilizado y espacio libre del disco en MiB)
- ./programa disco -tg (muestra el porcentaje de espacio utilizado y espacio libre del disco en GiB)

Referencias Bibliograficas:
- https://www.site24x7.com/learn/linux/optimize-memory.html
- https://access.redhat.com/documentation/es-es/red_hat_enterprise_linux/4/html/reference_guide/ch-proc
- https://www.kernel.org/doc/html/latest/filesystems/proc.html
- https://tldp.org/LDP/Linux-Filesystem-Hierarchy/html/proc.html
- https://www.geeksforgeeks.org/proc-file-system-linux/
- https://docs.oracle.com/cd/E24842_01/html/E23086/spmonitor-22.html
- https://manpages.ubuntu.com/manpages/trusty/es/man8/vmstat.8.html
- https://www.ibm.com/docs/es/power7?topic=commands-vmstat-command
- https://www.cyberciti.biz/faq/how-to-check-running-process-in-linux-using-command-line/ (Muestra como usar el ps -aux y el top para la parte del CPU Usage)
