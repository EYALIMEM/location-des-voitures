#include <stdio.h>
#include <stdlib.h>
#include<string.h>
typedef struct date
{
    int jour;
    int mois;
    int annee;
}date;
typedef struct voiture
{
    int iv;
    char marque[20];
    char Immat[20];
     char modele[20];
    char couleur[20];
    char disponibilite[10];
    int nbplaces;
    float prixjour;
} voiture;
typedef struct client
{
    int codeclient;
    char nom[20];
    char prenom[20];
    int num_telephone;
} client ;
typedef struct contrat{
    int numc;
    int idv;
    int idc;
    struct date date_location;
    struct date date_restitution;
    float prixjour;
    float cout;
    }contrat;

typedef struct liste {
    struct voiture val;
    struct liste*suiv;
}liste;
typedef struct element {
    struct contrat val;
    struct element*suiv;
}element;
typedef struct head {
    struct Node* tete;
}head;
void afficher_menu()
{
    printf("{BIENVENUE CHEZ E-CARS} \n\n");
    printf("1-ajout d'une voiture\n");
    printf("2-Location d'une voiture\n");
    printf("3-Suppression d'une voiture\n");
    printf("4-Modification de l'état et la description d'une voiture\n");
    printf("5-description d'une voiture\n");
   printf("6-Les voitures de notre agence:\n");

    printf("7-Historique de location\n");
    printf("8-Reclamation\n");
}
int differenceEnJours(date date1, date date2) {
    const int joursParMois[12] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int joursDepuisOrigine(date date) {
        int jours = 0;
        int annee;
        for (annee = 0; annee < date.annee; annee++) {
            jours += 365;
            if ((annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0)) {
                jours++;
            }
        }

        for (int mois = 1; mois < date.mois; mois++) {
            jours += joursParMois[mois];
            if (mois == 2 && ((date.annee % 4 == 0 && date.annee % 100 != 0) || (date.annee % 400 == 0))) {
                jours++;
            }
        }

        jours += date.jour;

        return jours;
    }
    int jours1 = joursDepuisOrigine(date1);
    int jours2 = joursDepuisOrigine(date2);
    return jours2 - jours1;
}


void ajout_voiture(){
FILE * voit = fopen("fvoitures.bin","ab");
if (voit == NULL){
   printf("\t\t\tERREUR: Impossible d'ouvrir le fichier!\n");
  } else{
      struct voiture vaj;
      printf("identifiant:");
      scanf("%d",&vaj.iv);
      printf ("\t\t\tmarque  : ");
      scanf ("%s", vaj.marque);
      printf ("\t\t\tImmatriculation: ");
      scanf ("%s", vaj.Immat);

       printf ("\t\t\tmodele : ");
       scanf ("%s", vaj.modele);
       printf("\t\t\tcouleur : ");
       scanf("%s",vaj.couleur);
       printf("\t\t\tnombre des places :");
       scanf("%d",&vaj.nbplaces);
       printf("\t\t\tDisponibilite:");
       scanf("%s",vaj.disponibilite);
       printf("\t\t\tprix d'un jour de location:\n");
       scanf("%f", &vaj.prixjour);
        if (fwrite(&vaj,sizeof(voiture),1,voit) == 1){
        fclose (voit);
        printf("voiture ajoutée avec succes!\n");
        }
        else{printf("erreur\n");
           }
  }
}
void louer_voiture() {
    FILE *fvoit = fopen("fvoitures.bin", "rb");
    FILE *cont = fopen("fcontrats.bin", "ab");

    if (fvoit == NULL || cont == NULL) {
        printf("\t\t\tERREUR: Impossible d'ouvrir le fichier!\n");
        return;
    }

    struct voiture vt;
    struct contrat CON;
    int id;

    int tr = 0;  // Déclaration de la variable tr ici

    printf("\t\t\t Saisir l'identifiant du client:");
    scanf("%d", &id);

    printf("\t\t\tEntrez les informations de la voiture voulue: \n");
    printf("\t\t\tmarque  : ");
    scanf("%s", vt.marque);
    printf("\t\t\tmodele : ");
    scanf("%s", vt.modele);
    printf("\t\t\tcouleur : ");
    scanf("%s", vt.couleur);
    printf("\t\t\tnombre des places :");
    scanf("%d", &vt.nbplaces);

    struct voiture V;
    FILE *f = fopen("nn.bin", "wb");  // Ouvrir un nouveau fichier pour la liste mise à jour

    while (fread(&V, sizeof(voiture), 1, fvoit) != 0) {
        if (strcmp(V.marque, vt.marque) == 0 && strcmp(V.modele, vt.modele) == 0 &&
            strcmp(V.couleur, vt.couleur) == 0 && V.nbplaces == vt.nbplaces) {
            tr = 1;

            if (strcmp(V.disponibilite, "Oui") == 0 || strcmp(V.disponibilite, "oui") == 0 ||
                strcmp(V.disponibilite, "OUI") == 0) {
                tr = 2;
                 CON.prixjour = V.prixjour;
                printf("\t\t\tCette voiture est disponible!\n");
                printf("\t\t\tle numero de contrat : ");
                scanf("%d", &CON.numc);
                CON.idv = V.iv;
                CON.idc = id;
                printf("\t\t\tDate de la location(jour-mois-annee):");
                scanf("%02d-%02d-%04d", &CON.date_location.jour, &CON.date_location.mois,
                      &CON.date_location.annee);
                printf("\t\t\tDate de restitution (jour-mois-annee):");
                scanf("%02d-%02d-%04d", &CON.date_restitution.jour, &CON.date_restitution.mois,
                      &CON.date_restitution.annee);

                CON.cout = differenceEnJours(CON.date_location, CON.date_restitution) * V.prixjour;
                fwrite(&CON, sizeof(contrat), 1, cont);

                strcpy(V.disponibilite, "non");
            }
        }

        fwrite(&V, sizeof(voiture), 1, f);  // Écrire chaque voiture dans le fichier mis à jour
    }

    fclose(fvoit);
    fclose(cont);
    fclose(f);

    remove("fvoitures.bin");
    rename("nn.bin", "fvoitures.bin");

    if (tr == 0) {
        printf("\t\t\tCaracteristiques introuvables! \n");
    } else if (tr == 1) {
        printf("\t\t\tCette voiture est deja en location!\n");
    } else {
        printf("La voiture a été louée avec succès!\n");
    }
}



