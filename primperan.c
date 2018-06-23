
#include <stdlib.h>

#include "oldskool.h"

#include "effects.h"
#include "drawchar.h"

unsigned char *FontBuffer;
int oldskool = 0;

void primperan_exit(int val) {
	os_ticks_end();
	os_mod_end();
	os_fb_end();
	free(FontBuffer);
	exit(val);
}

static char    *SBuffer;

int main(int argc, char **argv) {
	oldskool = os_fb_init(argc, argv);
	if (oldskool == -1) {
		primperan_exit(1);
	}

	os_ticks_init(70); // 70hz

	SBuffer = os_fb_get_buffer();

	FontBuffer=(char *)malloc(1416*41);
	DeCrunchFont(FontBuffer);

	os_fb_clear_full(63);

	if (os_mod_start() == -1) {
		primperan_exit(1);
	}

	Initbuf();

	Title(SBuffer);

	Credits(SBuffer);

	Indicaciones(SBuffer);

	Aspirina(SBuffer);

	Pastillaz(SBuffer);

	Greetings(SBuffer);

	Secundarios(SBuffer);

	End(SBuffer);

	primperan_exit(0);
	return 0;
}

