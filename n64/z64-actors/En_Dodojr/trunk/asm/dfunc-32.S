#include <mips.h>
#include "../dodojr.h"

        .text
        
        /*
        *# Original address: 0x809f7ab8
        */

        .set            noreorder
        .set            noat
        .global         En_Dodojr_f32
        .ent            En_Dodojr_f32

En_Dodojr_f32:        

        addiu   sp,sp,-40
        sw      ra,36(sp)
        sw      s0,32(sp)
        move    s0,a0
        jal     0x8002d868
        sw      a1,44(sp)
        li      t6,10
        sw      t6,16(sp)
        addiu   a0,s0,182
        move    a1,zero
        li      a2,4
        jal     0x8007869c
        li      a3,1000
        lh      t7,180(s0)
        move    a0,s0
        sh      t7,48(s0)
        jal     En_Dodojr_f6
        lw      a1,44(sp)
        beqz    v0,$Lf32_01
        li      t8,60
        sh      t8,514(s0)
        jal     En_Dodojr_f9
        move    a0,s0
        li      t9,7
        #lui     t0,0x809f		# 0x809F7B3C
        #addiu   t0,t0,31548		# En_Dodojr_f33
	la	t0,En_Dodojr_f33
        sh      t9,508(s0)
        sw      t0,400(s0)
$Lf32_01:
        lw      ra,36(sp)
        lw      s0,32(sp)
        addiu   sp,sp,40
        jr      ra
        nop
        
        .end            En_Dodojr_f32
        .set            at
        .set            reorder
