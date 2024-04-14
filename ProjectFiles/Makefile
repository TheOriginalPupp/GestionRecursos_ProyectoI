CC=gcc #Compilador
CFLAGS=-I. -Wall #Opciones de compilacion (directorio actual)
LDLIBS=

EXECUTABLE=programa #Padre que queremos compilar
CHILDREN=cpu memoria disco #Hijos 

all: $(EXECUTABLE) $(CHILDREN) #Compilar

# Regla para el padre (enlaza para crear el ejecutable)
$(EXECUTABLE): programa.o
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






