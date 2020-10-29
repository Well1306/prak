#include <stdio.h>
// о
unsigned short swapbytes(unsigned short b) {
    unsigned short a = b & 0xFF00;
    b = b << 8;
    a = a >> 8;
    b += a;
    return b;
}

int main(int argc, char ** argv) {
    FILE* inpf = stdin;
    FILE* outf = stdout;
    char ch, bs = 0;
    unsigned short utf16ch = 0;
    int i, flag = 0, ofs = 0, bofs = 0;
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
                            bs = ch;
                            bofs = ofs;
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
                        bs = ch;
                        bofs = ofs;
                    }
                    ch = ch & 0x3F;
                    utf16ch += (unsigned short) ch;
                    utf16ch = utf16ch & 0x07FF;
                } else {
                    bs = ch;
                    bofs = ofs;
                }
            }
        }
        if(flag) utf16ch = swapbytes(utf16ch);
        if(bs && ofs) {
            fprintf(stderr, "Broken symbol %x in pos %d\n", (unsigned char) bs, bofs);
            bs  = bofs = 0;
        }
        else fwrite(&utf16ch, sizeof utf16ch, 1, outf);
        utf16ch = 0;
    }
    fclose(inpf);
    fclose(outf);
    return 0;
}
