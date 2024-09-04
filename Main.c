#include <avr/io.h>
#include <util/delay.h>

void flash_leds();
void shift_left();
void shift_right();
void converge_leds();
void diverge_leds();
void ping_pong();
void snake_effect();
void converge_diverge_leds();

int main(void) {
    DDRA = 0xFF;  // Set Port A as output for LEDs
    DDRB = 0x00;  // Set Port B as input for DIP switch
    PORTB = 0xFF; // Enable pull-up resistors on Port B

    while (1) {
        switch (PINB) {  // Read the DIP switch value
            case 0b11111111:
                flash_leds();
                break;
            case 0b11111110:
                shift_left();
                break;
            case 0b11111101:
                shift_right();
                break;
            case 0b11111100:
                converge_leds();
                break;
            case 0b11111011:
                diverge_leds();
                break;
            case 0b11111010:
                ping_pong();
                break;
            case 0b11111001:
                snake_effect();
                break;
            case 0b11111000:
                converge_diverge_leds();
                break;
            default:
                PORTA = 0x00; // Turn off all LEDs
                break;
        }
    }
}

void flash_leds() {
    while (PINB == 0b11111111) {  // Continue flashing as long as the switch is in this position
        PORTA = 0xFF;
        _delay_ms(500);
        PORTA = 0x00;
        _delay_ms(500);
    }
}

void shift_left() {
    while (PINB == 0b11111110) {  // Continue shifting left as long as the switch is in this position
        for (uint8_t i = 0; i < 8; i++) {
            PORTA = (1 << i);
            _delay_ms(250);
            if (PINB != 0b11111110) return;  // Exit if switch position changes
        }
    }
}

void shift_right() {
    while (PINB == 0b11111101) {  // Continue shifting right as long as the switch is in this position
        for (int8_t i = 7; i >= 0; i--) {
            PORTA = (1 << i);
            _delay_ms(250);
            if (PINB != 0b11111101) return;  // Exit if switch position changes
        }
    }
}

void converge_leds() {
    while (PINB == 0b11111100) {  // Continue converging as long as the switch is in this position
        for (uint8_t i = 0; i < 4; i++) {
            PORTA = (1 << i) | (1 << (7 - i));
            _delay_ms(300);
            if (PINB != 0b11111100) return;  // Exit if switch position changes
        }
    }
}

void diverge_leds() {
    while (PINB == 0b11111011) {  // Continue diverging as long as the switch is in this position
        for (int8_t i = 3; i >= 0; i--) {
            PORTA = (1 << i) | (1 << (7 - i));
            _delay_ms(300);
            if (PINB != 0b11111011) return;  // Exit if switch position changes
        }
    }
}

void ping_pong() {
    while (PINB == 0b11111010) {  // Continue ping-ponging as long as the switch is in this position
        // Move LEDs from left to right
        for (uint8_t i = 2; i < 7; i++) {
            PORTA = (1 << i);
            if (i == 6) {
                PORTA |= (1 << (i + 1));
            }
            _delay_ms(750);
            if (PINB != 0b11111010) return;  // Exit if switch position changes
        }

        // Move LEDs from right to left
        for (int8_t i = 6; i >= 1; i--) { // Start from 6 to avoid repeating the last LED
            PORTA = (1 << i);
            if (i == 1) {
                PORTA |= (1 << (i - 1));
            }
            _delay_ms(750);
            if (PINB != 0b11111010) return;  // Exit if switch position changes
        }
    }
}

void snake_effect() {
    static uint8_t pattern = 0x01;  // Start with the first LED on

    while (PINB == 0b11111001) {  // Continue the snake effect as long as the switch is in this position
        PORTA = pattern;  // Output the current pattern to Port A
        _delay_ms(500);

        pattern = (pattern << 1) | pattern;  // Turn on the next LED

        if (PINA == 0xFF) {  // If all LEDs are on
            pattern = 0x01;  // Reset to only the first LED on
        }

        if (PINB != 0b11111001) return;  // Exit if switch position changes
    }
}

void converge_diverge_leds() {
    while (PINB == 0b11111000) {  // Continue converging and diverging as long as the switch is in this position
        for (uint8_t i = 0; i < 4; i++) {
            PORTA = (1 << i) | (1 << (7 - i));
            _delay_ms(300);
            if (PINB != 0b11111000) return;  // Exit if switch position changes
        }
        for (int8_t i = 3; i >= 0; i--) {
            PORTA = (1 << i) | (1 << (7 - i));
            _delay_ms(300);
            if (PINB != 0b11111000) return;  // Exit if switch position changes
        }
    }
}
