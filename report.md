# Sistema di Gestione degli Accessi e delle Prenotazioni di un'Aula Studio

## Introduzione

Il progetto consiste in un programma scritto in linguaggio C per gestire l'utilizzo di un'aula studio universitaria.

Il sistema permette di registrare gli studenti, inserire prenotazioni, controllare i posti disponibili, gestire il check-in, registrare gli ingressi senza prenotazione, mantenere una lista d'attesa e produrre un report sull'occupazione dell'aula.

Il lavoro è stato svolto come progetto di gruppo da due persone, seguendo la traccia relativa alla gestione degli accessi e delle prenotazioni di un'aula studio.

## Struttura del progetto

Il progetto è organizzato in questo modo:

```text
include/
    aula_studio.h

src/
    aula_studio.c
    main.c

test/
    test_aula_studio.c

Makefile
README.md
report.pdf
```

## Motivazione della scelta degli ADT

Per rappresentare il sistema sono stati scelti quattro tipi principali:

- `Studente`
- `Prenotazione`
- `Accesso`
- `AulaStudio`

La struttura `Studente` contiene i dati essenziali dello studente: matricola, nome e corso di laurea.

La struttura `Prenotazione` rappresenta una prenotazione effettuata da uno studente per una data e una fascia oraria. Contiene anche il posto assegnato, lo stato della prenotazione, il check-in e l'eventuale stato di attesa.

La struttura `Accesso` viene usata per memorizzare lo storico degli accessi effettivi all'aula studio.

La struttura `AulaStudio` è l'ADT principale, perché contiene gli array degli studenti, delle prenotazioni, dei presenti, della lista d'attesa e dello storico.

Questa scelta permette di mantenere insieme tutti i dati necessari al funzionamento del programma senza usare variabili sparse nel codice.

## Integrazione degli ADT nel sistema

Gli ADT sono collegati tra loro tramite la matricola dello studente.

Quando uno studente viene registrato, i suoi dati vengono salvati nell'array degli studenti. Quando viene inserita una prenotazione, viene controllato che la matricola esista. Se ci sono posti disponibili, viene assegnato un posto; altrimenti lo studente viene inserito in lista d'attesa.

Quando lo studente effettua il check-in, viene aggiunto alla lista dei presenti e viene registrato un nuovo accesso nello storico.

## Progettazione del sistema

Il programma è diviso in tre parti principali:

- `aula_studio.h`, che contiene strutture dati e prototipi;
- `aula_studio.c`, che contiene la logica del sistema;
- `main.c`, che contiene il menu testuale.

Questa divisione permette di separare la parte di gestione dei dati dalla parte di interazione con l'utente.

Il sistema usa array statici con dimensione massima predefinita. Questa scelta è stata fatta per mantenere il progetto semplice e coerente con il livello richiesto, evitando la gestione dinamica della memoria.

## Specifica sintattica e semantica delle principali operazioni

### `void inizializzaAula(AulaStudio *aula, int postiTotali)`

Inizializza la struttura principale.

Pre-condizioni:
- il puntatore all'aula deve essere valido;
- il numero di posti deve essere positivo.

Post-condizioni:
- tutti i contatori vengono inizializzati a zero;
- viene salvato il numero totale di posti.

Effetti collaterali:
- modifica la struttura `AulaStudio`.

### `int registraStudente(AulaStudio *aula, const char matricola[], const char nome[], const char corso[])`

Registra un nuovo studente.

Pre-condizioni:
- aula inizializzata;
- matricola non già presente;
- spazio disponibile nell'array degli studenti.

Post-condizioni:
- lo studente viene inserito nell'elenco degli studenti.

Input:
- matricola;
- nome;
- corso di laurea.

Output:
- `1` se la registrazione riesce;
- `0` in caso di errore.

Effetti collaterali:
- modifica l'array degli studenti e il contatore degli studenti.

### `int inserisciPrenotazione(AulaStudio *aula, const char matricola[], const char data[], const char fascia[])`

