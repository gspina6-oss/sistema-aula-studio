#include <stdio.h>
#include <string.h>
#include "../include/aula_studio.h"

static void copiaStringa(char destinazione[], const char origine[], int limite) {
    strncpy(destinazione, origine, limite - 1);
    destinazione[limite - 1] = '\0';
}

static int cercaPresente(const AulaStudio *aula, const char matricola[]) {
    int i;

    for (i = 0; i < aula->numeroPresenti; i++) {
        if (strcmp(aula->presenti[i].matricola, matricola) == 0)
            return i;
    }

    return -1;
}

static int cercaAccessoAttivo(const AulaStudio *aula, const char matricola[], const char data[], const char fascia[]) {
    int i;

    for (i = 0; i < aula->numeroStorico; i++) {
        if (aula->storico[i].presente == VERO &&
            strcmp(aula->storico[i].matricola, matricola) == 0 &&
            strcmp(aula->storico[i].data, data) == 0 &&
            strcmp(aula->storico[i].fasciaOraria, fascia) == 0) {
            return i;
        }
    }

    return -1;
}

static int contaOccupati(const AulaStudio *aula, const char data[], const char fascia[]) {
    int i;
    int occupati = 0;

    for (i = 0; i < aula->numeroPrenotazioni; i++) {
        if (aula->prenotazioni[i].attiva == VERO &&
            aula->prenotazioni[i].inAttesa == FALSO &&
            strcmp(aula->prenotazioni[i].data, data) == 0 &&
            strcmp(aula->prenotazioni[i].fasciaOraria, fascia) == 0) {
            occupati++;
        }
    }

    for (i = 0; i < aula->numeroStorico; i++) {
        if (aula->storico[i].presente == VERO &&
            strcmp(aula->storico[i].data, data) == 0 &&
            strcmp(aula->storico[i].fasciaOraria, fascia) == 0 &&
            cercaPrenotazione(aula, aula->storico[i].matricola, data, fascia) == -1) {
            occupati++;
        }
    }

    return occupati;
}

static int aggiungiPresente(AulaStudio *aula, const char matricola[]) {
    int indice;

    if (aula->numeroPresenti >= MAX_PRESENTI)
        return 0;

    if (cercaPresente(aula, matricola) != -1)
        return 0;

    indice = cercaStudente(aula, matricola);
    if (indice == -1)
        return 0;

    aula->presenti[aula->numeroPresenti] = aula->studenti[indice];
    aula->numeroPresenti++;

    return 1;
}

static int aggiungiStorico(AulaStudio *aula, const char matricola[], const char data[], const char fascia[], int posto) {
    Accesso *accesso;

    if (aula->numeroStorico >= MAX_STORICO)
        return 0;

    accesso = &aula->storico[aula->numeroStorico];

    copiaStringa(accesso->matricola, matricola, MAX_MATRICOLA);
    copiaStringa(accesso->data, data, MAX_DATA);
    copiaStringa(accesso->fasciaOraria, fascia, MAX_FASCIA);
    accesso->posto = posto;
    accesso->presente = VERO;

    aula->numeroStorico++;

    return 1;
}

static void rimuoviDaAttesa(AulaStudio *aula, const char matricola[]) {
    int i;
    int j;

    for (i = 0; i < aula->numeroAttesa; i++) {
        if (strcmp(aula->attesa[i].matricola, matricola) == 0) {
            for (j = i; j < aula->numeroAttesa - 1; j++)
                aula->attesa[j] = aula->attesa[j + 1];

            aula->numeroAttesa--;
            return;
        }
    }
}

void inizializzaAula(AulaStudio *aula, int postiTotali) {
    aula->numeroStudenti = 0;
    aula->numeroPrenotazioni = 0;
    aula->numeroPresenti = 0;
    aula->numeroAttesa = 0;
    aula->numeroStorico = 0;
    aula->postiTotali = postiTotali;
}

int cercaStudente(const AulaStudio *aula, const char matricola[]) {
    int i;

    for (i = 0; i < aula->numeroStudenti; i++) {
        if (strcmp(aula->studenti[i].matricola, matricola) == 0)
            return i;
    }

    return -1;
}

int registraStudente(AulaStudio *aula, const char matricola[], const char nome[], const char corso[]) {
    Studente *studente;

    if (aula->numeroStudenti >= MAX_STUDENTI)
        return 0;

    if (cercaStudente(aula, matricola) != -1)
        return 0;

    studente = &aula->studenti[aula->numeroStudenti];

    copiaStringa(studente->matricola, matricola, MAX_MATRICOLA);
    copiaStringa(studente->nome, nome, MAX_NOME);
    copiaStringa(studente->corsoLaurea, corso, MAX_CORSO);

    aula->numeroStudenti++;

    return 1;
}

