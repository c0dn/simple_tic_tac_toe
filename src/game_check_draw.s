// extern global variable defined in C
.extern x_board
.extern o_board

.global check_draw
check_draw:
    adrp x0, x_board                // Load the page address of x_board into x0
    add x0, x0, :lo12:x_board       // Calculate the full address of x_board
    ldrh w0, [x0]                   // Load half word (16 bits) into w0


    adrp x1, o_board                // Load the page address of o_board into x1
    add x1, x1, :lo12:o_board       // Calculate the full address of o_board
    ldrh w1, [x1]                   // Load half word (16 bits) into w1

    orr w0, w0, w1 // Bitwise OR: x0 = x0 | x1 to combine boards

    mov w1, #0x1FF        // Load 0b111111111 into x1, the representation of a full tic tae toe board
    cmp w0, w1            // Compare result with 0x1FF, if this is true it means the board is full

    cset x0, EQ           // Set x0 to 1 if equal, 0 if not

    ret
