#ifndef	_So595_h
#define	_So595_h
/*
 * Serial Out 74XX595 class.
 * Sends data out through a 74XX595 serial to parallel converter chain.
 *
 * We can daisy chain multiple chips together.
 * The data is sent out big endian (first byte ends up at the furthest '595.
 *
 * This code can handle chains of 65535 '595 chips.
 *
 * Written 1.0 2012/3/3 Rex di Bona
 * Version 1.1 2015/6/12 Rex di Bona
 *
 */

#include <inttypes.h>
#include <Arduino.h>

class So595 {
private:
	uint16_t _len;	// length of data stream (in bytes)
	uint8_t	*_data;	// pointer to data array
	uint8_t	_ds;	// Serial data pin number
	uint8_t	_sclk;	// Shift register clock pin number
	uint8_t	_hclk;	// Hold (output) register clock
	uint8_t	_sclr;	// Shift register clear pin number
	uint8_t	_oe;	// Output enable pin number 
public:
	So595(uint16_t len, uint8_t *data, uint8_t ds, uint8_t sclk, uint8_t hclk, uint8_t sclr, uint8_t oe);
	So595(uint16_t len, uint8_t *data, uint8_t ds, uint8_t sclk, uint8_t hclk, uint8_t sclr);
	So595(uint16_t len, uint8_t *data, uint8_t ds, uint8_t sclk, uint8_t hclk);
	void	send();		// push the data string out to the '595 chips
	int8_t	set(uint8_t bit, uint8_t value);	// set the specified bit to (value & 1) - (return -1 on error)
	int8_t	get(uint8_t bit);			// get the specified bit (return -1 on error)
	void	clear();	// set all outputs to zero  (Uses MR if defined)
	int8_t	enable();	// turn on outputs (if oe defined) - Returns -1 if oe not defined
	int8_t	disable();	// Change outputs to high impedance (if oe defined) - Returns -1 if oe not defined
};

#endif	/* So595.h */
