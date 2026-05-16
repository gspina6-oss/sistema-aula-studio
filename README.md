# Sistema Aula Studio

Progetto in C per la gestione di studenti, prenotazioni e accessi ad un'aula studio universitaria.

## Compilazione

```bash
make
```

## Avvio

```bash
./aula_studio
```

Su Windows/MSYS2 può essere necessario:

```bash
./aula_studio.exe
```

## Test

```bash
make test
```

## Pulizia

```bash
make clean
```

## Struttura

- `include/aula_studio.h`: definizione delle strutture dati e dei prototipi
- `src/aula_studio.c`: implementazione delle funzioni
- `src/main.c`: menu principale
- `test/test_aula_studio.c`: casi di test
- `Makefile`: compilazione del programma e dei test
