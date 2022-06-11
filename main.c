#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include "inc/fmod.h"

//Variables Globales
    FMOD_SYSTEM *pSystem;
    FMOD_SOUND *pMusique;
    FMOD_RESULT result;
    FMOD_CHANNELGROUP *pChannelGroup;
    FMOD_CHANNEL *pChannel;

    int enPause = 0;
    int enLecture = 1;
    int detectionTouche = 0;

//Définition de la structure piste
typedef struct Piste{
    char *pTitre;
    char *pArtiste;
    char *pChemin;
    struct Piste *pNext;
    struct Piste *pPrev;
}Piste;

//Initialisation
Piste *ppDL=NULL;


//Prototypes des fonctions utilisées dans le programme

void Menu_Principal();
void Mode_Utilisateur();
void Mode_Admin();

int Chargement_BDD();
void Recherche_Musique(char *search,Piste **pHead);
int Lecture_Musique(Piste **pHead);

int Ajouter_Element(Piste **pHead, char *titre, char *artiste, char *chemin);
void Afficher_Liste();
int Supprimer_Musique();
void Ajouter_Musique();


int main()
{
    int choix;
    Chargement_BDD();
    Menu_Principal();
    result = FMOD_System_Create(&pSystem);

    result =FMOD_System_Init(pSystem,1,FMOD_INIT_NORMAL,NULL);

    result = FMOD_System_CreateSound(pSystem, "Musiques\\Rick Astley - Never Gonna Give You Up.mp3", FMOD_DEFAULT , 0, &pMusique);

    result = FMOD_System_PlaySound(pSystem,pMusique,0,0,&pChannel);
    scanf("%d",choix);

}

void Menu_Principal(){
    int choix;

    system("cls");

    printf("---------- PolyMusic 2022 ----------\n\n");
    printf("1- Mode Utilisateur\n");
    printf("2- Mode Administrateur\n");
    printf("3- Fermer le programme\n");
    printf("Dans quel mode voulez vous entrer ?\n");
    scanf("%d", &choix);

    switch(choix){
    case 1:
        Mode_Utilisateur();
        break;

    case 2:
        Mode_Admin();
        break;

    case 3:
        system("cls");
        printf("Fermeture du programme...\n");
        exit(0);
    default:
        system("cls");
        printf("Votre choix n'est pas pris en compte\n\n");
        Menu_Principal();
    }
}

void Mode_Utilisateur(){

    int choix;
    char *recherche = NULL;

    while(choix != 4)
    {
        system("cls");

        printf("---------- Menu Utilisateur ----------\n\n");
        printf("1 - Recherche\n");
        printf("2 - Lecture d'une musique\n");
        printf("3 - Retour au menu principal\n");
        printf("Votre choix ?\n");
        scanf("%d", &choix);

        switch(choix){

        case 1:
            fflush(stdin);
            recherche = (char*)malloc(200 * sizeof(char));
            printf( "Que voulez vous ecouter ? : \n" );
            scanf( "%[^\n]", recherche );
            fgetc( stdin );
            if(recherche!=NULL){
                Recherche_Musique(recherche,&ppDL);
            }

            break;
        case 2:
            fflush(stdin);
            Lecture_Musique(&ppDL);
            break;
        case 3:
            Menu_Principal();
            break;
        default:
            system("cls");
            printf("Votre choix n'est pas pris en compte\n\n");
            Mode_Utilisateur();
        }
    }
}

void Mode_Admin(){
    int choix;

    system("cls");

        printf("---------- Menu admin ----------\n\n");
        printf("1 - Liste des musiques disponibles\n");
        printf("2 - Ajouter une musique\n");
        printf("3 - Supprimer une musique\n");
        printf("4 - Retour au menu principal\n");
        printf("Votre choix ?\n");
        scanf("%d", &choix);

        switch(choix){
        case 1:
            Afficher_Liste(&ppDL);
            Mode_Admin();
            break;

        case 2:
            Ajouter_Musique();
            break;
        case 3:
            Supprimer_Musique(&ppDL);
            Afficher_Liste();
            Mode_Admin();
            break;

        case 4:
            Menu_Principal();
            break;
        default:
            system("cls");
            printf("Votre choix n'est pas pris en compte\n\n");
            Mode_Admin();
        }
}


