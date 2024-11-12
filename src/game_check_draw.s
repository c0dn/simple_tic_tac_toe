// extern global variable defined in C
.extern x_board
.extern o_board

.global check_draw
check_draw:
    ldr x0, =x_board      // Load address of x_board
    ldr x0, [x0]          // load o_board to register x0

    ldr x1, =o_board      // Load address of o_board
    ldr x1, [x1]          // load o_board to register x1

    orr x0, x0, x1 // Bitwise OR: x0 = x0 | x1 to combine boards

    mov x1, #0x1FF        // Load 0b111111111 into x1, the representation of a full tic tae toe board
    cmp x0, x1            // Compare result with 0x1FF, if this is true it means the board is full

    cset x0, EQ           // Set x0 to 1 if equal, 0 if not

    ret
