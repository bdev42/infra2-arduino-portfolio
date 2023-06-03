/*
// li r2, 0x69
// li r4, 4
// li r3, 0x20
// li r1, 8
// shl r4, r1
// or r4, r3
// st r4, (r2)
// li r3, 1
// sub r2, r3
// st r1, (r2)
// ld r1, (r2)
// li r10, 0
// li r9, :loop
// :loop
// nop r11, r1
// sub r11, r10
// jpnz r9
// halt 


#define PROGRAM {   \
    0x1692, 0x1044, \
    0x1203, 0x1081, \
    0x4641, 0x4143, \
    0xa042, 0x1013, \
    0x4523, 0xa012, \
            0x8012, \
    0x100a, 0x10d9, \
    0x40b1, 0x44a3, \
    0x45ba, 0xf039, \
    0x0000, 0x0000, \
    0x0000, 0x0000, \
}
*/

///*
/*
MEMORY ALLOCATION TEST PROGRAM

li r0, 0 ;current block
li r10, 1
li r8, 8
li r6, 1
shl r6, r8
:newblock
li r2, 1
li r11, :loop
:loop
nop r1, r0
shl r1, r8
or r1, r2
st r1, (r1)
shl r2, r10
nop r5, r2
sub r5, r6
jpnz r11
add r0, r10
li r11, :newblock
jp r11
*/
#define PROGRAM {                   \
    0x1000, 0x101a, 0x1088, 0x1016, \
    0x4668, 0x1012, 0x107b, 0x4010, \
    0x4618, 0x4112, 0xa011, 0x462a, \
    0x4052, 0x4556, 0xf03b, 0x440a, \
    0x105b, 0xc00b,                 \
}
//*/