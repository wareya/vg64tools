#include <mips.h>
#include "../dodojr.h"

        .text
        
        /*
        *# Original address: 0x809f799c
        */

        .set            noreorder
        .set            noat
        .global         En_Dodojr_f30
        .ent            En_Dodojr_f30

En_Dodojr_f30:        

        addiu   sp,sp,-32
        sw      ra,28(sp)
        sw      s0,24(sp)
        sw      a1,36(sp)
        lw      t6,4(a0)
        lui     $at,0x100
        move    s0,a0
        or      t7,t6,$at
        jal     0x8002d868
        sw      t7,4(a0)
        move    a0,s0
        jal     En_Dodojr_f6
        lw      a1,36(sp)
        beqzl   v0,$Lf30_01
        lw      ra,28(sp)
        jal     En_Dodojr_f7
        move    a0,s0
	
        #lui     t8,0x809f		# 0x809F758C
        #addiu   t8,t8,30092		# En_Dodojr_f23
	la	t8,En_Dodojr_f23
	
        sw      t8,400(s0)
        lw      ra,28(sp)
$Lf30_01:
        lw      s0,24(sp)
        addiu   sp,sp,32
        jr      ra
        nop
        
        .end            En_Dodojr_f30
        .set            at
        .set            reorder

