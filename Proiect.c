#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id_participant;
    char nume[50];
    char joc[30];
    int punctaj;
    int taxa_participare;
    char statut[10];  // "activ" sau "inactiv"
} Participant;

void creareFisierParticipanti() {
    FILE *f = fopen("participanti.dat", "wb");
    if (!f) {
        printf("Eroare la crearea fișierului.\n");
        return;
    }

    int n;
    printf("Numar participanti initiali: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        Participant p;
        printf("ID: "); scanf("%d", &p.id_participant);
        printf("Nume: "); getchar(); fgets(p.nume, 50, stdin); strtok(p.nume, "\n");
        printf("Joc: "); fgets(p.joc, 30, stdin); strtok(p.joc, "\n");
        printf("Punctaj: "); scanf("%d", &p.punctaj);strtok(p.punctaj, "\n");
        printf("Taxa participare: "); scanf("%d", &p.taxa_participare);strtok(p.taxa_participare, "\n");
        strcpy(p.statut, "activ");

        fwrite(&p, sizeof(Participant), 1, f);
    }

    fclose(f);
    printf("Fisier creat cu succes.\n");
}

void adaugareParticipantNou() {
    FILE *f = fopen("participanti.dat", "ab");
    if (!f) {
        printf("Eroare la deschiderea fișierului.\n");
        return;
    }

    Participant p;
    printf("ID: "); scanf("%d", &p.id_participant);
    printf("Nume: "); getchar(); fgets(p.nume, 50, stdin); strtok(p.nume, "\n");
    printf("Joc: "); fgets(p.joc, 30, stdin); strtok(p.joc, "\n");
    printf("Punctaj: "); scanf("%d", &p.punctaj);
    printf("Taxa participare: "); scanf("%d", &p.taxa_participare);
    strcpy(p.statut, "activ");

    fwrite(&p, sizeof(Participant), 1, f);
    fclose(f);
    printf("Participant adaugat cu succes.\n");
}

void modificarePunctajParticipant() {
    FILE *f = fopen("participanti.dat", "rb+");
    if (!f) {
        printf("Eroare la deschiderea fișierului.\n");
        return;
    }

    int id, punctajNou;
    printf("ID participant: "); scanf("%d", &id);
    printf("Punctaj nou: "); scanf("%d", &punctajNou);

    Participant p;
    while (fread(&p, sizeof(Participant), 1, f)) {
        if (p.id_participant == id) {
            p.punctaj = punctajNou;
            fseek(f, -sizeof(Participant), SEEK_CUR);
            fwrite(&p, sizeof(Participant), 1, f);
            printf("Punctaj actualizat.\n");
            fclose(f);
            return;
        }
    }

    printf("Participantul nu a fost gasit.\n");
    fclose(f);
}

void actualizareStatutParticipanti() {
    FILE *f = fopen("participanti.dat", "rb+");
    if (!f) {
        printf("Eroare la deschiderea fișierului.\n");
        return;
    }

    Participant p;
    while (fread(&p, sizeof(Participant), 1, f)) {
        if (p.punctaj < 50 && strcmp(p.statut, "activ") == 0) {
            strcpy(p.statut, "inactiv");
            fseek(f, -sizeof(Participant), SEEK_CUR);
            fwrite(&p, sizeof(Participant), 1, f);
        }
    }

    fclose(f);
    printf("Statut actualizat pentru participantii cu punctaj < 50.\n");
}

void marcareParticipantInactiv() {
    FILE *f = fopen("participanti.dat", "rb+");
    if (!f) {
        printf("Eroare la deschiderea fișierului.\n");
        return;
    }

    int id;
    printf("ID participant de sters: "); scanf("%d", &id);

    Participant p;
    while (fread(&p, sizeof(Participant), 1, f)) {
        if (p.id_participant == id) {
            strcpy(p.statut, "inactiv");
            fseek(f, -sizeof(Participant), SEEK_CUR);
            fwrite(&p, sizeof(Participant), 1, f);
            printf("Participant sters .\n");
            fclose(f);
            return;
        }
    }

    printf("Participantul nu a fost gasit.\n");
    fclose(f);
}

void generareRaportCompletText() {
    FILE *f = fopen("participanti.dat", "rb");
    FILE *r = fopen("raport.txt", "w");
    if (!f || !r) {
        printf("Eroare la deschiderea fișierelor.\n");
        return;
    }

    Participant p;
    while (fread(&p, sizeof(Participant), 1, f)) {
        fprintf(r, "ID: %d, Nume: %s, Joc: %s, Punctaj: %d, Taxa: %d, Statut: %s\n",
                p.id_participant, p.nume, p.joc, p.punctaj, p.taxa_participare, p.statut);
    }

    fclose(f);
    fclose(r);
    printf("Raport complet salvat în raport.txt\n");
}

