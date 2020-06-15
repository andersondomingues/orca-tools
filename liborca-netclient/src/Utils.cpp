#include <Utils.h>
#include <stdint.h>
#include <stdio.h>

void Orca::NetClient::Utils::HexDump(int8_t *buf, uint32_t size){
	
    uint32_t k, l;
	int8_t ch;
	
	for(k = 0; k < size; k += 16){
		printf("\n%08x ", (unsigned int)(unsigned long int)(buf + k));
		for(l = 0; l < 16; l++){
			printf("%02x ", (uint8_t) buf[k + l]);
			if (l == 7) putchar(' ');
		}
		printf(" |");
		for(l = 0; l < 16; l++){
			ch = buf[k + l];
			if ((ch >= 32) && (ch <= 126))
				putchar(ch);
			else
				putchar('.');
		}
		putchar('|');
	}
}