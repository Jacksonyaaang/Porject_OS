#include <stdio.h>

#include "processus_gestion.h"
#include "tinyalloc.h"
#include "cpu.h"
#include "screen.h"
#include "string.h"

//les variables extern
extern int horloge;
extern int temps_ecoule;

//switch the context
void ctx_sw(uint32_t tab_sauve[5], uint32_t tab_restau[5]);
char *Name_Etat[] = {"elu", "activable", "endormi", "mourant"};


//les variables globales
Processus *tableau_processus[NBR_PROCESSUS];

Processus *actif = NULL; //the current actif process
Processus *tete_process = NULL;   //the head of the activable liste  
Processus *queue_process = NULL;
Processus *tete_endormi = NULL;

int NumberProcess = 0;


int32_t cree_processus(void (*code)(void), char *nom){
    
    if (NumberProcess+1>NBR_PROCESSUS) return -1;

    Processus *new_process = malloc(sizeof(*new_process));
    new_process->pid = NumberProcess;
    sprintf(new_process->nom_processus, "%s", nom);
    new_process->etat_proc = activable;
    new_process->tab_registre[1] = (uint32_t) &(new_process->pile_execution[TAILLE_PILE-1]); //%esp save the adrr of top of stack
    new_process->pile_execution[TAILLE_PILE-1] = (uint32_t)code;
    tableau_processus[NumberProcess] = new_process;

    insert_queue(new_process);

    NumberProcess++;
    return new_process->pid;
}


void struct_init(){
    Processus *process_depart = malloc(sizeof(*process_depart));
    process_depart->pid = 0;
    strcpy(process_depart->nom_processus, "idle");
    process_depart->etat_proc =  elu;
    tableau_processus[0] = process_depart;
    NumberProcess++;
    actif = process_depart; //initialization the current process

    tete_process = process_depart;
    queue_process = process_depart;
    queue_process->suiv = NULL;

    cree_processus(proc1, "proc1");
    cree_processus(proc2, "proc2");
    cree_processus(proc3, "proc3");
/*     cree_processus(proc4, "proc4");
    cree_processus(proc5, "proc5");
    cree_processus(proc6, "proc6");
     */

}

Processus *extraction_tete_activable(){
    Processus *pro_extract = tete_process;
    if (tete_process->suiv == NULL){
        tete_process = NULL;
        queue_process = NULL;
        return pro_extract;
    }
    
    pro_extract->etat_proc = elu;
    tete_process = tete_process->suiv;
    pro_extract->suiv =  NULL;
    return pro_extract;
}
Processus *extraction_tete_endormi(){
    Processus *a_retirer = tete_endormi;

    if (tete_endormi->suiv == NULL) {
        tete_endormi = NULL;
        return a_retirer;
    }

    tete_endormi = a_retirer->suiv;
    a_retirer->suiv = NULL;
    a_retirer->etat_proc = activable;
    return a_retirer;

}


void insert_queue(Processus *pro_insert){
    pro_insert->etat_proc = activable;
    pro_insert->suiv = NULL;
    if (tete_process == NULL)
    {
        tete_process = pro_insert;
        queue_process = pro_insert;
        return ;
    }
    queue_process->suiv = pro_insert;
    queue_process=  pro_insert;
}

void insert_endormi(Processus *pro_insert){
    pro_insert->etat_proc = endormi;
    if (tete_endormi == NULL){
        tete_endormi=  pro_insert;
    }else if (tete_endormi->temps_reveil > pro_insert->temps_reveil){  //quand le premier est < que pro_insert
        pro_insert->suiv = tete_endormi;
        tete_endormi = pro_insert;
    }else{
        Processus *current_pro = tete_endormi;
        while (current_pro->suiv != NULL && current_pro->suiv->temps_reveil < pro_insert->temps_reveil){
            current_pro = current_pro->suiv;
        }
        pro_insert->suiv = current_pro->suiv;
        current_pro->suiv =  pro_insert;
    }
    
}

void dors(uint32_t nbr_secs){
    if (nbr_secs == 0) return;

    actif->temps_reveil = horloge+nbr_secs;
    ordonnance();
}