void generareRaportFiltratTaxaPunctaj() {
    FILE *f = fopen("participanti.dat", "rb");
    if (!f) {
        printf("Eroare la deschiderea fișierului.\n");
        return;
    }

    Participant p;
    printf("Participanti cu taxa < 100 si punctaj > 80:\n");
    while (fread(&p, sizeof(Participant), 1, f)) {
        if (p.taxa_participare < 100 && p.punctaj > 80) {
            printf("ID: %d, Nume: %s, Joc: %s, Punctaj: %d\n",
                   p.id_participant, p.nume, p.joc, p.punctaj);
        }
    }

    fclose(f);
}

void afisareParticipant() {
    FILE *f = fopen("participanti.dat", "rb");
    if (!f) {
        printf("Eroare la deschiderea fișierului.\n");
        return;
    }

    int id;
    printf("ID participant cautat: "); scanf("%d", &id);

    Participant p;
    while (fread(&p, sizeof(Participant), 1, f)) {
        if (p.id_participant == id) {
            printf("Nume: %s, Joc: %s, Punctaj: %d, Statut: %s\n",
                   p.nume, p.joc, p.punctaj, p.statut);
            fclose(f);
            return;
        }
    }

    printf("Participantul nu a fost gasit.\n");
    fclose(f);
}

// Funcție NOUĂ - raport integral stil "secvential"
void generareRaportTabelarComplet() {
    FILE *f = fopen("participanti.dat", "rb");
    FILE *ft = fopen("raport_secv.txt", "w");

    if (!f || !ft) {
        printf("Eroare la deschiderea fișierelor.\n");
        if (f) fclose(f);
        if (ft) fclose(ft);
        return;
    }

    Participant p;
    int nrc = 0;
    fprintf(ft, "Nr. crt.  ID     Nume                           Joc                         Punctaj  Taxa  Statut\n");
    fprintf(ft, "----------------------------------------------------------------------------------------------\n");

    while (fread(&p, sizeof(Participant), 1, f)) {
        nrc++;
        fprintf(ft, "%-9d %-6d %-30s %-28s %-8d %-6d %s\n",
                nrc, p.id_participant, p.nume, p.joc, p.punctaj, p.taxa_participare, p.statut);
    }

    fclose(f);
    fclose(ft);
    printf("Raport secvential complet salvat în 'raport_secv.txt'.\n");
}

// Funcție NOUĂ - raport parțial (doar participanti inactivi)
void generareRaportTabelarInactivi() {
    FILE *f = fopen("participanti.dat", "rb");
    FILE *ft = fopen("raport_partial_secv.txt", "w");

    if (!f || !ft) {
        printf("Eroare la deschiderea fișierelor.\n");
        if (f) fclose(f);
        if (ft) fclose(ft);
        return;
    }

    Participant p;
    int nrc = 0;
    fprintf(ft, "Nr. crt.  ID     Nume                           Joc                         Punctaj  Taxa  Statut\n");
    fprintf(ft, "----------------------------------------------------------------------------------------------\n");

    while (fread(&p, sizeof(Participant), 1, f)) {
        if (strcmp(p.statut, "inactiv") == 0) {
            nrc++;
            fprintf(ft, "%-9d %-6d %-30s %-28s %-8d %-6d %s\n",
                    nrc, p.id_participant, p.nume, p.joc, p.punctaj, p.taxa_participare, p.statut);
        }
    }

    fclose(f);
    fclose(ft);
    printf("Raport partial (inactivi) salvat în 'raport_partial_secv.txt'.\n");
}

void meniu() {
    int optiune;
    do {
        printf("\n--- Meniu competitie e-sports ---\n");
        printf("1. Creare fisier si participanti initiali\n");
        printf("2. Adauga participant\n");
        printf("3. Actualizeaza punctaj\n");
        printf("4. Actualizeaza statut (sub 50 puncte)\n");
        printf("5. Sterge participant\n");
        printf("6. Raport complet in fisier text\n");
        printf("7. Raport filtrat\n");
        printf("8. Cauta participant dupa ID\n");
        printf("9. Raport secvential complet\n");
        printf("10. Raport secvential partial (inactivi)\n");
        printf("0. Iesire\n");
        printf("Optiune: ");
        scanf("%d", &optiune);

        switch (optiune) {
            case 1: creareFisierParticipanti(); break;
            case 2: adaugareParticipantNou(); break;
            case 3: modificarePunctajParticipant(); break;
            case 4: actualizareStatutParticipanti(); break;
            case 5: marcareParticipantInactiv(); break;
            case 6: generareRaportCompletText(); break;
            case 7: generareRaportFiltratTaxaPunctaj(); break;
            case 8: afisareParticipant(); break;
            case 9: generareRaportTabelarComplet(); break;
            case 10: generareRaportTabelarInactivi(); break;
        }
    } while (optiune != 0);
}

int main() {
    meniu();
    return 0;
}
