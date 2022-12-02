#include <stdio.h>

#include "processus_gestion.h"
#include "tinyalloc.h"
#include "cpu.h"
#include "screen.h"
#include "string.h"


//switch the context
void ctx_sw(uint32_t tab_sauve[5], uint32_t tab_restau[5]);

Processus *tableau_processus[NBR_PROCESSUS];
//les variables globales
Processus *actif = NULL; //the current actif process
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

    cree_processus(proc1, "proc1");
    cree_processus(proc2, "proc2");
    cree_processus(proc3, "proc3");
    cree_processus(proc4, "proc4");
    cree_processus(proc5, "proc5");
    cree_processus(proc6, "proc6");
    cree_processus(proc7, "proc7");

}



int32_t mon_pid(void){
    return actif->pid;
}
char *mon_nom(void){
    return actif->nom_processus;
}

void ordonnance(void){
    Processus *next_pro = NULL;
    int index_next;
    if (actif->pid != 7){
        index_next = (actif->pid)+1;
    }else 
    {
        index_next = 0;
    }
    next_pro = tableau_processus[index_next];

    next_pro->etat_proc = elu;
    actif->etat_proc = activable;
    Processus *save_current = actif;
    actif = next_pro;
    ctx_sw(save_current->tab_registre, next_pro->tab_registre);
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