void affiche_etats(void){
    //strcpy(buffer, Name_Etat[actif->etat_proc]);
    char *buffer0 = malloc(sizeof(*buffer0));
    sprintf(buffer0, "process [%s]'s status :[%s]",tableau_processus[0]->nom_processus, Name_Etat[actif->etat_proc]);
    char *buffer1 = malloc(sizeof(*buffer1));
    sprintf(buffer1, "process [%s]'s status :[%s]",tableau_processus[1]->nom_processus, Name_Etat[actif->etat_proc]);
        char *buffer2 = malloc(sizeof(*buffer2));
    sprintf(buffer2, "process [%s]'s status :[%s]",tableau_processus[2]->nom_processus, Name_Etat[actif->etat_proc]);
        char *buffer3 = malloc(sizeof(*buffer3));
    sprintf(buffer3, "process [%s]'s status :[%s]",tableau_processus[3]->nom_processus, Name_Etat[actif->etat_proc]);

    int length_etat0 = strlen(buffer0);
    int length_etat1 = strlen(buffer1);
    int length_etat2 = strlen(buffer2);
    int length_etat3 = strlen(buffer3);
    int place_ligne0 = 5;
    int place_ligne1 = 6;
    int place_ligne2 = 7;
    int place_ligne3 = 8;
    int place_col0 = 80-length_etat0;
    int place_col1 = 80-length_etat1;
    int place_col2 = 80-length_etat2;
    int place_col3 = 80-length_etat3;
    for (int i=0;i<length_etat0; ++i){
        ecrit_car(place_ligne0, place_col0+i, 0,3,14,*(buffer0+i));
    }
        for (int i=0;i<length_etat1; ++i){
        ecrit_car(place_ligne1, place_col1+i, 0,3,14,*(buffer1+i));
    }
        for (int i=0;i<length_etat2; ++i){
        ecrit_car(place_ligne2, place_col2+i, 0,3,14,*(buffer2+i));
    }
        for (int i=0;i<length_etat3; ++i){
        ecrit_car(place_ligne3, place_col3+i, 0,3,14,*(buffer3+i));
    }




    free(buffer0);
    free(buffer1);
    free(buffer2);
    free(buffer3);

}



int32_t mon_pid(void){
    return actif->pid;
}
char *mon_nom(void){
    return actif->nom_processus;
}

int nbr_secondes(){
    return actif->temps_reveil;
}


void ordonnance(void){
    /*
    int index_next;
    if (actif->pid != 7){  // pour l'ordonnanceur static 
        index_next = (actif->pid)+1;
    }else 
    {
        index_next = 0;
    }
    next_pro = tableau_processus[index_next];
    */
    Processus *save_actif = actif;

    while (tete_endormi != NULL && tete_endormi->temps_reveil < horloge){
        //inserer la queue de la liste activable 
        insert_queue(extraction_tete_endormi());
    }
    Processus *next_pro = extraction_tete_activable();

    if (actif->temps_reveil > horloge){
        insert_endormi(actif);
    }else{
        insert_queue(actif);
    }



    actif = next_pro;

    ctx_sw(save_actif->tab_registre, next_pro->tab_registre);
}


/*les tests________________________*/

//the first 2 tests:

/*
void idle(void){
    printf("[idle] je tente de passer la main a proc1...\n");
    ctx_sw(tableau_processus[0]->tab_registre, tableau_processus[1]->tab_registre);
}

void proc1(void){
    printf("[proc1] idle m'a donne la main\n");
    printf("[proc1] j'arrete le systeme\n");
    hlt();
} 
*/






/*Aller-retour entre les processus*/

/*
void idle(void){
    for (int i = 0; i < 3; i++){
        printf("[idle] je tente de passer la main a proc1...\n\n");
        ctx_sw(tableau_processus[0]->tab_registre, tableau_processus[1]->tab_registre);
    }
    printf("[idle] je bloque le systeme\n");
    hlt();
}

void proc1(void){
    for (;;){
        printf("[proc1] idle m'a donne la main\n");
        printf("[proc1] je tente de lui la redonner...\n");
        ctx_sw(tableau_processus[1]->tab_registre, tableau_processus[0]->tab_registre);
    }
    
}
*/


/*
void idle(void)
{
    for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    ordonnance();
    }
}
void proc1(void) {
    for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    ordonnance();
    }
}

void proc2(void) {
    for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    ordonnance();
    }
}
void proc3(void) {
    for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    ordonnance();
    }
}
void proc4(void) {
    for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    ordonnance();
    }
}
void proc5(void) {
    for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    ordonnance();
    }
}
void proc6(void) {
    for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    ordonnance();
    }
}
void proc7(void) {
    for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    ordonnance();
    }
}
*/

/*les tests pour basculer les process par l'horloge de système*/

/* void idle(void)
{
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}
void proc1(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}
void proc2(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}
void proc3(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}
void proc4(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}
void proc5(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}
void proc6(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
void proc7(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
} 
}*/


void idle()
{
    for (;;) {
        sti();
        hlt();
        cli();
    }
/*     p tete_process->suiv->suiv->suiv->pid
    p tete_endormi->suiv->suiv->pid */
}
void proc1(void)
{
    for (;;) {
        printf("temps_ecoule : %d\n", temps_ecoule);
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
        mon_nom(), mon_pid());
        dors(2);
    }
}
void proc2(void)
{
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
        mon_nom(), mon_pid());
        dors(3);
    }
}
void proc3(void)
{
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
        mon_nom(), mon_pid());
        dors(5);
    }
}


