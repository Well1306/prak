#include <stdio.h>
//sa
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
    unsigned short a = 0, utf16ch = 0;
    int be = 0, ofs = 0;
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
    if((fread(&utf16ch, sizeof utf16ch, 1, inpf)) == 0) goto end;
    be = utf16ch == 0xFFFE;
    goto bom;
    while((fread(&utf16ch, sizeof utf16ch, 1, inpf)) != 0) {
bom:    ofs++;
        if(be) utf16ch = swapbytes(utf16ch);
        if((utf16ch & 0xF800) != 0) {
            a = utf16ch & 0xF000;
            a = a >> 12;
            ch = 0xE0 + (char) a;
            fwrite(&ch, 1, 1, outf);
            a = utf16ch & 0x0FC0;
            a = a >> 6;
            ch = 0x80 + (char) a;
            fwrite(&ch, 1, 1, outf);
            a = utf16ch & 0x003F;
            ch = 0x80 + (char) a;
            fwrite(&ch, 1, 1, outf);
        } else {
            if((utf16ch & 0x0780) != 0) {
                a = utf16ch & 0x07C0;
                a = a >> 6;
                ch = 0xC0 + (char) a;
                fwrite(&ch, 1, 1, outf);
                a = utf16ch & 0x003F;
                ch = 0x80 + (char) a;
                fwrite(&ch, 1, 1, outf);
            } else {
                if((utf16ch & 0xFF80) == 0) {
                    ch = (char) utf16ch;
                    fwrite(&ch, 1, 1, outf);
                } 
            }
        }
    utf16ch = 0;
    }
end:if(feof(inpf) && utf16ch) fprintf(stderr, "Odd number of bytes\n");
    fclose(inpf);
    fclose(outf);
    return 0;
}
