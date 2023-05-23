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