int cercaPrenotazione(const AulaStudio *aula, const char matricola[], const char data[], const char fascia[]) {
    int i;

    for (i = 0; i < aula->numeroPrenotazioni; i++) {
        if (aula->prenotazioni[i].attiva == VERO &&
            strcmp(aula->prenotazioni[i].matricola, matricola) == 0 &&
            strcmp(aula->prenotazioni[i].data, data) == 0 &&
            strcmp(aula->prenotazioni[i].fasciaOraria, fascia) == 0) {
            return i;
        }
    }

    return -1;
}

int postiDisponibili(const AulaStudio *aula, const char data[], const char fascia[]) {
    return aula->postiTotali - contaOccupati(aula, data, fascia);
}

int inserisciPrenotazione(AulaStudio *aula, const char matricola[], const char data[], const char fascia[]) {
    int indiceStudente;
    int disponibili;
    Prenotazione *prenotazione;

    if (aula->numeroPrenotazioni >= MAX_PRENOTAZIONI)
        return 0;

    indiceStudente = cercaStudente(aula, matricola);
    if (indiceStudente == -1)
        return 0;

    if (cercaPrenotazione(aula, matricola, data, fascia) != -1)
        return 0;

    if (cercaAccessoAttivo(aula, matricola, data, fascia) != -1)
        return 0;

    disponibili = postiDisponibili(aula, data, fascia);

    if (disponibili <= 0 && aula->numeroAttesa >= MAX_ATTESA)
        return 0;

    prenotazione = &aula->prenotazioni[aula->numeroPrenotazioni];

    copiaStringa(prenotazione->matricola, matricola, MAX_MATRICOLA);
    copiaStringa(prenotazione->data, data, MAX_DATA);
    copiaStringa(prenotazione->fasciaOraria, fascia, MAX_FASCIA);

    prenotazione->attiva = VERO;
    prenotazione->checkIn = FALSO;

    if (disponibili > 0) {
        prenotazione->postoAssegnato = aula->postiTotali - disponibili + 1;
        prenotazione->inAttesa = FALSO;
    } else {
        prenotazione->postoAssegnato = -1;
        prenotazione->inAttesa = VERO;
        aula->attesa[aula->numeroAttesa] = aula->studenti[indiceStudente];
        aula->numeroAttesa++;
    }

    aula->numeroPrenotazioni++;

    return 1;
}

int annullaPrenotazione(AulaStudio *aula, const char matricola[], const char data[], const char fascia[]) {
    int indice;

    indice = cercaPrenotazione(aula, matricola, data, fascia);
    if (indice == -1)
        return 0;

    if (aula->prenotazioni[indice].checkIn == VERO)
        return 0;

    if (aula->prenotazioni[indice].inAttesa == VERO)
        rimuoviDaAttesa(aula, matricola);

    aula->prenotazioni[indice].attiva = FALSO;

    return 1;
}

int checkInStudente(AulaStudio *aula, const char matricola[], const char data[], const char fascia[]) {
    int indice;
    Prenotazione *prenotazione;

    indice = cercaPrenotazione(aula, matricola, data, fascia);
    if (indice == -1)
        return 0;

    prenotazione = &aula->prenotazioni[indice];

    if (prenotazione->inAttesa == VERO)
        return 0;

    if (prenotazione->checkIn == VERO)
        return 0;

    if (!aggiungiPresente(aula, matricola))
        return 0;

    prenotazione->checkIn = VERO;

    return aggiungiStorico(aula, matricola, data, fascia, prenotazione->postoAssegnato);
}

int ingressoSenzaPrenotazione(AulaStudio *aula, const char matricola[], const char data[], const char fascia[]) {
    int indiceStudente;
    int disponibili;
    int posto;

    indiceStudente = cercaStudente(aula, matricola);
    if (indiceStudente == -1)
        return 0;

    if (cercaPresente(aula, matricola) != -1)
        return 0;

    if (cercaPrenotazione(aula, matricola, data, fascia) != -1)
        return 0;

    disponibili = postiDisponibili(aula, data, fascia);

    if (disponibili <= 0) {
        if (aula->numeroAttesa >= MAX_ATTESA)
            return 0;

        aula->attesa[aula->numeroAttesa] = aula->studenti[indiceStudente];
        aula->numeroAttesa++;

        return 2;
    }

    posto = aula->postiTotali - disponibili + 1;

    if (!aggiungiPresente(aula, matricola))
        return 0;

    return aggiungiStorico(aula, matricola, data, fascia, posto);
}

int registraUscita(AulaStudio *aula, const char matricola[]) {
    int indice;
    int i;

    indice = cercaPresente(aula, matricola);
    if (indice == -1)
        return 0;

    for (i = indice; i < aula->numeroPresenti - 1; i++)
        aula->presenti[i] = aula->presenti[i + 1];

    aula->numeroPresenti--;

    for (i = aula->numeroStorico - 1; i >= 0; i--) {
        if (strcmp(aula->storico[i].matricola, matricola) == 0 &&
            aula->storico[i].presente == VERO) {
            aula->storico[i].presente = FALSO;
            break;
        }
    }

    return 1;
}

