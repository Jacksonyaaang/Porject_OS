#include <stdio.h>
#include <stdbool.h>

#include "screen.h"
#include "cpu.h"
#include "string.h"
#include "clock_re.h"
#include "segment.h"
#define TAILLE_TIME 8

int temps_ecoule = 0;
int horloge = 0;



void display_hours(char *s){

    int length_s = strlen(s);
    uint32_t ligne_hours = 0;
    uint32_t colonne_hours = 80-length_s;
    
    for (int i=0;i<length_s;++i){
        ecrit_car(ligne_hours, colonne_hours+i, 0, 1, 15, *(s+i));
    }
}

void tic_PIT(void){
    outb(0x20, 0x20);  //PIC, tu doit écoute les interruptions mtn
    temps_ecoule++;

    if (temps_ecoule%50 == 0){
        horloge++;
        int tt_seconde =temps_ecoule/50;
        int seconde = (tt_seconde)%60;
        int minute = (tt_seconde/60)%60;
        int hours = tt_seconde/3600;
        char buffer[TAILLE_TIME];
        sprintf(buffer, "%d:%d:%d", hours, minute, seconde);
        display_hours(buffer);  //le traiter --> vraiment 
    }
    ordonnance();

}

void init_traitant_IT(uint32_t num_IT, void (*traitant)(void)){
    uint32_t *addr_depart = (uint32_t *)(0x1000 + num_IT*8); //pcq chaque case contient 8 octetsQUARTZ
    uint32_t  faible_traitant = (uint32_t )traitant & 0x0000ffff;
    uint32_t fort_traitant = (uint32_t)traitant &0xffff0000;    
    uint32_t premier_mot = KERNEL_CS<<16 | faible_traitant;
    uint32_t seconde_mot = fort_traitant |0x8E00;
    *addr_depart = premier_mot;
    *(addr_depart+1) = seconde_mot;
}




void change_frequency(){
    outb(0x34, 0x43);
    int CLOCKFREQ = 50;
    int QUARTZ = 0x1234DD;  
    int message_send = (QUARTZ/CLOCKFREQ);
    outb(message_send%256, 0x40); //les bits faibles
    outb(message_send>>8, 0x40);
    masque_IRQ(0,0);
    init_traitant_IT(32,traitant_IT_32);
}


void masque_IRQ(uint32_t num_IRQ, bool masque){
    uint8_t tab_masquage = inb(0x21);
    uint8_t etat_bit = (tab_masquage>>num_IRQ) & 0b1;
    if (masque ==1 && etat_bit ==0){
        tab_masquage += 1<<num_IRQ;
    }else if (masque == 0 && etat_bit ==1){
        tab_masquage -= 1<<num_IRQ;
    }

    outb(tab_masquage, 0x21);
}

