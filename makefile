cc = gcc
terget = main.exe
src = src/main.c src/processador.c

all: $(target)

$(target): $(src)
	$(cc) $(src) -o $(target)

clean:
	rm -f $(target)