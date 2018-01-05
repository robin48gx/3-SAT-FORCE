#include <stdio.h>

char *to_bin(int b)
{
    static char buff[400];
    int k,i = 5;                  // bit pos 5, 6th bit

    b &= 0x3F;                  // 6 bit

    for (k = 0; k < 6; k++) {

        if (b & 0x20)
            buff[i] = '1';
        else
            buff[i] = '0';

        b <<= 1;
        i--;
    }

    return (char *)buff;
}

int check(char *rregx, int pattern)
{
    int i;

    printf("Checking 0x%02X against %s\n", pattern, rregx);
    for (i = 0; i < 6; i++) {
        if (rregx[i] == 'X')
            continue;           // DON't care
        if (rregx[i] == '0')
            if (((pattern >> i) & 0x00) == 0) {
                continue;       // correct
            } else
                return 1;       // FAIL
        // so must be looking for a '1'
        else if (((pattern >> i) & 0x00) == 1) {
            continue;           // correct
        } else
            return 1;           // FAIL

    }
    return 0;                   //        PASS
}

char *rx[14][3] = {
    {"X0XXXX", "XX0XXX", "XXXX1X"},     // .0.*|..0.*|.*1.
    {"0XXXXX", "XX0XXX", "XXXXX1"},     // 0.*|..0.*|.*1
    {"1XXXXX", "XX1XXX", "XXXX1X"},     // 1.*|..1.*|...1.*
    {"XX1XXX", "XXxX0X", "XXXXX1"},     // ..1.*|.*0.|.*1
    {"0XXXXX", "X1XXXX", "XXXXX0"},     // 0.*|.1.*|.*0
    {"X1XXXX", "XXX1XX", "XXXXX1"},     // .1.*|...1.*|.*1
    {"XXX0XX", "XXXX0X", "XXXXX0"},     // ...0.*|.*0.|.*0
    {"1XXXXX", "XXXX0X", "XXXXX1"},     // 1.*|.*0.|.*1
    {"XX0XXX", "XXX0XX", "XXXXX1"},     // ..0.*|...0.*|.*1

};

int main()
{
    int ii, j, k;
    int f;

    printf("rregx\n");
    for (ii = 0; ii < 6; ii++) {
        for (j = 0; j < 64; j++) {
            f = 0;
            for (k = 0; k < 3; k++) {
                printf(" 0x%X : check %d %d  :== %d\n", j, j, k, f += check(rx[ii][k], j));
            }
            if (f < 3) {
                // one passed!
                //
                printf("PASSED\n");
            } else {
                printf("FAILED\n");
            }
        }
    }
    return 0;
}