void stampaStudenti(const AulaStudio *aula) {
    int i;

    printf("\nStudenti registrati:\n");

    if (aula->numeroStudenti == 0) {
        printf("Nessuno studente registrato.\n");
        return;
    }

    for (i = 0; i < aula->numeroStudenti; i++) {
        printf("%s - %s - %s\n",
               aula->studenti[i].matricola,
               aula->studenti[i].nome,
               aula->studenti[i].corsoLaurea);
    }
}

void stampaPrenotati(const AulaStudio *aula, const char data[], const char fascia[]) {
    int i;
    int trovati = 0;

    printf("\nPrenotazioni del %s, fascia %s:\n", data, fascia);

    for (i = 0; i < aula->numeroPrenotazioni; i++) {
        if (aula->prenotazioni[i].attiva == VERO &&
            aula->prenotazioni[i].inAttesa == FALSO &&
            strcmp(aula->prenotazioni[i].data, data) == 0 &&
            strcmp(aula->prenotazioni[i].fasciaOraria, fascia) == 0) {
            printf("%s - posto %d",
                   aula->prenotazioni[i].matricola,
                   aula->prenotazioni[i].postoAssegnato);

            if (aula->prenotazioni[i].checkIn == VERO)
                printf(" - presente");

            printf("\n");
            trovati = 1;
        }
    }

    if (!trovati)
        printf("Nessuna prenotazione trovata.\n");
}

void stampaPresenti(const AulaStudio *aula) {
    int i;

    printf("\nStudenti presenti:\n");

    if (aula->numeroPresenti == 0) {
        printf("Nessuno studente presente.\n");
        return;
    }

    for (i = 0; i < aula->numeroPresenti; i++) {
        printf("%s - %s\n",
               aula->presenti[i].matricola,
               aula->presenti[i].nome);
    }
}

void stampaListaAttesa(const AulaStudio *aula) {
    int i;

    printf("\nLista d'attesa:\n");

    if (aula->numeroAttesa == 0) {
        printf("Lista vuota.\n");
        return;
    }

    for (i = 0; i < aula->numeroAttesa; i++) {
        printf("%d. %s - %s\n",
               i + 1,
               aula->attesa[i].matricola,
               aula->attesa[i].nome);
    }
}

void stampaStorico(const AulaStudio *aula) {
    int i;

    printf("\nStorico accessi:\n");

    if (aula->numeroStorico == 0) {
        printf("Nessun accesso registrato.\n");
        return;
    }

    for (i = 0; i < aula->numeroStorico; i++) {
        printf("%s - %s - %s - posto %d - %s\n",
               aula->storico[i].matricola,
               aula->storico[i].data,
               aula->storico[i].fasciaOraria,
               aula->storico[i].posto,
               aula->storico[i].presente == VERO ? "presente" : "uscito");
    }
}

static int fasciaGiaStampata(const AulaStudio *aula, int indiceCorrente) {
    int i;

    for (i = 0; i < indiceCorrente; i++) {
        if (strcmp(aula->prenotazioni[i].data, aula->prenotazioni[indiceCorrente].data) == 0 &&
            strcmp(aula->prenotazioni[i].fasciaOraria, aula->prenotazioni[indiceCorrente].fasciaOraria) == 0) {
            return 1;
        }
    }

    return 0;
}

void stampaReport(const AulaStudio *aula) {
    int i;
    int prenotazioniAttive = 0;
    int accessiEffettivi = aula->numeroStorico;
    int nonPresentati = 0;

    for (i = 0; i < aula->numeroPrenotazioni; i++) {
        if (aula->prenotazioni[i].attiva == VERO) {
            prenotazioniAttive++;

            if (aula->prenotazioni[i].inAttesa == FALSO &&
                aula->prenotazioni[i].checkIn == FALSO) {
                nonPresentati++;
            }
        }
    }

    printf("\nReport generale:\n");
    printf("Studenti registrati: %d\n", aula->numeroStudenti);
    printf("Prenotazioni totali: %d\n", aula->numeroPrenotazioni);
    printf("Prenotazioni attive: %d\n", prenotazioniAttive);
    printf("Accessi effettivi: %d\n", accessiEffettivi);
    printf("Studenti presenti: %d\n", aula->numeroPresenti);
    printf("Studenti in attesa: %d\n", aula->numeroAttesa);
    printf("Prenotati non presentati: %d\n", nonPresentati);

    printf("\nOccupazione per fascia oraria:\n");

    if (aula->numeroPrenotazioni == 0) {
        printf("Nessuna fascia registrata.\n");
        return;
    }

    for (i = 0; i < aula->numeroPrenotazioni; i++) {
        if (aula->prenotazioni[i].attiva == VERO && !fasciaGiaStampata(aula, i)) {
            printf("%s %s: %d/%d posti occupati\n",
                   aula->prenotazioni[i].data,
                   aula->prenotazioni[i].fasciaOraria,
                   contaOccupati(aula, aula->prenotazioni[i].data, aula->prenotazioni[i].fasciaOraria),
                   aula->postiTotali);
        }
    }
}
