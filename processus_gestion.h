#ifndef __PROCESSUS_GESTION__
#define __PROCESSUS_GESTION__

#include <stdint.h>
#include <stdbool.h>
#define NBR_PROCESSUS 8
#define TAILLE_PILE 512
#define MAX_NAME 10

typedef enum Etat{elu, activable, endormi}Etat;


typedef struct Processus{
    int pid;
    char nom_processus[MAX_NAME];
    Etat etat_proc;
    uint32_t tab_registre[5];
    uint32_t pile_execution[TAILLE_PILE];
    struct Processus *suiv;
    int temps_reveil;

}Processus;




//table de processus : pr l'ordonnaceur:
//switch the context
extern void ctx_sw(uint32_t tab_sauve[5], uint32_t tab_restau[5]);

//les fonctions principales
void struct_init();
int32_t cree_processus(void (*code)(void), char *nom);

void ordonnance(void);
void dors(uint32_t nbr_secs);

Processus *extraction_tete_activable();
Processus *extraction_tete_endormi();

void insert_queue(Processus *pro_insert);
void insert_endormi(Processus *pro_insert);

int32_t mon_pid(void);
char *mon_nom(void);
int nbr_secondes();




extern Processus *tableau_processus[NBR_PROCESSUS];


//fonction de test:
void idle(void);
void proc1(void);
void proc2(void);
void proc3(void);
/* void proc4(void);
void proc5(void);
void proc6(void);*/
void proc7(void); 

#endif
