#include <iostream>

#include <math/matrix_utils.h>
#include <swan_wb/swan_wb_generator.h>
#include <swan/swan.h>

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

    double dur;
    clock_t start,end;
    

    CombinedAffine *ptr = (CombinedAffine*)malloc(sizeof(CombinedAffine));
    combined_affine_init(ptr, 4, 4);
    combined_affine_free(ptr);
    free(ptr);
    ptr = NULL;
    cout << "hello" << endl;

    uint8_t k[] = {0xff, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff};
    uint8_t in[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t out[16] = {0};

    start = clock();

    // SWAN_Encrypt_64_main();
    SWAN_Encrypt_64( (char *)in, (char *)k, (char *)out);

    end = clock();

    print_u8(in, 8);
    print_u8(out, 8);

    dur = (double)(end - start);
    printf("\nEnc Use Time:%f s\n\n",(dur/CLOCKS_PER_SEC));

    swan_whitebox_content swc;

    start = clock();

    swan_whitebox_64_init(k, SWAN_ENCRYPT, &swc);

    end = clock();
    dur = (double)(end - start);
    printf("Generate Whitebox Use Time:%f s\n",(dur/CLOCKS_PER_SEC));


    print_u8(in, 8);

    start = clock();

    swan_wb_encrypt(&swc, in, out);

    end = clock();
    dur = (double)(end - start);

    print_u8(out, 8);
    printf("Whitebox Enc Use Time:%f s\n",(dur/CLOCKS_PER_SEC));


    swan_whitebox_content swct;
    uint8_t * swct_ptr = NULL;

    swan_wb_export_to_bytes(&swc, &swct_ptr);

    swan_wb_import_from_bytes(swct_ptr, &swct);

    start = clock();

    swan_wb_encrypt(&swct, in, out);

    end = clock();
    dur = (double)(end - start);

    print_u8(out, 8);
    printf("Whitebox copy Enc Use Time:%f s\n",(dur/CLOCKS_PER_SEC));

    swan_whitebox_release(&swct);

    swan_whitebox_release(&swc);

    return 0;
}
