#include <iostream>
#include <typeinfo>
#include <linux/input.h>
#include <linux/uinput.h>

using namespace std;

int main() {
	cout << "Mouse Events:" << endl;
	cout << "BTN_LEFT: " << BTN_LEFT << " |  BTN_RIGHT: " << BTN_RIGHT << " | BTN_MIDDLE: " << BTN_MIDDLE << endl;
	cout << "typeof(BTN_LEFT): " << typeid(BTN_LEFT).name() << endl << "typeof(KEY_A): " << typeid(KEY_A).name() << endl;

	cout << endl << "ABS_X: " << ABS_X << endl;
	cout << "KEY_UP: " << KEY_UP << endl;

	return 0;
}
