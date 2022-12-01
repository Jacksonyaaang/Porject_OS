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


void create_process(void (*fct)(void), char *name){
    Processus *process_next= malloc(sizeof(*process_next));
    process_next->pid =1;
    process_next->etat_proc = activable;
    sprintf(process_next->nom_processus, "%s", name);
    process_next->tab_registre[1] = (uint32_t)&(process_next->pile_execution[TAILLE_PILE-1]);
    process_next->pile_execution[TAILLE_PILE-1] = (uint32_t)fct;
    tableau_processus[1] = process_next;
}


void struct_init(){
    Processus *process_depart = malloc(sizeof(*process_depart));
    process_depart->pid = 0;
    strcpy(process_depart->nom_processus, "idle");
    process_depart->etat_proc =  elu;
    tableau_processus[0] = process_depart;

    actif = process_depart; //initialization the current process

    create_process(proc1, "proc1");
}



int32_t mon_pid(void){
    return actif->pid;
}
char *mon_nom(void){
    return actif->nom_processus;
}

void ordonnance(void){
    Processus *next_pro = NULL;
    for (int i=0;i<NBR_PROCESSUS;++i){
        if (tableau_processus[i]->etat_proc == activable){
            next_pro = tableau_processus[i];
            break;
        }
    }   
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