int Ajouter_Element(Piste **pHead, char *titre, char *artiste, char *chemin){

    Piste *pCur=NULL;
    if(pHead==NULL){
        return(-1);
    }
    else{
        Piste *pNew=(Piste*)malloc(sizeof(Piste));
        pNew->pTitre=titre;
        pNew->pArtiste=artiste;
        pNew->pChemin=chemin;
        pNew->pNext=NULL;
        pNew->pPrev=NULL;

        if(*pHead == NULL){
        *pHead=pNew;
        }
        else{
            pCur=*pHead;
            while( pCur->pNext != NULL){
                pCur=pCur->pNext;
            }
            pNew->pPrev=pCur;
            pCur->pNext=pNew;
        }
        return(0);
    }
}

int Chargement_BDD(){
    char buffer[255];
    FILE *inputFile;
    inputFile=fopen("bddmp3.txt","r");
    const char * delim =";";
    char* artisteTmp;
    char* titreTmp;
    char* cheminTmp;


    while(!feof(inputFile)){
            char * strtoken = strtok(fgets(buffer,255,inputFile),delim);
            int i = 0;
            artisteTmp= (char*)malloc(200 * sizeof(char));
            titreTmp= (char*)malloc(200 * sizeof(char));
            cheminTmp= (char*)malloc(200 * sizeof(char));

            while(strtoken!=NULL){
                strtoken = strtok(NULL,delim);
                    if(i==0){
                        strcpy(artisteTmp, strtoken);
                    }
                    else if(i == 1){
                        strcpy(titreTmp, strtoken);
                    }
                    else if(i == 2){
                        strcpy(cheminTmp, strtoken);
                    }
                    i++;
            }
            Ajouter_Element(&ppDL,titreTmp,artisteTmp,cheminTmp);
    }
    return 0;
}

void Afficher_Liste(Piste **pHead){

    Piste *pCur = *pHead;
    char retour;
    int cptr=1;

    system("cls");

    while(pCur != NULL){
        printf("ID : %d / Titre : %s / Artiste : %s  Chemin : %s \n", cptr,pCur->pTitre, pCur->pArtiste, pCur->pChemin);
        pCur = pCur->pNext;
        cptr++;
    }
    fflush(stdin);
    printf("\n");
    printf("appuyez sur une lettre pour continuer\t");
    scanf("%c",&retour);
}

void Recherche_Musique(char *search,Piste **pHead){

    const char * comparaison;
    const char * comparaison2;
    int cptr = 0;
    char retour;
    Piste *pCur = *pHead;

    system("cls");

    //met en minuscule la recherche
    for(int i = 0; *(search+i); i++){
    *(search+i) = tolower(*(search+i));
    }

    while(pCur != NULL){
        for (int i = 0; pCur->pTitre[i];i++){
            pCur->pTitre[i]=tolower(pCur->pTitre[i]);
        }
        for (int i = 0; pCur->pArtiste[i];i++){
            pCur->pArtiste[i]=tolower(pCur->pArtiste[i]);
        }
        comparaison = strstr(pCur->pArtiste,search);
        comparaison2 = strstr(pCur->pTitre,search);
        if(comparaison != NULL || comparaison2 != NULL ){
            printf("Artiste : %s / Titre : %s\n",pCur->pArtiste,pCur->pTitre);
            cptr++;
        }
        pCur = pCur->pNext;
    }

    printf("il y a eu %d resultat(s)\n",cptr);

    if(cptr==0){
        printf("aucune correspondance dans la bdd");
    }

    fflush(stdin);
    printf("\n");
    printf("appuyez sur r pour retourner au menu precedent\t");
    scanf("%c",&retour);
    if(retour == 'r'){
        system("cls");
        Mode_Utilisateur();
    }
    else{
        printf("appuyez sur r");
    }
}

