#include "system.h"
#include "altera_up_avalon_accelerometer_spi.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include <stdlib.h>

#include "alt_types.h"
#include "sys/times.h"

#define OFFSET -32
#define PWM_PERIOD 16
#define TAPS (sizeof(mem)/sizeof(alt_32))

alt_8 pwm = 0;
alt_u8 led;
int level;

double a[] = { 0.004641, 0.007377, -0.002408, -0.007110, 0.003266, 0.000061,
		-0.009358, 0.003975, 0.004379, -0.013316, 0.003048, 0.011436, -0.017929,
		-0.001074, 0.022260, -0.022477, -0.010874, 0.039597, -0.026322,
		-0.033757, 0.075199, -0.028898, -0.120355, 0.287922, 0.636863, 0.287922,
		-0.120355, -0.028898, 0.075199, -0.033757, -0.026322, 0.039597,
		-0.010874, -0.022477, 0.022260, -0.001074, -0.017929, 0.011436,
		0.003048, -0.013316, 0.004379, 0.003975, -0.009358, 0.000061, 0.003266,
		-0.007110, -0.002408, 0.007377, 0.004641 };
//Quantised
#define EXP_COEFFICIENT_GAIN 12
alt_32 a_quant[] = { 19, 30, -10, -29, 13, 0, -38, 16, 18, -55, 12, 47, -73, -4,
		91, -92, -45, 162, -108, -138, 308, -118, -493, 1179, 2609, 1179, -493,
		-118, 308, -138, -108, 162, -45, -92, 91, -4, -73, 47, 12, -55, 18, 16,
		-38, 0, 13, -29, -10, 30, 19 };

alt_32 mem[49];
int ind;

//Timing
clock_t exec_t1, exec_t2;

void led_write(alt_u8 led_pattern) {
	IOWR(LED_BASE, 0, led_pattern);
}

void convert_read(alt_32 acc_read, int * level, alt_u8 * led) {
	//acc_read += OFFSET;
	alt_u8 val = (acc_read >> 6) & 0x07;
	*led = (8 >> val) | (8 << (8 - val));
	*level = (acc_read >> 1) & 0x1f;
}

void convert_data(int * level, alt_u8 * led) {
	int i;
	double res = 0;
	for (i = 0; i < TAPS; i++) {
		res += (double) mem[(ind + i) % TAPS] * a[i];
	}
	alt_32 filt_read = (alt_32) res;
	alt_u8 val = (filt_read >> 6) & 0x07;
	*led = (8 >> val) | (8 << (8 - val));
	*level = (filt_read >> 1) & 0x1f;
}

void convert_data_quant(int * level, alt_u8 * led) {
	int i;
	alt_64 res = 0;
	for (i = 0; i < TAPS; i++) {
		res += (alt_64) mem[(ind + i) % TAPS] * a_quant[i];
	}
	//printf("%d", res);
	alt_32 filt_read = (res >> EXP_COEFFICIENT_GAIN);
	alt_u8 val = (filt_read >> 6) & 0x07;
	*led = (8 >> val) | (8 << (8 - val));
	*level = (filt_read >> 1) & 0x1f;
}

void sys_timer_isr() {
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);

	if (pwm < abs(level)) {

		if (level < 0) {
			led_write(led << 1);
		} else {
			led_write(led >> 1);
		}

	} else {
		led_write(led);
	}

	if (pwm > PWM_PERIOD) {
		pwm = 0;
	} else {
		pwm++;
	}

}

void timer_init(void * isr) {

	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0003);
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);
	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, 0x0900);
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, 0x0000);
	alt_irq_register(TIMER_IRQ, 0, isr);
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0007);

}

int main() {
	ind = 0;
	alt_32 x_read;
	alt_up_accelerometer_spi_dev * acc_dev;
	acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");
	if (acc_dev == NULL) { // if return 1, check if the spi ip name is "accelerometer_spi"
		return 1;
	}

	timer_init(sys_timer_isr);

	exec_t1 = times(NULL); // get system time before starting the process

	while (1) {

		alt_up_accelerometer_spi_read_x_axis(acc_dev, &x_read);
		// alt_printf("raw data: %x\n", x_read);
		mem[ind] = x_read + OFFSET;
		ind++;
		ind = ind % TAPS;
		//convert_read(x_read, & level, & led);
		//convert_data(&level, &led);
		convert_data_quant(&level, &led);

		exec_t2 = times(NULL); // get system time after finishing the process
		int step = (int) (exec_t2 - exec_t1);
		printf("proc time = %d ticks \n", step);
		//printf("time = %d ticks \n", (int) exec_t2);
		exec_t1 = exec_t2;
	}

	return 0;
}

