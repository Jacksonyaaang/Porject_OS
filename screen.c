#include <stdio.h>

#include "screen.h"
#include "cpu.h"
#include "string.h"
#define LIGNE 25
#define COLONNE 80

uint32_t ligne_general=0;
uint32_t colonne_general=0;

uint16_t *ptr_mem(uint32_t lig, uint32_t col)
{
    uint16_t *ptr = (uint16_t *)(0xB8000 + 2*(lig*80 + col));

    return ptr;
}
//pass

void ecrit_car(uint32_t lig, uint32_t col, uint32_t cclignote,uint32_t cfond, uint32_t ctext,char c)
{
    uint16_t *ptr = ptr_mem(lig, col);
    *ptr = c | ctext<<8 | cfond<<12 |cclignote<<15;
}

void efface_ecran(void)
{
    char espace = ' ';
    for (int i=0;i<LIGNE;++i){
        for (int j=0;j<COLONNE;++j){
            ecrit_car(i, j, 0,0,15,espace);
        }
    }
}

void place_curseur(uint32_t lig, uint32_t col){
    uint16_t addr_cursuer = lig*80 + col;
    uint8_t add_basse = (uint8_t)(addr_cursuer);
    uint8_t add_haute = (uint8_t)((addr_cursuer &0b1111111100000000)>>8);
    outb(0x0F, 0x3D4);
    outb(add_basse, 0x3D5);
    outb(0x0E, 0x3D4);
    outb(add_haute, 0x3D5);
    ligne_general = lig;
    colonne_general = col;
}

void traite_car(char c){

    uint32_t tab_pas_avance;
    
    switch (c)
    {
    case 8 :  //Recule le curseur d’une colonne (si colonne ̸ = 0)
        if (colonne_general != 0){
            place_curseur(ligne_general, colonne_general-1);
        }
        break;
    case 9: //Avance à la prochaine tabulation (colonnes 0, 8, 16, ..., 72, 79)
        tab_pas_avance = 8-((colonne_general+1)%8);
        if (colonne_general +tab_pas_avance<= 79 ){
            place_curseur(ligne_general, colonne_general+tab_pas_avance);  
        }
        else{
            place_curseur(ligne_general, 79);
        }

        break;
    case 10: //Déplace le curseur sur la ligne suivante, colonne 0
        place_curseur(ligne_general+1, 0);
        break;
    case 12: //Efface l’écran et place le curseur sur la colonne 0 de la ligne 0
        efface_ecran();
        place_curseur(0,0);
        break;
    case 13: //Déplace le curseur sur la ligne actuelle, colonne 0
        place_curseur(ligne_general, 0);
        break;
    
    default:
        if (colonne_general+1<80){
            ecrit_car(ligne_general, colonne_general, 0,4,15,c);
            place_curseur(ligne_general, colonne_general+1);
        }
        else if (colonne_general+1>=80){
            if (ligne_general+1<25){
                ecrit_car(ligne_general, colonne_general, 0,4,15,c);
                place_curseur(ligne_general+1, 0);
            }
            else
            {
                ecrit_car(ligne_general, colonne_general, 0,4,15,c);
                defilement();
                place_curseur(ligne_general+1, 0);

            }
        }

        break;
    }
}

void defilement(void)
{
    uint16_t *case_lig_courant;
    uint16_t *case_ligne_prev;
    for (int i=1;i<LIGNE; ++i){
        for (int j=0;j<COLONNE;j++){
            case_ligne_prev = ptr_mem(i-1, j);
            case_lig_courant = ptr_mem(i, j);
            memmove(case_ligne_prev, case_lig_courant, sizeof(*case_lig_courant));
            if (i==24){
                ecrit_car(i, j, 0,0,15,' ');
            }

        }

    }
    place_curseur(ligne_general-1, colonne_general);
}

void console_putbytes(const char *s, int len){
    //efface_ecran();
    for (int i=0;i<len; i++){
        traite_car(*(s+i));
    }
}