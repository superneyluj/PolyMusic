#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include "inc/fmod.h"

//Variables Globales
    FMOD_SYSTEM *pSystem;
    FMOD_SOUND *pMusique;
    FMOD_CHANNELGROUP *pChannelGroup;
    FMOD_CHANNEL *pChannel;



//D�finition de la structure piste
typedef struct Piste{
    char *pTitre;
    char *pArtiste;
    char *pChemin;
    struct Piste *pNext;
    struct Piste *pPrev;
}Piste;

//Initialisation
Piste *ppDL=NULL;


//Prototypes des fonctions utilis�es dans le programme
void Menu_Login();
void Menu_Principal();
void Mode_Utilisateur();
void Mode_Admin();

int Chargement_BDD();
void Recherche_Musique(char *search,Piste **pHead);
int Lecture_Musique(Piste **pHead);

int Ajouter_Element(Piste **pHead, char *titre, char *artiste, char *chemin);
void Afficher_Liste(Piste **pHead);
int Supprimer_Musique(Piste **pHead);
void Ajouter_Musique();
void verificationSuppression(int choix,Piste **pHead);
void verificationExistence(Piste **pHead, char *titre, char *artiste);

int main()
{
    Menu_Login();
    Chargement_BDD();
    Menu_Principal();
    return 0;

}

