#include <iostream>

using namespace std;

void print_u8(uint8_t *out, int len) {
    int ct = 8;
    for (int i=0; i<len; i++) {
        if (i%ct==0) printf("\n");
        printf("%02X", *(out+i));
    }
    printf("\n");
}

int main(int argc, char **argv) 
{
    
    cout << "hello" << endl;
    return 0;
}