Inserisce una prenotazione.

Pre-condizioni:
- lo studente deve essere registrato;
- non deve esistere già una prenotazione attiva uguale;
- deve esserci spazio nell'array delle prenotazioni.

Post-condizioni:
- viene creata una prenotazione;
- se i posti sono terminati, lo studente viene inserito in lista d'attesa.

Input:
- matricola;
- data;
- fascia oraria.

Output:
- `1` se la prenotazione viene inserita;
- `0` in caso di errore.

Effetti collaterali:
- modifica le prenotazioni;
- può modificare la lista d'attesa.

### `int postiDisponibili(const AulaStudio *aula, const char data[], const char fascia[])`

Calcola i posti disponibili in una data fascia.

Pre-condizioni:
- aula inizializzata.

Post-condizioni:
- non modifica i dati.

Input:
- data;
- fascia oraria.

Output:
- numero di posti disponibili.

### `int checkInStudente(AulaStudio *aula, const char matricola[], const char data[], const char fascia[])`

Registra il check-in di uno studente prenotato.

Pre-condizioni:
- la prenotazione deve esistere;
- lo studente non deve essere già presente;
- la prenotazione non deve essere in lista d'attesa.

Post-condizioni:
- lo studente viene inserito tra i presenti;
- viene salvato un accesso nello storico.

Output:
- `1` in caso di successo;
- `0` in caso di errore.

### `int ingressoSenzaPrenotazione(AulaStudio *aula, const char matricola[], const char data[], const char fascia[])`

Gestisce l'ingresso di uno studente senza prenotazione.

Pre-condizioni:
- lo studente deve essere registrato;
- lo studente non deve essere già presente.

Post-condizioni:
- se c'è posto, lo studente entra;
- se non c'è posto, viene inserito in lista d'attesa.

Output:
- `1` se l'ingresso riesce;
- `2` se lo studente viene inserito in lista d'attesa;
- `0` in caso di errore.

### `int registraUscita(AulaStudio *aula, const char matricola[])`

Registra l'uscita di uno studente presente.

Pre-condizioni:
- lo studente deve essere presente.

Post-condizioni:
- lo studente viene rimosso dall'elenco dei presenti;
- lo storico viene aggiornato.

Output:
- `1` se l'uscita viene registrata;
- `0` se lo studente non era presente.

## Report generato dal programma

Il programma genera un report con:

- numero di studenti registrati;
- numero totale di prenotazioni;
- prenotazioni attive;
- accessi effettivi;
- studenti presenti;
- studenti in attesa;
- studenti prenotati che non si sono presentati;
- occupazione per fascia oraria.

## Casi di test

I test sono stati inseriti nel file `test/test_aula_studio.c`.

I casi di test coprono:

- registrazione degli studenti;
- rifiuto di una matricola duplicata;
- inserimento delle prenotazioni;
- aggiornamento della disponibilità;
- lista d'attesa;
- check-in;
- registrazione dell'uscita;
- storico degli accessi;
- ingresso senza prenotazione;
- annullamento di una prenotazione.

Il razionale dei test è verificare sia i casi corretti sia alcuni casi di errore, come duplicati, posti esauriti e secondo check-in dello stesso studente.

## Compilazione

Il progetto può essere compilato con:

```bash
make
```

Per eseguire i test:

```bash
make test
```

Per pulire i file generati:

```bash
make clean
```

## Ambiente di sviluppo

Il progetto è stato compilato e testato su Windows tramite MSYS2 UCRT64, usando GCC e Make.

Lo standard usato nel Makefile è C99:

```text
-std=c99
```

## Conclusioni

Il progetto implementa le funzionalità richieste dalla traccia: gestione studenti, prenotazioni, disponibilità, check-in, ingresso senza prenotazione, uscita, lista d'attesa, storico e report.

La struttura a file separati rende il codice più ordinato e permette di testare le funzionalità principali in modo indipendente dal menu.
