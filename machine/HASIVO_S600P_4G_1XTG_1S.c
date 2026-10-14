#include "machine_defs.h"

/* Sold as Mokerlink 2G04210GSMX. Same ports as the ZX310S-4T2XH, but a
 * different LED wiring and the SDS0 setting in machine_custom_init() */
__code const struct machine machine = {
	.machine_name = MACHINE_MODEL,
	.isRTL8373 = 0,
	.min_port = 3,
	.max_port = 8,
	.n_sfp = 1,
	.n_10g = 1,
	.log_to_phys_port = {0, 0, 0, 5, 1, 2, 3, 4, 6},
	.phys_to_log_port = {4, 5, 6, 7, 3, 8, 0, 0, 0},
	.is_sfp = {0, 0, 0, 0, 0, 0, 0, 0, 1},
	.sfp_port[0].pin_detect = GPIO38,
	.sfp_port[0].pin_los = GPIO_NA,
	.sfp_port[0].pin_tx_disable = GPIO_NA,
	.sfp_port[0].sds = 1,
	.sfp_port[0].i2c = I2CBUS( GPIO39_I2C_SDA4, GPIO40_I2C_SCL3_MDC1 ),
	.reset_pin = GPIO48_I2C_SCL1,
	.high_leds = { .mux =  LED_28_SYS, .enable = LED_27 | LED_28_SYS | LED_29 },
	.led_mux_custom = 1,
	/* Pad source is logical port * 4 + LED index, 0x3f is unused.
	 * Every port has 3 pads: one single green LED and a two-colour LED
	 * on two pads. The SFP-port red LED (pad 26) has no use yet. */
	.led_mux = { 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, // 65e0
		     0x3f, 0x3f, 0x3f, 0x3f, 0x0e, // 65e4 pad 9: 10G link/act
		     0x0c, 0x0d, 0x12, 0x10, 0x11, // 65e8 pads 10-11: 10G amber/green, 12-14: port 1
		     0x16, 0x14, 0x15, 0x1a, 0x18, // 65ec pads 15-17: port 2, 18-20: port 3
		     0x19, 0x1e, 0x1c, 0x1d, 0x22, // 65f0 pads 21-23: port 4, 24: SFP bottom
		     0x20, 0x21, 0x3f },           // 65f4 pads 25-26: SFP top green/red
	.port_led_set = { 0, 0, 0, 1, 0, 0, 0, 0, 2},
	/* Ports 1-4: top-left amber: 1G, top-left green: 2.5G, top-right green: 10/100M
	 * Port 5: top-left green: 10G/5G, top-left amber: 2.5G (both solid), top-right green: link/act
	 * SFP-port: top green: 10G/5G, bottom green: 2.5G/1G
	 */
	.led_sets = { { LEDS_1G | LEDS_LINK | LEDS_ACT,
			LEDS_2G5 | LEDS_LINK | LEDS_ACT,
			LEDS_100M | LEDS_10M | LEDS_LINK | LEDS_ACT,
			0 },
		      {
			// Speed LEDs stay solid, the link/act LED blinks
			LEDS_2G5 | LEDS_LINK,
			LEDS_10G | LEDS_5G | LEDS_LINK,
			// LINK/ACT only light for the selected speeds, so select all of them
			LEDS_10G | LEDS_5G | LEDS_2G5 | LEDS_1G | LEDS_100M | LEDS_10M | LEDS_LINK | LEDS_ACT,
			0 },
		      {
			LEDS_10G | LEDS_5G | LEDS_LINK | LEDS_ACT,
			0,
			LEDS_2G5 | LEDS_1G | LEDS_LINK | LEDS_ACT,
			0 }
		    },
};
