#include <stdio.h>
#include "../include/aula_studio.h"

static void stampaMenu(void) {
    printf("\n--- Sistema Aula Studio ---\n");
    printf("1. Registra studente\n");
    printf("2. Inserisci prenotazione\n");
    printf("3. Verifica disponibilita'\n");
    printf("4. Check-in studente\n");
    printf("5. Ingresso senza prenotazione\n");
    printf("6. Registra uscita\n");
    printf("7. Annulla prenotazione\n");
    printf("8. Visualizza studenti registrati\n");
    printf("9. Visualizza prenotati\n");
    printf("10. Visualizza presenti\n");
    printf("11. Visualizza lista d'attesa\n");
    printf("12. Visualizza storico\n");
    printf("13. Report generale\n");
    printf("0. Esci\n");
    printf("Scelta: ");
}

int main(void) {
    AulaStudio aula;
    int scelta;
    int postiTotali;
    int esito;

    char matricola[MAX_MATRICOLA];
    char nome[MAX_NOME];
    char corso[MAX_CORSO];
    char data[MAX_DATA];
    char fascia[MAX_FASCIA];

    printf("Numero posti aula studio: ");
    scanf("%d", &postiTotali);

    inizializzaAula(&aula, postiTotali);

    do {
        stampaMenu();
        scanf("%d", &scelta);

        switch (scelta) {
            case 1:
                printf("Matricola: ");
                scanf("%19s", matricola);

                printf("Nome: ");
                scanf(" %49[^\n]", nome);

                printf("Corso di laurea: ");
                scanf(" %49[^\n]", corso);

                if (registraStudente(&aula, matricola, nome, corso))
                    printf("Studente registrato.\n");
                else
                    printf("Registrazione non riuscita.\n");
                break;

            case 2:
                printf("Matricola: ");
                scanf("%19s", matricola);

                printf("Data (gg/mm/aaaa): ");
                scanf("%10s", data);

                printf("Fascia oraria: ");
                scanf("%19s", fascia);

                if (inserisciPrenotazione(&aula, matricola, data, fascia))
                    printf("Prenotazione inserita.\n");
                else
                    printf("Prenotazione non riuscita.\n");
                break;

            case 3:
                printf("Data (gg/mm/aaaa): ");
                scanf("%10s", data);

                printf("Fascia oraria: ");
                scanf("%19s", fascia);

                printf("Posti disponibili: %d\n", postiDisponibili(&aula, data, fascia));
                break;

            case 4:
                printf("Matricola: ");
                scanf("%19s", matricola);

                printf("Data (gg/mm/aaaa): ");
                scanf("%10s", data);

                printf("Fascia oraria: ");
                scanf("%19s", fascia);

                if (checkInStudente(&aula, matricola, data, fascia))
                    printf("Check-in effettuato.\n");
                else
                    printf("Check-in non riuscito.\n");
                break;

            case 5:
                printf("Matricola: ");
                scanf("%19s", matricola);

                printf("Data (gg/mm/aaaa): ");
                scanf("%10s", data);

                printf("Fascia oraria: ");
                scanf("%19s", fascia);

                esito = ingressoSenzaPrenotazione(&aula, matricola, data, fascia);

                if (esito == 1)
                    printf("Ingresso registrato.\n");
                else if (esito == 2)
                    printf("Posti esauriti. Studente inserito in lista d'attesa.\n");
                else
                    printf("Ingresso non riuscito.\n");
                break;

            case 6:
                printf("Matricola: ");
                scanf("%19s", matricola);

                if (registraUscita(&aula, matricola))
                    printf("Uscita registrata.\n");
                else
                    printf("Studente non presente.\n");
                break;

            case 7:
                printf("Matricola: ");
                scanf("%19s", matricola);

                printf("Data (gg/mm/aaaa): ");
                scanf("%10s", data);

                printf("Fascia oraria: ");
                scanf("%19s", fascia);

                if (annullaPrenotazione(&aula, matricola, data, fascia))
                    printf("Prenotazione annullata.\n");
                else
                    printf("Prenotazione non annullata.\n");
                break;

            case 8:
                stampaStudenti(&aula);
                break;

            case 9:
                printf("Data (gg/mm/aaaa): ");
                scanf("%10s", data);

                printf("Fascia oraria: ");
                scanf("%19s", fascia);

                stampaPrenotati(&aula, data, fascia);
                break;

            case 10:
                stampaPresenti(&aula);
                break;

            case 11:
                stampaListaAttesa(&aula);
                break;

            case 12:
                stampaStorico(&aula);
                break;

            case 13:
                stampaReport(&aula);
                break;

            case 0:
                printf("Fine programma.\n");
                break;

            default:
                printf("Scelta non valida.\n");
        }
    } while (scelta != 0);

    return 0;
}
