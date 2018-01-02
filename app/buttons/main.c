#include <main.h>
#include <input.h>

int main()
{
	gpio_led_init();
	button_init();
	while(1);
}
