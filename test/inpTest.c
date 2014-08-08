#include "inputHandler.h"

int main() {
	if (setup_uinput_device() < 0) {
		printf("Unable to find uinput device.\n");
		return -1;
	}
	
	sleep(5);
	kb_press_key(30);
	kb_press_key(30);
	close_uinput_device();
}
