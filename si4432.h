/*
 * SI4432 library for Arduino - v0.1
 *
 * Please note that Library uses standart SS pin for NSEL pin on the chip. This is 53 for Mega, 10 for Uno.
 * NOTES:
 *
 * V0.1
 * * Library supports no `custom' changes and usages of GPIO pin. Modify/add/remove your changes if necessary
 * * Radio use variable packet field format with 4 byte address header, first data field as length. Change if necessary
 *
 * made by Ahmet (theGanymedes) Ipkin
 *
 * 2014
 */

#ifndef si4432_H_
#define si4432_H_
#include "Arduino.h"

/* Now, according to the this design, you must
 * 1- Create an instance
 * 2- Call init()
 *
 * According to the data sheet, you can change any register value and most will get to work after going into IDLE state and back (to RX/TX)
 * (some are hot - changes right away) I didn't test this - but why not? :)
 * */

class Si4432 {
public:

	Si4432(uint8_t sdnPin, uint8_t InterruptPin = 0); // when a InterruptPin is given, interrupts are checked with this pin - rather than SPI polling

	void setFrequency(unsigned long baseFrequency); // sets the freq. call before boot
	void setChannel(byte channel); // sets the channel. call before switching to tx or rx mode
	void setBaudRate(uint64_t bps); // sets the  bps. call before switching to tx or rx mode
void init();
	inline void setCommsSignature(uint16_t signature); // used to 'sign' packets with a predetermined signature - call before boot

	bool sendPacket(uint8_t length, const byte* data, uint64_t ackTimeout, bool waitResponse = false,
			uint8_t *responseLength = 0, byte* responseBuffer = 0); // switches to Tx mode and sends the package, then optionally receives response package

	bool waitForPacket(uint64_t waitMs); // switch to Rx mode
	void getPacketReceived(uint8_t* length, byte* readData); // wait for the 'valid' package to arrive and read from Rx FIFO - returns false if no package or erroneous package received

	void readAll();

	void clearTxFIFO();
	void clearRxFIFO();

	void clearFIFO();

	void softReset();

	void hardReset();

protected:
	enum AntennaMode {
		RXMode = 0x04, TXMode = 0x08, Idle = 0x01, TuneMode = 0x02
	};

	uint8_t _sdnPin, _intPin;

	uint64_t _freqCarrier;
	uint8_t _freqChannel;
	int64_t _bps;
	uint16_t _packageSign;

	void boot(); // sets SPI and pins ready and boot the radio

	void switchMode(int mode);

	enum Registers {
		REG_DEV_TYPE = 0x00,
		REG_DEV_VERSION = 0x01,
		REG_DEV_STATUS = 0x02,

		REG_INT_STATUS1 = 0x03,
		REG_INT_STATUS2 = 0x04,
		REG_INT_ENABLE1 = 0x05,
		REG_INT_ENABLE2 = 0x06,
		REG_STATE = 0x07,
		REG_OPERATION_CONTROL = 0x08,

		REG_GPIO0_CONF = 0x0B,
		REG_GPIO1_CONF = 0x0C,
		REG_GPIO2_CONF = 0x0D,
		REG_IOPORT_CONF = 0x0E,

		REG_DATAACCESS_CONTROL = 0x30,
		REG_EZMAC_STATUS = 0x31,
		REG_HEADER_CONTROL1 = 0x32,
		REG_HEADER_CONTROL2 = 0x33,
		REG_PREAMBLE_LENGTH = 0x34,
		REG_PREAMBLE_DETECTION = 0x35,
		REG_SYNC_WORD3 = 0x36,
		REG_SYNC_WORD2 = 0x37,
		REG_SYNC_WORD1 = 0x38,
		REG_SYNC_WORD0 = 0x39,
		REG_TRANSMIT_HEADER3 = 0x3A,
		REG_TRANSMIT_HEADER2 = 0x3B,
		REG_TRANSMIT_HEADER1 = 0x3C,
		REG_TRANSMIT_HEADER0 = 0x3D,

		REG_PKG_LEN = 0x3E,

		REG_CHECK_HEADER3 = 0x3F,
		REG_CHECK_HEADER2 = 0x40,
		REG_CHECK_HEADER1 = 0x41,
		REG_CHECK_HEADER0 = 0x42,

		REG_RECEIVED_HEADER3 = 0x47,
		REG_RECEIVED_HEADER2 = 0x48,
		REG_RECEIVED_HEADER1 = 0x49,
		REG_RECEIVED_HEADER0 = 0x4A,

		REG_RECEIVED_LENGTH = 0x4B,
		REG_TX_POWER = 0x6D,
		REG_TX_DATARATE1 = 0x6E,
		REG_TX_DATARATE0 = 0x6F,
		REG_MODULATION_MODE1 = 0x70,
		REG_MODULATION_MODE2 = 0x71,

		REG_FREQ_DEVIATION = 0x72,
		REG_FREQ_OFFSET1 = 0x73,
		REG_FREQ_OFFSET2 = 0x74,
		REG_FREQBAND = 0x75,
		REG_FREQCARRIER_H = 0x76,
		REG_FREQCARRIER_L = 0x77,

		REG_FREQCHANNEL = 0x79,
		REG_CHANNEL_STEPSIZE = 0x7A,

		REG_FIFO = 0x7F,

	};

	void ChangeRegister(Registers reg, byte value);
	byte ReadRegister(Registers reg);

	void BurstWrite(Registers startReg, const byte value[], uint8_t length);
	void BurstRead(Registers startReg, byte value[], uint8_t length);

};

#endif /* si4432_H_ */
