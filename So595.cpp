/*
 * Serial Out 74XX595 class.
 * Sends data out through a 74XX595 serial to parallel converter chain.
 *
 * We can daisy chain multiple chips together.
 * The data is sent out big endian (first byte ends up at the furthest '595.
 *
 * This code can handle chains of 8192 '595 chips.
 *
 * Written 1.0 2012/3/3 Rex di Bona
 * Version 1.1 2015/6/12 Rex di Bona
 *
 */
#include "So595.h"

So595::So595(uint16_t len, uint8_t *data, uint8_t ds, uint8_t sclk, uint8_t hclk, uint8_t sclr, uint8_t oe)
{
	_len = len;
	_ds = ds;
	_sclk = sclk;
	_hclk = hclk;
	_sclr = sclr;
	_oe = oe;
	
	if (_oe != 255) {
		pinMode(_oe, OUTPUT);
		digitalWrite(_oe, HIGH);
	}
	if (_sclr != 255) {
		pinMode(_sclr, OUTPUT);
		digitalWrite(_sclr, HIGH);
	}
	pinMode(_ds, OUTPUT);
	pinMode(_sclk, OUTPUT);
	pinMode(_hclk, OUTPUT);
	digitalWrite(_sclk, LOW);	// default is LOW for clocks
	digitalWrite(_hclk, LOW);	// default is LOW for clocks
}

So595::So595(uint16_t len, uint8_t *data, uint8_t ds, uint8_t sclk, uint8_t hclk, uint8_t sclr)
{
	So595(len, data, ds, sclk, hclk, sclr, 255);
}


So595::So595(uint16_t len, uint8_t *data, uint8_t ds, uint8_t sclk, uint8_t hclk)
{
	So595(len, data, ds, sclk, hclk, 255, 255);
}

/*
 * Clear the data.
 * We clear the backing array first.
 * If the clear strobe is defined we then use that to clear the chain of '595 chips,
 * Otherwise we send the data to the chips suing a normal send() function.
 */
void So595::clear()
{
	uint8_t	*dp;

	dp = _data;	// firstly clear the data array
	for (uint16_t i = 0; i < _len; i++) {
		*dp++ = 0;
	}
	if (_sclr != 255) {
		// clear pin defined - use it as it's faster.
		digitalWrite(_sclr, LOW);	// pulse it LOW
		digitalWrite(_sclr, HIGH);
		digitalWrite(_hclk, HIGH);	// and transfer to holding register */
		digitalWrite(_hclk, LOW);
	} else {
		send();		// and send it to the '595 chips
	}
}

int8_t So595::enable()
{
	if (_oe == 255) {
		return -1;
	}
	digitalWrite(_oe, LOW);
	return 0;
}

int8_t So595::disable()
{
	if (_oe == 255) {
		return -1;
	}
	digitalWrite(_oe, HIGH);
	return 0;
}

void So595::send()
{
	uint8_t	*dp;

	dp = _data;
	for (uint16_t i = 0; i < _len; i++) {
		uint8_t	v = *dp++;
		uint8_t mask = 0x80;
		for (uint8_t b = 0; b < 8; b++) {
			digitalWrite(_ds, v & mask);
			mask >>= 1;
		}
	}
	digitalWrite(_hclk, HIGH);	// and transfer to holding register
	digitalWrite(_hclk, LOW);
}

int8_t So595::set(uint16_t bit, uint8_t value)
{
	uint16_t t1;
	uint8_t	 t2, t3;

	t1 = bit >> 3;
	if (t1 >= _len) {
		return -1;
	}
	t2 = bit & 7;
	t3 = _data[t1];
	_data[t1] = (t3 & ~(1 << t1)) | ((value & 1) << t1);
	return 0;
}

int8_t So595::get(uint16_t bit)
{
	uint16_t t1;
	uint8_t	 t2;

	t1 = bit >> 3;
	if (t1 >= _len) {
		return -1;
	}
	t2 = bit & 7;
	return ((_data[t1] >> t2) & 1);
}