void inserer_voit(liste** tete, struct voiture nv) {
    liste* nouvelElement = (liste*)malloc(sizeof(liste));
    if (nouvelElement == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    nouvelElement->val = nv;
    nouvelElement->suiv = *tete;
    *tete = nouvelElement;
}
void inserer_contrat(element** tete, struct contrat nv) {
    element* nouvelElement = (element*)malloc(sizeof(element));
    if (nouvelElement == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    nouvelElement->val = nv;
    nouvelElement->suiv = *tete;
    *tete = nouvelElement;
}

void libererListe(liste** tete) {
    liste* p = *tete;
    liste * suivant;

    while (p != NULL) {
        suivant = p->suiv;
        free(p);
        p = suivant;
    }

    *tete = NULL; //
}
void libererelement(element** tete) {
    struct element* p = *tete;
    struct element* suivant;

    while (p != NULL) {
        suivant = p->suiv;
        free(p);
        p = suivant;
    }

    *tete = NULL;
}
void suppression(int id) {
    FILE* fvoit = fopen("fvoitures.bin", "rb");
    if (fvoit == NULL) {
        fprintf(stderr, "Error opening the file voitures.bin\n");
        return;
    }
    FILE* nv = fopen("nv.bin", "wb");
    if (nv == NULL) {
        fprintf(stderr, "Error opening the file nv.bin\n");
        return;
    }
    struct voiture v;
    int trouve = 0;
    while (fread(&v, sizeof(voiture), 1, fvoit) == 1) {
        if (v.iv != id) {
            fwrite(&v, sizeof(voiture), 1,nv);

        }else{
            trouve = 1;}
    }
    fclose(fvoit);
    fclose (nv);
    if (trouve){
    remove("fvoitures.bin");
    rename("nv.bin","fvoitures.bin");

    printf("La voiture a été supprimée avec succès.\n");
}else {
    remove("nv.bin");
        printf("La voiture avec l'ID %d n'a pas été trouvée.\n", id);
    }
}
void description_voiture(int id)
{
    FILE*fvoit = fopen("fvoitures.bin","rb");
    struct liste*listev = NULL;
    struct voiture v;
    while(fread(&v, sizeof(voiture), 1, fvoit)==1){
            inserer_voit(&listev,v);
    }
    fclose(fvoit);
    struct liste*p = listev;
    int trouve = 0;
    while(p!=NULL){
        if(p->val.iv == id){
                trouve = 1;
            printf("la description de la voiture d'identifiant:%d\n ",id);
            printf("marque:%s\n",p->val.marque);
             printf("immatriculation:%s\n",p->val.Immat);
            printf("modele:%s\n",p->val.modele);
            printf("couleur:%s\n",p->val.couleur);

            printf("nombre des places:%d\n",p->val.nbplaces);
            printf("disponibilite:%s\n",p->val.disponibilite);
            printf("prix location un jour:%f\n",p->val.prixjour);
          }
          break;
         p = p->suiv;

    }
    libererListe(&listev);


    if(trouve == 0){
        printf("voiture introuvable");
    }
}
void modifier_description_voiture(int id) {
    FILE* fvoit = fopen("fvoitures.bin", "rb");
    if (fvoit == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier voitures.bin\n");
        exit(EXIT_FAILURE);
        return;
    }

    struct voiture v;
    struct liste* listev = NULL;
    while (fread(&v, sizeof(struct voiture), 1, fvoit) == 1) {
        inserer_voit(&listev, v);
    }
        fclose(fvoit);

    struct liste* p = listev;
    int trouve = 0;
    while (p != NULL) {
        if (p->val.iv == id) {
            trouve = 1;
            printf("Donner le nouveau modele:");
            scanf("%s", p->val.modele);
            printf("Donner la nouvelle couleur:");
            scanf("%s", p->val.couleur);
            printf("Donner la nouvelle immatriculation:");
            scanf("%s", p->val.Immat);
            printf("Donner le nouveau nombre de places:");
            scanf("%d", &p->val.nbplaces);
            printf("Donner le nouvel etat de disponibilite:");
            scanf("%s", p->val.disponibilite);
            printf("Donner le nouveau prix d'un jour de location:");
            scanf("%f", &p->val.prixjour);
            break;
        }
        p = p->suiv;
    }

    if (!trouve) {
        printf("Voiture introuvable!\n");
         libererListe(&listev);
        return;
    }
    FILE* nf = fopen("fvoitures.bin", "wb");
    if (nf == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier voitures.bin\n");
        exit(EXIT_FAILURE);
    }
    struct liste* i = listev;
    while (i != NULL) {
        fwrite(&(i->val), sizeof(struct voiture), 1, nf);
        i = i->suiv;
    }
    fclose(nf);
    libererListe(&listev);
    remove("fvoitures.bin");
    rename("nf.bin","fvoitures.bin");
    printf("Modification effectuée avec succès!\n");

}

   void affiche_historique() {
    FILE *cont = fopen("fcontrats.bin", "rb");

    if (cont == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier.\n");
        return;
    }

    struct contrat C;
    while (fread(&C, sizeof(contrat), 1, cont) == 1) {
        printf("\t\t\tnumero de contrat        : %d \n", C.numc);
        printf("\t\t\tclient locataire        : %d \n", C.idc);
        printf("\t\t\tidentifiant  de la  voiture    : %d \n", C.idv);
        printf("\t\t\tDate de location : %02d-%02d-%04d\n", C.date_location.jour, C.date_location.mois, C.date_location.annee);
        printf("\t\t\tDate de restitution :%02d-%02d-%04d\n", C.date_restitution.jour, C.date_restitution.mois, C.date_restitution.annee);
        printf("\t\t\tcout de la période de location  : %f\n", C.cout);
        printf("\t\t\t--------------------------------\n");
    }
   fclose(cont);
}


    void voit_dispo(){

    FILE*fvoit = fopen("fvoitures.bin","rb");
    struct voiture v;
    while(fread(&v, sizeof(voiture), 1, fvoit)==1){
     if(strcmp(v.disponibilite, "Oui") == 0 || strcmp(v.disponibilite, "oui") == 0 || strcmp(v.disponibilite, "OUI") == 0){
        printf( "%d\t%s\t%s\t%s\t%s\t%s\t%d\t%f\n",v.iv,v.marque,v.modele,v.Immat,v.couleur,v.disponibilite,v.nbplaces,v.prixjour);
     }
    }
    }
    void afficher_voit(){
         FILE*fvoit = fopen("fvoitures.bin","rb");
    struct voiture v;
    while(fread(&v, sizeof(voiture), 1, fvoit)==1){
        printf( "%d\t%s\t%s\t%s\t%s\t%s\t%d\t%f\n",v.iv,v.marque,v.modele,v.Immat,v.couleur,v.disponibilite,v.nbplaces,v.prixjour);
     }
    }
    void hist_jour_par_jour() {
    FILE *cont = fopen("fcontrats.bin", "rb");
    if (cont == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier contrats.bin\n");
        exit(EXIT_FAILURE);
    }
    int jour,mois,annee;
    struct date hisj;
    printf("Date (jour-mois-annee) : ");
    scanf("%d-%d-%d", &hisj.jour, &hisj.mois, &hisj.annee);

    struct contrat c;
    int trouve = 0;

    while (fread(&c, sizeof(contrat), 1, cont) == 1) {
        if (c.date_location.jour == hisj.jour && c.date_location.mois == hisj.mois && c.date_location.annee == hisj.annee) {
            trouve = 1;
            printf("%d\t%d\t%d\t%02d-%02d-%04d\t%02d-%02d-%04d\t%f\n",
                   c.numc, c.idv, c.idc,
                   c.date_location.jour, c.date_location.mois, c.date_location.annee,
                   c.date_restitution.jour, c.date_restitution.mois, c.date_restitution.annee,
                   c.cout);
        }
    }

    fclose(cont);

    if (!trouve) {
        printf("Aucun contrat trouvé pour la date spécifiée.\n");
    }
}
void hist_mois_par_mois() {
    FILE *cont = fopen("fcontrats.bin", "rb");
    if (cont == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier contrats.bin\n");
        exit(EXIT_FAILURE);
    }
    int jour,mois,annee;
    struct date hisj;
    printf("Date (jour-mois-annee) : ");
    scanf("%d-%d-%d", &hisj.jour, &hisj.mois, &hisj.annee);

    struct contrat c;
    int trouve = 0;

    while (fread(&c, sizeof(contrat), 1, cont) == 1) {
        if (c.date_location.mois == hisj.mois && c.date_location.annee == hisj.annee) {
            trouve = 1;
            printf("%d\t%d\t%d\t%02d-%02d-%04d\t%02d-%02d-%04d\t%f\n",
                   c.numc, c.idv, c.idc,
                   c.date_location.jour, c.date_location.mois, c.date_location.annee,
                   c.date_restitution.jour, c.date_restitution.mois, c.date_restitution.annee,
                   c.cout);
        }
    }

    fclose(cont);

    if (!trouve) {
        printf("Aucun contrat trouvé pour la date spécifiée.\n");
    }
}



int main(){
    int choix;
    do {
        afficher_menu();

        printf("Saisissez votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
           {
            ajout_voiture();}
            break;

            case 2:{
                louer_voiture();}
               break;
            case 3:
                {
               int id;
                printf("donnez l'ID de la voiture a supprimer\n");
                scanf("%d",&id);
                 suppression(id);
                }
               break;
            case 4:
                {
                    int id;
                   printf("donnez l'ID de la voiture à modifier\n");
                scanf("%d",&id);
                modifier_description_voiture(id);
                }
                break;
            case 5:{
                int id;
                printf("donnez l'ID de la voiture que vous voulez sa description:\n");
                scanf("%d",&id);
                description_voiture(id);
            }
                break;
            case 6:{
                printf("1-Toutes les voitures:\n");
                printf("2-Les voitures disponibles:\n");
                int d;
                scanf("%d",&d);
                if (d == 1){
                    afficher_voit();
                }else{
                voit_dispo();

                }
            }
                break;
            case 7:
                {
                    int h;
                    printf("choisissez:\n");
                    printf("1-historique de tous les contrats:\n");
                    printf("2-historique par jour:\n ");
                    printf("3-historique par mois:\n");
                    scanf("%d",&h);
                    if (h == 1){
                    affiche_historique();
                    break;
                    }else if (h ==2) {
                        hist_jour_par_jour();
                        break;
                    }else{
                        hist_mois_par_mois();

                    }
                }
                break;

            case 8:
                {
                    int num;
                    printf("saisir le numero du contrat de reclamation :\n");
                    scanf("%d",&num);
                  struct contrat c;
                 FILE *fcont = fopen("fcontrats.bin", "rb");
               struct element *listec = NULL;
    while (fread(&c, sizeof(contrat), 1, fcont) == 1) {
        inserer_contrat(&listec, c);
    }

    fclose(fcont);

    FILE *nv = fopen("nv.bin", "ab");
    if (nv == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier nv.bin\n");
        exit(EXIT_FAILURE);
    }

    int tr = 0;
    struct element *p = listec;
    struct element *suivant;
    while (p != NULL) {
        suivant = p->suiv;

        if (p->val.numc == num) {
            struct date nvdate_restitution;
            printf("Entrez la nouvelle date de restitution (jour-mois-annee): ");
            scanf("%02d-%02d-%04d", &nvdate_restitution.jour, &nvdate_restitution.mois, &nvdate_restitution.annee);

            float frais_remboursement = differenceEnJours(nvdate_restitution,p->val.date_restitution) * p->val.prixjour;
            printf("Frais de remboursement : %f\n", frais_remboursement);
            tr = 1;
        } else {
            fwrite(&(p->val), sizeof(contrat), 1, nv);
        }

        p = suivant;
    }

    fclose(nv);

    if (tr == 1) {
        printf("Suppression avec succès!\n");
    } else {
        printf("Aucun contrat avec le numéro %d n'a été trouvé.\n", num);
    }

    remove("fcontrats.bin");
    rename("nv.bin", "fcontrats.bin");
    libererelement(&listec);
}

                break;

            default:{
                printf("Choix invalide\n");
            }
               break;

               }
               }while(choix > 0 && choix <=8);
               return 0;
    }














