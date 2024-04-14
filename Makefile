CC=gcc #Compilador
CFLAGS=-I. -Wall #Opciones de compilacion (directorio actual)
LDLIBS=

EXECUTABLE=programa_principal #Padre que queremos compilar
CHILDREN=programa_cpu programa_memoria programa_disco #Hijos 

all: $(EXECUTABLE) $(CHILDREN) #Compilar

# Regla para el padre (enlaza para crear el ejecutable)
$(EXECUTABLE): programa_principal.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Reglas para los hijos
$(CHILDREN): %: %.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Compilar cada archivo
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Limpiar (elimina archivos objeto y ejecutable)
clean:
	rm -f *.o $(EXECUTABLE) $(CHILDREN)






