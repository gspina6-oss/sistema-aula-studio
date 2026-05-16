#ifndef AULA_STUDIO_H
#define AULA_STUDIO_H

#define MAX_STUDENTI 100
#define MAX_PRENOTAZIONI 200
#define MAX_PRESENTI 100
#define MAX_ATTESA 100
#define MAX_STORICO 300

#define MAX_MATRICOLA 20
#define MAX_NOME 50
#define MAX_CORSO 50
#define MAX_DATA 11
#define MAX_FASCIA 20

typedef enum {
    FALSO = 0,
    VERO = 1
} Boolean;

/* Dati principali di uno studente registrato. */
typedef struct {
    char matricola[MAX_MATRICOLA];
    char nome[MAX_NOME];
    char corsoLaurea[MAX_CORSO];
} Studente;

/* Dati di una prenotazione per data e fascia oraria. */
typedef struct {
    char matricola[MAX_MATRICOLA];
    char data[MAX_DATA];
    char fasciaOraria[MAX_FASCIA];
    int postoAssegnato;
    Boolean attiva;
    Boolean checkIn;
    Boolean inAttesa;
} Prenotazione;

/* Dati salvati nello storico degli accessi. */
typedef struct {
    char matricola[MAX_MATRICOLA];
    char data[MAX_DATA];
    char fasciaOraria[MAX_FASCIA];
    int posto;
    Boolean presente;
} Accesso;

/* Struttura principale che contiene lo stato dell'aula studio. */
typedef struct {
    Studente studenti[MAX_STUDENTI];
    Prenotazione prenotazioni[MAX_PRENOTAZIONI];
    Studente presenti[MAX_PRESENTI];
    Studente attesa[MAX_ATTESA];
    Accesso storico[MAX_STORICO];

    int numeroStudenti;
    int numeroPrenotazioni;
    int numeroPresenti;
    int numeroAttesa;
    int numeroStorico;

    int postiTotali;
} AulaStudio;

void inizializzaAula(AulaStudio *aula, int postiTotali);

int registraStudente(AulaStudio *aula, const char matricola[], const char nome[], const char corso[]);
int cercaStudente(const AulaStudio *aula, const char matricola[]);

int inserisciPrenotazione(AulaStudio *aula, const char matricola[], const char data[], const char fascia[]);
int annullaPrenotazione(AulaStudio *aula, const char matricola[], const char data[], const char fascia[]);
int cercaPrenotazione(const AulaStudio *aula, const char matricola[], const char data[], const char fascia[]);
int postiDisponibili(const AulaStudio *aula, const char data[], const char fascia[]);

int checkInStudente(AulaStudio *aula, const char matricola[], const char data[], const char fascia[]);
int ingressoSenzaPrenotazione(AulaStudio *aula, const char matricola[], const char data[], const char fascia[]);
int registraUscita(AulaStudio *aula, const char matricola[]);

void stampaStudenti(const AulaStudio *aula);
void stampaPrenotati(const AulaStudio *aula, const char data[], const char fascia[]);
void stampaPresenti(const AulaStudio *aula);
void stampaListaAttesa(const AulaStudio *aula);
void stampaStorico(const AulaStudio *aula);
void stampaReport(const AulaStudio *aula);

#endif
