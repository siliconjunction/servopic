# PIC Firmware

## Overview

The firmware is designed to run on a PIC 12F1840. It is run on the internal oscillator at 32 MHz. The I/O pins used are as below. The parser.h/parser.c files contain the state machine to parse the AT commands. These files are automatically generated by a java program. The at_finctions.h/at_functions.c handle all calls from the parser and translate them into calls to the main routines in interrupt.c.

<table border="1" cellspacing="0" cellpadding="2">
	<tr><th>Pin No.</th><th>Function</th><th>Description</th><th>ICSP</th></tr>
	<tr><td>1</td><td>Vdd</td><td>+5V</td><td>2</td></tr>
	<tr><td>2</td><td>RA5</td><td>Servo</td><td></td></tr>
	<tr><td>3</td><td>RA4</td><td>LED</td><td></td></tr>
	<tr><td>4</td><td>RA3/MCLR</td><td>Prog Inp</td><td>1</td></tr>
	<tr><td>5</td><td>RA2</td><td>Not used</td><td></td></tr>
	<tr><td>6</td><td>RA1/Rx/ICSPCLK</td><td>Rx</td><td>5</td></tr>
	<tr><td>7</td><td>RA0/Tx/ICSPDAT</td><td>Tx</td><td>4</td></tr>
	<tr><td>8</td><td>Vss</td><td>Gnd</td><td>3</td></tr>
</table>