void Menu_Principal(){

    int choix=0;
    int resultat=0;

    //efface les donn�es enregistrer pas le clavier
    fflush(stdin);


    while(choix!=4){

        printf("---------- PolyMusic 2022 ----------\n\n");
        printf("---------- Bienvenue ----------\n\n");
        printf("---------- By Julien Sicot ----------\n\n");
        printf("1- Mode Utilisateur\n");
        printf("2- Mode Administrateur\n");
        printf("3- Fermer le programme\n");
        printf("Dans quel mode voulez vous entrer ?\n");
        resultat = scanf("%d", &choix);

        //gestion de l'erreur si l'utilisateur entre autre chose qu'un int
        if(resultat!=1){
            system("cls");
            printf("Erreur de saisie, retour au menu principal...\n\n");
            Menu_Principal();
        }

        switch(choix){
            case 1:
            system("cls");
            Mode_Utilisateur();
            break;

            case 2:
            system("cls");
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
}

void Mode_Utilisateur(){

    int choix=0;
    int resultat=0;
    char *recherche = NULL;

    fflush(stdin);

    while(choix != 4)
    {

        printf("---------- Menu Utilisateur ----------\n\n");
        printf("1 - Recherche\n");
        printf("2 - Lecture d'une musique\n");
        printf("3 - Retour au menu principal\n");
        printf("Votre choix ?\n");
        resultat = scanf("%d", &choix);

        //gestion de l'erreur si l'utilisateur entre autre chose qu'un int
        if(resultat!=1){
            system("cls");
            printf("Erreur de saisie, retour au menu principal...\n\n");
            Menu_Principal();
        }

        switch(choix){

        case 1:
            fflush(stdin);
            system("cls");
            recherche = (char*)malloc(200 * sizeof(char));
            printf( "Que voulez vous ecouter ? : \n" );

            //scanf qui permet de g�rer les espaces
            scanf( "%[^\n]", recherche );
            if(recherche!=NULL){
                Recherche_Musique(recherche,&ppDL);
            }

            break;
        case 2:
            fflush(stdin);
            system("cls");
            Lecture_Musique(&ppDL);
            break;
        case 3:
            system("cls");
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
    fflush(stdin);
    int choix;
    int resultat=0;
    char retour;

        printf("---------- Menu admin ----------\n\n");
        printf("1 - Liste des musiques disponibles\n");
        printf("2 - Ajouter une musique\n");
        printf("3 - Supprimer une musique\n");
        printf("4 - Retour au menu principal\n");
        printf("Votre choix ?\n");
        resultat = scanf("%d", &choix);

        //gestion de l'erreur si l'utilisateur entre autre chose qu'un int
        if(resultat!=1){
            system("cls");
            printf("Erreur de saisie, retour au menu principal...\n\n");
            Menu_Principal();
        }

        switch(choix){
        case 1:
            Afficher_Liste(&ppDL);
            fflush(stdin);
            printf("\n");
            printf("appuyez sur une touche pour continuer\t");
            scanf("%c",&retour);
            system("cls");
            Mode_Admin();
            break;

        case 2:
            Ajouter_Musique();
            break;
        case 3:
            Supprimer_Musique(&ppDL);
            system("cls");
            Mode_Admin();
            break;

        case 4:
            system("cls");
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
    //si le pointeur pointe vers null �a signifie qu'il y a une erreur
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
    int cptr=1;

    system("cls");


    while(pCur != NULL){
        printf("ID : %d / Titre : %s / Artiste : %s  Chemin : %s \n", cptr,pCur->pTitre, pCur->pArtiste, pCur->pChemin);
        pCur = pCur->pNext;
        cptr++;
    }

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
    int resultat=0;
    int cptr=1;


    Piste *pCur = *pHead;

    system("cls");

    printf("Vous etes sur le point de supprimer une musique\n");
    printf("Liste des musiques\n");
    Afficher_Liste(&ppDL);
    printf("Quelle musique voulez vous supprimer ? renseignez l'ID\t");
    resultat = scanf("%d",&choix);

    verificationSuppression(choix,&ppDL);

    if(resultat!=1){
        system("cls");
        printf("Erreur de saisie, retour au menu principal...\n\n");
        Menu_Principal();
    }

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
    return 1;
}

void Ajouter_Musique(){
    char titre [50];
    char artiste [50];
    char chemin_utilisateur [50];
    char chemin [50]="Musiques\\\\";

    system("cls");
    fflush(stdin);

    printf("Vous etes sur le point d'ajouter une musique\n");
    printf("Attention bien indiquer l'extension .mp3 de la musique\n");
    printf("Appuyez sur r pour retourner au menu administrateur\n");
    printf("Titre de la musique : \n");
    scanf("%19[^\n]", titre);
    fflush(stdin);
    printf("Artiste de la musique : \n");
    scanf("%19[^\n]", artiste);
    fflush(stdin);
    printf("Chemin de la musique : \n");
    scanf("%19[^\n]", chemin_utilisateur);

    strcat(chemin,chemin_utilisateur);

    verificationExistence(&ppDL,titre,artiste);
    Ajouter_Element(&ppDL,titre,artiste,chemin_utilisateur);
    system("cls");
    Mode_Admin();
}

int Lecture_Musique(Piste **pHead){

    int choix;
    char * chemin_musique;
    int taille;
    int detectionTouche = 0;
    int isPaused=0;
    int cptrMusiques=1;
    int debug;

    chemin_musique= (char*)malloc(200 * sizeof(char));

    Afficher_Liste(&ppDL);
    Piste *pCur = *pHead;

    FMOD_System_Create(&pSystem);
    FMOD_System_Init(pSystem,1,FMOD_INIT_NORMAL,NULL);


    printf("Quelle musique voulez vous jouer ?\n");
    scanf("%d",&choix);

    while((choix!=cptrMusiques)){
                pCur=pCur->pNext;
                cptrMusiques++;
    }
    if(choix==cptrMusiques){
        chemin_musique=pCur->pChemin;
    }


    taille = strlen(chemin_musique);
    if(chemin_musique[taille-1]==10){
        chemin_musique[taille-1]='\0';
    }


    printf("\n Musique en cours de lecture : %s\n",chemin_musique);
    FMOD_System_CreateSound(pSystem,chemin_musique,FMOD_DEFAULT,0,&pMusique);
    FMOD_System_PlaySound(pSystem,pMusique,0,0,&pChannel);
    printf("\nBouton espace pour mettre en pause/reprendre la lecture\nBouton r pour relancer au debut \nBouton echap pour revenir au menu principal");
    FMOD_Channel_SetPaused(pChannel,0);
    fflush(stdin);
    while(1){
        detectionTouche = getch();

        switch(detectionTouche){
        case 32:
            isPaused=!isPaused;
            FMOD_Channel_SetPaused(pChannel,isPaused);
            break;

        case 114:
            FMOD_System_PlaySound(pSystem,pMusique,0,0,&pChannel);
            break;

        case 82:
            FMOD_System_PlaySound(pSystem,pMusique,0,0,&pChannel);
            break;

        case 27:
            FMOD_Channel_Stop(pChannel);
            FMOD_Sound_Release(pMusique);
            FMOD_System_Close(pSystem);
            FMOD_System_Release(pSystem);
            system("cls");
            Mode_Utilisateur();
            break;
               }
    }
    return -1;
}

void Menu_Login(){

    char pseudo[15];
    char mdp[15];
    int verif=0;

    while (verif != 1){
        printf(" Votre pseudo: ");
        int verifpseudo=0;
        while(verifpseudo!=1){
            scanf("%s",pseudo);
            if(strlen(pseudo)>1){
                verifpseudo=1;
            }
            else{
                printf("Veuillez saisir un pseudo: ");
            }
        }
        int verifmdp=0;
        printf(" Votre mot de passe: ");
        while(verifmdp!=1){
            int i;
            for (i = 0; i < 10;i++) {
                mdp[i] = _getch();
                _putch('*');
                if (mdp[i] == 13){
                    mdp[i] = '\0';
                    break;
                }
            }
            if(strlen(mdp)>1){
                verifmdp=1;
            }
            else{
                printf("Veuillez saisir un mdp: ");
            }
        }
        printf("\n");
        if((strcmp(mdp,"admin")==0) ){
            printf("mdp");
        }
        if( (strcmp(pseudo, "admin")==0 ) && (strcmp(mdp, "admin")==0) ){
            system("cls");
            verif = 1;
        }
        else{
            printf("Pseudo ou Mot de passe incorrect\n");
        }
    }
}

void verificationSuppression(int choix,Piste **pHead){

    Piste *pCur = *pHead;
    char retour;
    int cptrMusiques=0;

    fflush(stdin);

    while(pCur!=NULL){
        pCur = pCur->pNext;
        cptrMusiques++;
    }

    if(choix==0 || choix>cptrMusiques){
        printf("Cette musique n'existe pas\nAppuyez sur une touche pour revenir au menu administrateur\n");
        scanf("%c",&retour);
        system("cls");
    }
}

void verificationExistence(Piste **pHead, char *titre, char *artiste){

    Piste *pCur = *pHead;
    char retour;

    fflush(stdin);

    while(pCur!=NULL){
        if((strcmp(pCur->pArtiste, artiste)==0) & (strcmp(pCur->pTitre, titre)==0)){
            printf("Cette musique existe deja dans la bdd\nAppuyez sur une touche pour revenir au menu administracteur");
            scanf("%c",&retour);
            system("cls");
            Mode_Admin();
            }
        pCur = pCur->pNext;
    }
}
