#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <stdint.h>



uint16_t *ptr_mem(uint32_t lig, uint32_t col);
void ecrit_car(uint32_t lig, uint32_t col, uint32_t cclignote,uint32_t cfond, uint32_t ctext,char c);
void efface_ecran(void);
void place_curseur(uint32_t lig, uint32_t col);
void traite_car(char c);
void defilement(void);
void console_putbytes(const char *s, int len);


#endif
