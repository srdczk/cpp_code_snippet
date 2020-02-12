void nima() {}


void print_at(char x, int r, int c, char color) {
    char *memory = (char *)(0xb8000 + 2 * (80 * r + c));
    memory[0] = x;
    memory[1] = color;

}

void main() {

    print_at('X', 10, 40, 0x0c);

}
