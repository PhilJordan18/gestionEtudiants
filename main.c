#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// Déficition de la structure un étudiant
typedef struct {
   char nom[50];
   char prenom[50];
   float resultatExamenIntra;
   float resultatProjet;
   float resultatExamenFinal;
} Etudiant;

// Prototypes des fonctions
void afficherMenu();
int validerChoix(int choix);
void gererChoixMenu(int choix, Etudiant **etudiants, int *taille);
int validerNotes(float *note);
void saisirEtudiant(Etudiant **etudiants, int *taille);
void afficherEtudiantEnEchec(Etudiant *etudiants, int taille);
void afficherMeilleurEtudiant(Etudiant *etudiants, int taille);
void afficherEtudiantAleatoire(Etudiant *etudiants, int taille);
float calculermoyenne(Etudiant etudiant);
void afficherMoyenneGroupe(Etudiant *etudiants, int taille);
void libererMemoire(Etudiant **etudiants);

int main() {
    Etudiant *etudiants = NULL;
    int taille = 0;
    int choix;
    /*
     * J'ai choisi intentionnellement de tous gérer dans le menu pour facilité
     * la gestion des données entrées sur les élèves.
     */
    do {
        afficherMenu();
        choix = validerChoix(choix);
        gererChoixMenu(choix, &etudiants, &taille);
    } while (choix != 6);
    libererMemoire(&etudiants);
    return 0;
}

void afficherMenu(){
    printf("\033[1;34m");
    printf("                \n"
           " _ __ ___   ___ _ __  _   _ \n"
           "| '_ ` _ \\ / _ \\ '_ \\| | | |\n"
           "| | | | | |  __/ | | | |_| |\n"
           "|_| |_| |_|\\___|_| |_|\\__,_|\n"
           "                            \n"
           "                            ");
    printf("\n\n");
    printf("┌──────────────────────────────────────────────────────────────────────────────┐\n"
           "│                                                                                      │\n"
           "│  1. Saisir un nouvel étudiant                                                      │\n"
           "│  2. Afficher la liste des étudiants en échec                                      │\n"
           "│  3. Afficher le meilleur étudiant                                                  │\n"
           "│  4. Afficher les détails d'un étudiant au hasard                                 │\n"
           "│  5. Afficher la moyenne du groupe                                                  │\n"
           "│  6. Quitter l'application                                                          │\n"
           "│                                                                                      │\n"
           "└──────────────────────────────────────────────────────────────────────────────┘");
    printf("\n\n");
    printf("\033[0m");
}

int validerChoix(int choix){
    do{
        printf("Faites votre choix (1-6) : ");
        if (scanf("%d", &choix) != 1){
            while (getchar() != '\n'){
                choix = 0;
            }
        }
    }while(choix < 1 || choix > 6);
    return choix;
}

void gererChoixMenu(int choix, Etudiant **etudiants, int *taille){
    switch (choix) {
        case 1:
            saisirEtudiant(etudiants, taille);
            break;
        case 2:
            afficherEtudiantEnEchec(*etudiants, *taille);
            break;
        case 3:
            afficherMeilleurEtudiant(*etudiants, *taille);
            break;
        case 4:
            afficherEtudiantAleatoire(*etudiants, *taille);
            break;
        case 5:
            afficherMoyenneGroupe(*etudiants, *taille);
            break;
        case 6:
            printf("Au revoir !");
            break;
    }
}

int validerNotes(float *note){
    int resultat;
    char lecteur[100];
    float chiffreTemporaire = 0;
    do {
        resultat = scanf("%99s", lecteur);
        if (resultat == 1 && sscanf(lecteur, "%f", &chiffreTemporaire) == 1){
            if (chiffreTemporaire >= 0 && chiffreTemporaire <= 100){
                *note = chiffreTemporaire;
                return 1;
            }
        }
        while (getchar() != '\n');
        printf("Notes invalides. Veillez entrer une note entre 0 et 100 : ");
    } while (1);
}

