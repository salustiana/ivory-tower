#include <stdio.h>

// max word length
#define MWL 100
// max histogram height
#define MHH 30.0

int main()
{
    int c, i;
    int wl = 0;
    int wls[MWL];
    for (i=0; i<MWL; i++)
        wls[i] = 0;

    // read input until EOF
    while ((c = getchar()) != EOF) {
        // these chars end a word
        if (c==' '||c=='\t'||c=='\n') {
            // if a word is ended, inc
            // the count for its length
            if (wl>0) {
                wls[wl]++;
                wl = 0;
            }
        }
        // inc current word length
        else
            wl++;
    }

    // set lw to len of longest word
    int lw = MWL-1;
    for (; lw>0 && wls[lw]==0; lw--)
        ;

    // scale down if necessary
    float ma = 0.0;
    for (i=0; i<=lw; i++) {
        if (wls[i] > ma)
            ma = wls[i];
    }

    float f;
    f = MHH/ma;
    if (f < 1) {
        int nv;
        for (i=0; i<=lw; i++) {
            if (wls[i] != 0) {
                nv = wls[i]*f;
                if (nv < 1)
                    wls[i] = 1;
                else
                    wls[i] = wls[i]*f;
            }
        }
        ma = (int) MHH;
    }

    int j;
    for (i=ma; i>0; i--) {
        for (j=0; j<=lw+1; j++) {
            if (wls[j] >= i)
                printf(" ^ ");
            else
                printf("   ");
        }
        putchar('\n');
    }
    // bottom
    for (j=0; j<=lw+1; j++)
        printf("--|");
    putchar('\n');
    for (j=0; j<=lw+1; j++)
        printf("%2d|", j);
    putchar('\n');
}