int Supprimer_Musique(Piste **pHead){

    int choix;
    int cptr=1;

    Piste *pCur = *pHead;

    system("cls");
    printf("Vous etes sur le point de supprimer une musique\n");
    printf("Liste des musiques\n");
    Afficher_Liste(&ppDL);
    printf("Quelle musique voulez vous supprimer ?\t");
    scanf("%d",&choix);

    while(pCur!=NULL){
        if(cptr==choix){
            if(pCur == *pHead){
            *pHead=pCur->pNext;
            }
            else{
            (pCur->pPrev)->pNext = pCur->pNext;
                if(pCur->pNext != NULL){
                (pCur->pNext)->pPrev = pCur->pPrev;
                }
            }
        }
        pCur=pCur->pNext;
        cptr++;
    }
    fflush(stdin);
    Afficher_Liste(&ppDL);
    return 1;
}

void Ajouter_Musique(){
    char titre [50];
    char artiste [50];
    char chemin_utilisateur [50];
    char chemin [50]="Musiques\\\\";

    fflush(stdin);
    printf("Vous etes sur le point d'ajouter une musique\n");
    printf("Attention bien indiquer l'extension .mp3 de la musique\n");
    printf("Titre de la musique : \n");
    scanf("%19[^\n]", &titre);
    fflush(stdin);
    printf("Artiste de la musique : \n");
    scanf("%19[^\n]", &artiste);
    fflush(stdin);
    printf("Chemin de la musique : \n");
    scanf("%19[^\n]", &chemin_utilisateur);

    strcat(chemin,chemin_utilisateur);

    Ajouter_Element(&ppDL,titre,artiste,chemin_utilisateur);
    Mode_Admin();
}

int Lecture_Musique(Piste **pHead){

    int choix;
    int cptr = 1;
    char * chemin_musique;
    int debug;
    int taille;
    int toto;

    chemin_musique= (char*)malloc(200 * sizeof(char));

    Afficher_Liste(&ppDL);
    Piste *pCur = *pHead;

    result = FMOD_System_Create(&pSystem);
    printf("%d\n",result);
    result =FMOD_System_Init(pSystem,1,FMOD_INIT_NORMAL,NULL);
    printf("%d\n",result);

    printf("Quelle musique voulez vous jouer ?\n");
    scanf("%d",&choix);

    while(choix!=cptr){
        pCur = pCur->pNext;
        cptr++;
    }
    chemin_musique = pCur->pChemin;

    taille = strlen(chemin_musique);
    if(chemin_musique[taille-1]==10){
        chemin_musique[taille-1]='\0';
    }

    printf("\n Musique en cours de lecture : %s\n",chemin_musique);
    result = FMOD_System_CreateSound(pSystem,chemin_musique,FMOD_DEFAULT,0,&pMusique);
    printf("%d\n",result);
    result = FMOD_System_PlaySound(pSystem,pMusique,0,enPause,&pChannel);
    printf("%d\n",result);
    printf("\nBouton espace pour jouer/pause\nBouton r pour relancer au debut \nBouton echap pour revenir au menu principal");
    FMOD_Channel_SetPaused(pChannel,enPause);
    fflush(stdin);
    while(enLecture){
        detectionTouche = getch();

        switch(detectionTouche){
        case 32:
            enPause != enPause;
            FMOD_Channel_SetPaused(pChannel,1);
            break;

        case 114:
            enPause=0;
            FMOD_System_PlaySound(pSystem,pMusique,0,enPause,&pChannel);
            break;

        case 82:
            enPause=0;
            FMOD_System_PlaySound(pSystem,pMusique,0,enPause,&pChannel);
            break;

        case 27:
            FMOD_Channel_Stop(pChannel);
            FMOD_Sound_Release(pMusique);
            FMOD_System_Close(pSystem);
            FMOD_System_Release(pSystem);
            enLecture=0;
            enPause=0;
            system("cls");
            Mode_Utilisateur();
            break;
               }
    }
    return -1;

}
