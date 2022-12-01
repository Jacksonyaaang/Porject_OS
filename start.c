#include <cpu.h>
#include <inttypes.h>
#include <stdio.h>
#include "screen.h"
#include "clock_re.h"
#include "processus_gestion.h"
#include "tinyalloc.h"

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display


void kernel_start(void)
{
    efface_ecran();

    struct_init();
    //printf("%i\n", mon_pid());
    idle();



    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}

