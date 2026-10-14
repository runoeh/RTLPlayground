# Hasivo S600P-4G-1XTG-1S+

Sold as Mokerlink 2G04210GSMX. PCB silkscreen: `Hasivo S600P-4G-1XTG-1S+ V1.02`.

Build with `make MACHINE=HASIVO_S600P_4G_1XTG_1S`.

CPU: RTL8372
Flash: 2 MByte SPI NOR (SOIC-8)
PHY: RTL8261 (10G copper) at MDIO address 3 on SDS0

### Ports

- 4 × RJ45: 10/100/1000/2500 Mbps (internal PHYs)
- 1 × RJ45: 10M to 10G (RTL8261), logical port 3
- 1 × SFP+: 1G, 2.5G and 10G, on SDS1, logical port 8

The port layout is the same as the [ZX310S-4T2XH](ZX310S-4T2XH.md), and the
machine entry is based on it. Two things differ.

### 10G copper port: SDS0 RX polarity

With the ZX310S-4T2XH settings, the 10G RJ45 port links at the right speed and
transmits, but never receives a frame. SDS0 page 6 register 2 needs both bits
`0x2000` and `0x4000` set. Either bit alone has no effect. `machine_custom_init()`
sets them. `sds_init()` and `sds_config()` do not change them later.

This was found by putting the RTL8261 in 10G PCS loopback
(`physet 3 3 0 6040`, restore with `physet 3 3 0 2040`), flooding broadcasts
into another port and watching RxGood on the 10G port with `stat`.

### LEDs

Each port has a single green LED and a two-colour LED, driven by 3 LED pads.
A two-colour LED sits between two pads, so it only lights when one of its pads
is active. Pads 0-8 and 27 are not connected.

| Pad | LED |
| --- | --- |
| 9 | 10G RJ45 top right, green: link/act |
| 10, 11 | 10G RJ45 top left, amber / green |
| 12, 13, 14 | Port 1: top right green / top left amber / top left green |
| 15, 16, 17 | Port 2 (same order) |
| 18, 19, 20 | Port 3 (same order) |
| 21, 22, 23 | Port 4 (same order) |
| 24 | SFP+ bottom, green |
| 25, 26 | SFP+ top, green / red |
| 28 | Power LED (system LED), inverted polarity |

The configured behaviour:

| Port | LED | Shows |
| --- | --- | --- |
| Ports 1-4 | Top left amber | 1G, blinks on activity |
| Ports 1-4 | Top left green | 2.5G, blinks on activity |
| Ports 1-4 | Top right green | 100M/10M, blinks on activity |
| 10G RJ45 | Top left green | 10G/5G, solid |
| 10G RJ45 | Top left amber | 2.5G, solid |
| 10G RJ45 | Top right green | Link at any speed, blinks on activity |
| SFP+ | Top green | 10G/5G, blinks on activity |
| SFP+ | Bottom green | 2.5G/1G, blinks on activity |

The SFP+ red LED is not used.

An LED set entry with only `LEDS_LINK | LEDS_ACT` never lights. It needs at
least one speed bit, so the 10G link/act LED selects all speeds.

The power LED is the system LED on pad 28, but wired with inverted polarity.
`machine_custom_init()` clears bit 28 of `RTL837X_REG_LED_GLB_ACTIVE` (0x65d8).

To map LEDs on a new board without reflashing: write 0 to the LED set registers
0x6524-0x6548 so that all LEDs are off, then invert one bit at a time in
0x65d8 and note which LED lights.

### Serial console

No UART header was found. The `/cmd` endpoint of the web interface runs the
same commands as the serial console, including `regget`, `sdsget` and
`phyget`.
