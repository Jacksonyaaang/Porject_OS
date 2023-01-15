#ifndef __CLOCK_RE__
#define __CLOCK_RE__

#include <stdint.h>
#include <stdbool.h>
#include "processus_gestion.h"

extern void traitant_IT_32(void);


void display_hours(char *s);
void tic_PIT(void);
void init_traitant_IT(uint32_t num_IT, void (*traitant)(void));
void change_frequency();
void masque_IRQ(uint32_t num_IRQ, bool masque);


#endif