void saisirEtudiant(Etudiant **etudiants, int *taille){
    // Allouer ou réallouer de la mémoire pour un nouvel étudiant
    if (*taille == 0){
        *etudiants = (Etudiant *)malloc(sizeof(Etudiant));
    } else{
        *etudiants = (Etudiant *) realloc(*etudiants, (*taille + 1) * sizeof(Etudiant));
    }
    if (*etudiants == NULL){
        printf("Erreur d'allocaton de la mémoire.\n");
        exit(1);
    }

    printf("Nom de l'étudiant : ");
    scanf("%s", (*etudiants)[*taille].nom);
    fflush(stdin);

    printf("Prénom de l'étudiant : ");
    scanf("%s", (*etudiants)[*taille].prenom);
    fflush(stdin);

    printf("Note de l'examen intra : ");
    validerNotes(&(*etudiants)[*taille].resultatExamenIntra);

    printf("Note du projet : ");
    validerNotes(&(*etudiants)[*taille].resultatProjet);

    printf("Note de l'examen final : ");
    validerNotes(&(*etudiants)[*taille].resultatExamenFinal);

    (*taille)++;
}

void afficherEtudiantEnEchec(Etudiant *etudiants, int taille){
    float moyenne;
    printf("\n         Étudiants en échec : \n\n");
    for (int i = 0; i < taille; ++i) {
        moyenne = calculermoyenne(etudiants[i]);
        if (moyenne < 60.0){
            printf("\033[31m");
            printf("%s %s, Moyenne : %.2f%%\n", etudiants[i].nom, etudiants[i].prenom, moyenne);
            printf("\033[0m\n");
        } else{
            printf("\033[1;32m\n");
            printf("Aucun n'élève du groupe n'est en échec!\n");
            printf("\033[0m\n");
        }
    }
}

void afficherMeilleurEtudiant(Etudiant *etudiants, int taille){
    int indiceMeilleur = 0;
    float meilleureMoyenne = 0;
    float moyenne;
    for (int i = 0; i < taille; i++){
        moyenne = calculermoyenne(etudiants[i]);
        if (moyenne > meilleureMoyenne){
            meilleureMoyenne = moyenne;
            indiceMeilleur = i;
        }
    }
    printf("\033[1;32m");
    printf("\n Le meilleur étudiant : %s %s avec une moyenne de %.2f%%\n",
           etudiants[indiceMeilleur].nom, etudiants[indiceMeilleur].prenom,
           meilleureMoyenne );
    printf("\033[0m\n");
}


void afficherEtudiantAleatoire(Etudiant *etudiants, int taille){
    srand(time(NULL));
    int indiceAleatoire = rand() % taille;
    Etudiant etudiant = etudiants[indiceAleatoire];
    float moyenne = calculermoyenne(etudiant);
    if (moyenne < 60.0){
        printf("\033[31m");
        printf("\n Étudiant au hazard : %s %s, Moyenne : %.2f%%\n", etudiant.nom, etudiant.prenom, moyenne);
        printf("\033[0m\n");
    }else if (moyenne < 80 && moyenne >= 60){
        printf("\033[1;33m");
        printf("\n Étudiant au hazard : %s %s, Moyenne : %.2f%%\n", etudiant.nom, etudiant.prenom, moyenne);
        printf("\033[0m\n");
    }
    else{
        printf("\033[1;32m");
        printf("\n Étudiant au hazard : %s %s, Moyenne : %.2f%%\n", etudiant.nom, etudiant.prenom, moyenne);
        printf("\033[0m\n");
    }

}

float calculermoyenne(Etudiant etudiant){
    return (float )(etudiant.resultatExamenIntra * 0.25 +
    etudiant.resultatProjet * 0.35 + etudiant.resultatExamenFinal * 0.40);
}

void afficherMoyenneGroupe(Etudiant *etudiants, int taille){
    float sommeMoyenne = 0;
    float moyenneGroupe;
    for (int i = 0; i < taille; ++i) {
        sommeMoyenne += calculermoyenne(etudiants[i]);
    }
    moyenneGroupe = sommeMoyenne / (float)taille;
    printf("\033[35m");
    printf("La moyenne générale du groupe : %.2f%%\n", moyenneGroupe);
    printf("\033[0m\n");
}

void libererMemoire(Etudiant **etudiants) {
    free(*etudiants);
    *etudiants = NULL;
}