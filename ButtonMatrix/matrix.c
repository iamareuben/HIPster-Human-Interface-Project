#include "define.h"
#include "matrix.h"

#include "../Generic/pwm.h"

enum matrixState_t button_matrix[4];

char current_row, current_col;

void matrix_init(void) {
    MATRIX_ROW_PORT_DIR &= ~_BV(MAT_ROW0)&~_BV(MAT_ROW1)&~_BV(MAT_ROW2)&~_BV(MAT_ROW3);     //Set row direction as inputs
    MATRIX_COLUMN_PORT_DIR |= _BV(MAT_COL0)|_BV(MAT_COL1);  			      //Ensure column pins are outputs.
    MATRIX_COLUMN_PORT2_DIR |= _BV(MAT_COL2);									  //Ensure column pins are outputs.
    MATRIX_ROW_PORT |= _BV(MAT_ROW0)|_BV(MAT_ROW1)|_BV(MAT_ROW2)|_BV(MAT_ROW3);     //Enable pull up resistors
    MATRIX_COLUMN_PORT |= _BV(MAT_COL0)|_BV(MAT_COL1);								     //Set columns high
    MATRIX_COLUMN_PORT2 |= _BV(MAT_COL2);												//Set columns high
}

void matrix_scan(void) {
    switch(current_col) {
        case 0:
            MATRIX_COLUMN_PORT |= _BV(MAT_COL1);
			MATRIX_COLUMN_PORT2 |= _BV(MAT_COL2);
            MATRIX_COLUMN_PORT &= ~_BV(MAT_COL0);
        break;
        case 1:
            MATRIX_COLUMN_PORT |= _BV(MAT_COL0);
			MATRIX_COLUMN_PORT2 |= _BV(MAT_COL2);
            MATRIX_COLUMN_PORT &= ~_BV(MAT_COL1);
        break;
		case 2:
            MATRIX_COLUMN_PORT |= _BV(MAT_COL0)|_BV(MAT_COL1);
            MATRIX_COLUMN_PORT2 &= ~_BV(MAT_COL2);
        break;
    }
    _delay_us(50);
    char port = (MATRIX_ROW_PORT_INPUT&(0x0f));
    for(uint8_t i = 0; i<4; i++) {
        if((((~port)&_BV(i))>>i)&&((button_matrix[i] == NONE)||current_col == 0))       //Make sure that two states aren't set
            button_matrix[i] = current_col+1; 
        else
            button_matrix[i] = NONE;
    }
    INC_MAX(current_col, NUM_COLS-1);
}

void matrix_action(void) {
    for(uint8_t i = 0; i<4; i++) {
        pwm_select(i, button_matrix[i]);
    }
}                        