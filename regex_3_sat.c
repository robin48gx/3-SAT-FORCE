#include <stdio.h>

//--------------------------------------
// convert 6 bit binary to string
//
char *to_bin(int b)
{
    static char buff[400];
    int k, i = 0;               // bit pos 5, 6th bit
    b &= 0x3F;                  // 6 bit
    for (k = 0; k < 6; k++) {
        if (b & 0x20)
            buff[i] = '1';
        else
            buff[i] = '0';
        b <<= 1;
        i++;
    }
    return (char *)buff;
}

int check(char *rregx, int pattern)
{
    int i;
    int ret = 0;                // default pass

    int bit = 0x20 << 1;        // bit 6, becomes bit 5 on first pass for 6 bit word

    printf("Checking 0x%02X 0xB%s against %s\n", pattern, to_bin(pattern), rregx);
    for (i = 0; i < 6; i++) {
        bit >>= 1;

        // each bit check. correvct way round ?
        printf("\n: %d PATTERN=%s CHECK=%c 0x%X :", i, rregx, rregx[i], bit);

        if (rregx[i] == 'X') {
            printf(" pass X ");
        }

        if (rregx[i] == '0') {
            if ((pattern & bit) == 0) {
                printf(" pass-0 ");
            } else {
                printf(" fail-0 ");
                ret = 1;        // FAIL
            }
        }

        if (rregx[i] == '1') {
            if ((pattern & bit)) {
                printf(" pass-1 ");
            } else {
                printf(" fail-1 ");
                ret = 1;        // FAIL
            }
        }



    } // for all six bits


    if (ret) // if ret true an error was found applying one pattern to the inout binary number
        printf(" fails...\n");
    else
        printf(" passes...\n");

    return ret;                 //        PASS
}

char *rx[14][3] = {
    {"X0XXXX", "XX0XXX", "XXXX1X"},     // .0.*|..0.*|.*1. // 1st // .0.*|..0.*|.*1.                    
    {"0XXXXX", "XX0XXX", "XXXXX1"},     // 0.*|..0.*|.*1    // 2nd // 0.*|..0.*|.*1

    {"1XXXXX", "XX1XXX", "XXX1XX"},     // 1.*|..1.*|...1.* // 3rd // 1.*|..1.*|...1.*
    {"XX1XXX", "XXXX0X", "XXXXX1"},     // ..1.*|.*0.|.*1  // 4th // ..1.*|.*0.|.*1     
    {"0XXXXX", "X1XXXX", "XXXXX0"},     // 0.*|.1.*|.*0    // 5th // 0.*|.1.*|.*0       
    {"X1XXXX", "XXX1XX", "XXXXX1"},     // .1.*|...1.*|.*1 // 6th // .1.*|...1.*|.*1
    {"XXX0XX", "XXXX0X", "XXXXX0"},     // ...0.*|.*0.|.*0 // 7th // ...0.*|.*0.|.*0

    {"1XXXXX", "XXXX0X", "XXXXX1"},     // 1.*|.*0.|.*1 // 8th // 1.*|.*0.|.*1
    {"1XXXXX", "XXXX0X", "XXXXX0"},     // RRRRRRRRRRRRRRRR // 9th // 1.*|.*0.|.*0

    {"1XXXXX", "XXXX0X", "XXXXX0"},     // 1.*|.*0.|.*0    // 10th // 1.*|.*0.|.*0
    {"1XXXXX", "XXX0XX", "XXXX1X"},     // 1.*|...0.*|.*1. // 11th // 1.*|...0.*|.*1.                                                   

    {"0XXXXX", "XXX0XX", "XXXX1X"},     // 0.*|...0.*|.*1. // 12th // 0.*|...0.*|.*1.

    {"XX1XXX", "XXX1XX", "XXXXX0"},     // ..1.*|...1.*|.*0  // 13th // ..1.*|...1.*|.*0
    {"XX0XXX", "XXX1XX", "XXXXX0"},     // ..0.*|...1.*|.*0  // 14th // ..0.*|...1.*|.*0

};

int main()
{
    int ii, j, k, cc;
    int failed;
    int triplet_has_failed;

    printf("rregx\n");
    for (j = 0; j < 64; j++) { // for all possible patterns i.e. binary 6 bit number

	triplet_has_failed = 0;

        for (ii = 0; ii < 14; ii++) {   // for all 14 sets of ored regexs
	    if ( triplet_has_failed ) continue; // move to next 6 bit pattern, this one has already failed
	    printf("moving to %d th regex\n",ii+1);
            failed = 0; // make it get cleared if one triplet passes

	    printf("--------------------\n");
            for (k = 0; k < 3; k++) {   // for the three regexs in the ored set

                //printf("TRIPLE CHECK 0x%X :  %d %d  :== %d\n", j, j, k, cc = check(rx[ii][k], j));
		cc = check(rx[ii][k], j);
		// if one of the three patterns passed then its OK
                if (cc == 1)
                    failed++; // so count the number of times it fails
            }
	    printf("--------------------\n");

                //printf("\n %dth REGEX--------- for 0x%X 0b%s-----%s %s %s------->\n",
		//	       	ii+1,j, to_bin(j), rx[ii][0], rx[ii][1],rx[ii][2]);
            if (failed<3) {
                // at least one passed!
                //
                printf("PASSED for 0x%X failed=%d\n", j, failed);
            } else {
                printf("FAILED for 0x%X failed = %d\n", j, failed);

                //printf("FAILED----on %dth REGEX--------- for 0x%X 0b%s-----%s %s %s------->\n",
	        //		       	ii+1,j, to_bin(j), rx[ii][0], rx[ii][1],rx[ii][2]);
                triplet_has_failed = 1; // no one of the fourteen triplets of regexs FAILED.
            }
        }

        if (!triplet_has_failed)
            printf(" Pattern 0x%X 0b%s passed\n", j, to_bin(j));
        else {
            printf(" Pattern 0x%X 0b%s failed\n", j, to_bin(j));
	    continue; // no point carrying on, if all theree failed then this bit pattern does not work
	}

    }
    return 0;
}
