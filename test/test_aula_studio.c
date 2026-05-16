#include <stdio.h>
#include "../include/aula_studio.h"

static int testFalliti = 0;

static void controlla(const char nomeTest[], int condizione) {
    if (condizione) {
        printf("[OK] %s\n", nomeTest);
    } else {
        printf("[ERRORE] %s\n", nomeTest);
        testFalliti++;
    }
}

static void testRegistrazioneStudenti(void) {
    AulaStudio aula;

    inizializzaAula(&aula, 2);

    /* Verifica inserimento, duplicati e ricerca. */
    controlla("registrazione primo studente",
              registraStudente(&aula, "0512100001", "Mario Rossi", "Informatica") == 1);

    controlla("rifiuto matricola duplicata",
              registraStudente(&aula, "0512100001", "Mario Rossi", "Informatica") == 0);

    controlla("ricerca studente registrato",
              cercaStudente(&aula, "0512100001") != -1);
}

static void testPrenotazioniDisponibilita(void) {
    AulaStudio aula;

    inizializzaAula(&aula, 2);

    registraStudente(&aula, "0512100001", "Mario Rossi", "Informatica");
    registraStudente(&aula, "0512100002", "Luca Bianchi", "Economia");

    /* Verifica che le prenotazioni aggiornino i posti disponibili. */
    controlla("prenotazione valida",
              inserisciPrenotazione(&aula, "0512100001", "20/05/2026", "09-11") == 1);

    controlla("aggiornamento posti disponibili",
              postiDisponibili(&aula, "20/05/2026", "09-11") == 1);

    controlla("rifiuto prenotazione duplicata",
              inserisciPrenotazione(&aula, "0512100001", "20/05/2026", "09-11") == 0);

    controlla("seconda prenotazione valida",
              inserisciPrenotazione(&aula, "0512100002", "20/05/2026", "09-11") == 1);

    controlla("posti esauriti",
              postiDisponibili(&aula, "20/05/2026", "09-11") == 0);
}

static void testListaAttesa(void) {
    AulaStudio aula;

    inizializzaAula(&aula, 1);

    registraStudente(&aula, "0512100001", "Mario Rossi", "Informatica");
    registraStudente(&aula, "0512100002", "Luca Bianchi", "Economia");

    inserisciPrenotazione(&aula, "0512100001", "21/05/2026", "11-13");

    /* Con un solo posto disponibile, il secondo studente va in attesa. */
    controlla("inserimento in lista d'attesa",
              inserisciPrenotazione(&aula, "0512100002", "21/05/2026", "11-13") == 1 &&
              aula.numeroAttesa == 1);

    controlla("prenotazione in attesa senza posto",
              aula.prenotazioni[1].inAttesa == VERO &&
              aula.prenotazioni[1].postoAssegnato == -1);
}

static void testCheckInUscitaStorico(void) {
    AulaStudio aula;

    inizializzaAula(&aula, 2);

    registraStudente(&aula, "0512100001", "Mario Rossi", "Informatica");
    inserisciPrenotazione(&aula, "0512100001", "22/05/2026", "09-11");

    /* Il check-in deve aggiornare presenti e storico. */
    controlla("check-in valido",
              checkInStudente(&aula, "0512100001", "22/05/2026", "09-11") == 1);

    controlla("studente presente dopo check-in",
              aula.numeroPresenti == 1);

    controlla("accesso inserito nello storico",
              aula.numeroStorico == 1);

    controlla("secondo check-in rifiutato",
              checkInStudente(&aula, "0512100001", "22/05/2026", "09-11") == 0);

    controlla("uscita valida",
              registraUscita(&aula, "0512100001") == 1);

    controlla("studente non presente dopo uscita",
              aula.numeroPresenti == 0);

    controlla("storico aggiornato dopo uscita",
              aula.storico[0].presente == FALSO);
}

static void testIngressoSenzaPrenotazione(void) {
    AulaStudio aula;

    inizializzaAula(&aula, 1);

    registraStudente(&aula, "0512100001", "Mario Rossi", "Informatica");
    registraStudente(&aula, "0512100002", "Luca Bianchi", "Economia");

    /* Il primo studente entra, il secondo va in attesa perché il posto è occupato. */
    controlla("ingresso senza prenotazione con posto libero",
              ingressoSenzaPrenotazione(&aula, "0512100001", "23/05/2026", "15-17") == 1);

    controlla("posti occupati dopo ingresso libero",
              postiDisponibili(&aula, "23/05/2026", "15-17") == 0);

    controlla("storico aggiornato per ingresso libero",
              aula.numeroStorico == 1);

    controlla("ingresso senza posto manda in attesa",
              ingressoSenzaPrenotazione(&aula, "0512100002", "23/05/2026", "15-17") == 2);
}

static void testAnnullamentoPrenotazione(void) {
    AulaStudio aula;

    inizializzaAula(&aula, 2);

    registraStudente(&aula, "0512100001", "Mario Rossi", "Informatica");
    inserisciPrenotazione(&aula, "0512100001", "24/05/2026", "09-11");

    /* Dopo l'annullamento la prenotazione non deve risultare attiva. */
    controlla("annullamento prenotazione esistente",
              annullaPrenotazione(&aula, "0512100001", "24/05/2026", "09-11") == 1);

    controlla("prenotazione annullata non cercabile",
              cercaPrenotazione(&aula, "0512100001", "24/05/2026", "09-11") == -1);
}

int main(void) {
    printf("Avvio test aula studio\n\n");

    testRegistrazioneStudenti();
    testPrenotazioniDisponibilita();
    testListaAttesa();
    testCheckInUscitaStorico();
    testIngressoSenzaPrenotazione();
    testAnnullamentoPrenotazione();

    printf("\nTest falliti: %d\n", testFalliti);

    if (testFalliti == 0)
        printf("Tutti i test sono stati superati.\n");

    return testFalliti;
}
