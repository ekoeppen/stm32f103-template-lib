#include <clocks.h>
#include <gpio.h>
#include <ringbuffer.h>
#include <usb/usb.h>
#include <usb/cp2102.h>

typedef HSE_OSC_T<> HSE;
typedef PLL_T<HSE, 72000000> PLL;
typedef SYSCLK_T<PLL> SYSCLK;
typedef SYSTICK_T<SYSCLK> SYSTICK;

typedef GPIO_T<PB, 7, INPUT, FLOATING, LOW, INTERRUPT_ENABLED, EDGE_FALLING> BUTTON;
typedef GPIO_T<PB, 8, OUTPUT_10MHZ, PUSH_PULL, LOW> LED1;
typedef GPIO_T<PB, 9, OUTPUT_10MHZ, PUSH_PULL, LOW> LED2;
typedef GPIO_T<PB, 12, OUTPUT_10MHZ, PUSH_PULL, LOW> D0;
typedef GPIO_T<PB, 13, OUTPUT_10MHZ, PUSH_PULL, LOW> D1;
typedef GPIO_T<PB, 14, OUTPUT_10MHZ, PUSH_PULL, LOW> D2;
typedef GPIO_T<PB, 15, OUTPUT_10MHZ, PUSH_PULL, LOW> D3;

typedef GPIO_PORT_T<PA> PORT_A;
typedef GPIO_PORT_T<PB, LED1, LED2, BUTTON, D0, D1, D2, D3> PORT_B;

typedef RINGBUFFER::T<uint8_t, 128> TX_BUFFER;
typedef RINGBUFFER::T<uint8_t, 256> RX_BUFFER;
typedef USB::CP2102_UART_T<PLL, TX_BUFFER, RX_BUFFER> UART;

extern "C" {

void USB_HP_CAN1_TX_IRQHandler(void) {
	if (UART::handle_hp_irq()) exit_idle();
}

void USB_LP_CAN1_RX0_IRQHandler(void) {
	if (UART::handle_lp_irq()) exit_idle();
}

}

int main(void)
{
	uint32_t count = 0;
	char buffer[128];

	HSE::init();
	PLL::init();
	SYSCLK::init();
	SYSTICK::init();
	PORT_A::init();
	PORT_B::init();
	UART::init();
	LED1::set_high();
	while (!UART::dtr() && !UART::rts()) {
		enter_idle();
	}
	LED1::set_low();
	SYSTICK::set_and_wait(100);
	UART::puts("Echo test\n");
	while(1) {
		LED2::toggle();
		//printf<UART>("[%d] Enter a string:\n", count++);
		while (!UART::gets(buffer, sizeof(buffer))) {
			LED1::set_high();
			enter_idle();
			LED1::set_low();
		}
		UART::putc('\n');
		UART::puts(buffer);
		//printf<UART>("\nModem state: %x\n", UART::handshaking_state);
	}
	return 0;
}
