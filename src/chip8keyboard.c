#include "chip8keyboad.h"
#include <assert.h>

static void chip8_keyboard_ensure_bounds(int key){
    assert(key >= 0 && key <= CHIP8_TOTAL_KEY);
}

int chip8_keyboard_map(const char* map, char key) {
    int low = 0;
    int high = (sizeof(map)/sizeof(map[0])) - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (map[mid] == key) {
            return mid; 
        }
        if (map[mid] < key) {
            low = mid + 1; 
        } else {
            high = mid - 1;  
        }
    }
    return -1;  
}

void chip8_keyboard_down(struct chip8_keyboard* keyboard, int key){
    keyboard->keyboard[key] = true;
}

void chip8_keyboard_up(struct chip8_keyboard* keyboard, int key){
    keyboard->keyboard[key] = false;
}

bool chip8_keyboard_is_down(struct chip8_keyboard* keyboard, int key){
    return keyboard->keyboard[key];
}

