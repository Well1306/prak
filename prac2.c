#include <stdio.h>
// о
unsigned short swapbytes(unsigned short b) {
    unsigned short a = b & 0xFF00, c = b & 0x00FF;
    c = c << 8;
    a = a >> 8;
    c += a;
    return c;
}

int main(int argc, char ** argv) {
    FILE* inpf = stdin;
    FILE* outf = stdout;
    char ch;
    unsigned short utf16ch = 0;
    int i, flag = 0, ofs = 0;
    if(argc > 1) {
        if((inpf = fopen(argv[1], "r")) == NULL) {
            fprintf(stderr, "Cannot open input file\n");
            return 1;
        }
        if(argc > 2)
            if((outf = fopen(argv[2], "w")) == NULL) {
                fprintf(stderr, "Cannot open output file\n");
                return 2;
            }
    }
    while((ch = fgetc(inpf)) != EOF) {
        ofs++;
        if(utf16ch == 0) {
            utf16ch += (unsigned short) ch;
            if((ch & 0x80) != 0) {
                if((ch & 0xF0) == 0xE0) {
                    for(i = 1; i <= 2; i++) {
                        utf16ch = utf16ch << 6;
                        ch = fgetc(inpf);
                        ofs++;
                        if((ch & 0xC0) != 0x80) {
                            fprintf(stderr, "broken symbol %x in pos %d\n", (unsigned char) ch, ofs);
                            goto bc;
                        }
                        ch = ch & 0x3F;
                        utf16ch += (unsigned short) ch;
                    }
                    if((utf16ch == 0xFFFE) || (utf16ch == 0xFEFF)) {
                        flag = utf16ch == 0xFFFE;
                    }
                } else if((ch & 0xE0) == 0xC0) {
                    utf16ch = utf16ch << 6;
                    ch = fgetc(inpf);
                    ofs++;
                    if((ch & 0xC0) != 0x80) {
                        fprintf(stderr, "broken symbol %x in pos %d\n", (unsigned char) ch, ofs);
                        goto bc;
                    }
                    ch = ch & 0x3F;
                    utf16ch += (unsigned short) ch;
                    utf16ch = utf16ch & 0x07FF;
                } else {
                    fprintf(stderr, "broken symbol %x in pos %d\n", (unsigned char) ch, ofs);
                    goto bc;
                }
            }
        }
        if(flag) utf16ch = swapbytes(utf16ch);
        fwrite(&utf16ch, sizeof utf16ch, 1, outf);
bc:     utf16ch = 0;
    }
    fclose(inpf);
    fclose(outf);
    return 0;
}
