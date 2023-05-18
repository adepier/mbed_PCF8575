/*
 * adaptation of 
 * https://www.mischianti.org/2019/07/22/pcf8575-i2c-16-bit-digital-i-o-expander/
 * for mbed
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Renzo Mischianti www.mischianti.org All right reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "PCF8575.h"

/**
 * Constructor
 * @param address: i2c address
 */
PCF8575::PCF8575(const uint8_t addr,  I2C &i2c)
    : _i2caddr(addr ), _i2c(&i2c) { 
		timer.start();
};


/**
 * Set if fin is OUTPUT or INPUT
 * @param pin: pin to set
 * @param mode: mode, supported only INPUT or OUTPUT (to simplify)
 */
void PCF8575::pinMode(uint8_t pin, uint8_t mode){
	DEBUG_PRINT("Set pin %i as %i ",pin,mode); 

	if (mode == OUTPUT){
		writeMode = writeMode | bit(pin);
		readMode =  readMode & ~bit(pin); 

	}else if (mode == INPUT){
		writeMode = writeMode & ~bit(pin);
		readMode =   readMode | bit(pin); 
	}
	else{
		DEBUG_PRINT("Mode non supported by PCF8575 \n");
	}
	DEBUG_PRINT("Write mode: %i \n",writeMode);

};


#ifndef PCF8575_LOW_MEMORY
	/**
	 * Read value of all INPUT pin
	 * Debounce read more fast than 10millis, non managed for interrupt mode
	 * @return
	 */
	PCF8575::DigitalInput PCF8575::digitalReadAll(void){
		DEBUG_PRINT("force read from buffer \n");
		readBuffer(true); 
#ifdef NOT_SEQUENTIAL_PINOUT
		if ((bit(0) & readMode)>0) digitalInput.p00 = ((byteBuffered & bit(0))>0)?HIGH:LOW;
		if ((bit(1) & readMode)>0) digitalInput.p01 = ((byteBuffered & bit(1))>0)?HIGH:LOW;
		if ((bit(2) & readMode)>0) digitalInput.p02 = ((byteBuffered & bit(2))>0)?HIGH:LOW;
		if ((bit(3) & readMode)>0) digitalInput.p03 = ((byteBuffered & bit(3))>0)?HIGH:LOW;
		if ((bit(4) & readMode)>0) digitalInput.p04 = ((byteBuffered & bit(4))>0)?HIGH:LOW;
		if ((bit(5) & readMode)>0) digitalInput.p05 = ((byteBuffered & bit(5))>0)?HIGH:LOW;
		if ((bit(6) & readMode)>0) digitalInput.p06 = ((byteBuffered & bit(6))>0)?HIGH:LOW;
		if ((bit(7) & readMode)>0) digitalInput.p07 = ((byteBuffered & bit(7))>0)?HIGH:LOW;
		if ((bit(8) & readMode)>0) digitalInput.p10 = ((byteBuffered & bit(8))>0)?HIGH:LOW;
		if ((bit(9) & readMode)>0) digitalInput.p11 = ((byteBuffered & bit(9))>0)?HIGH:LOW;
		if ((bit(10) & readMode)>0) digitalInput.p12 = ((byteBuffered & bit(10))>0)?HIGH:LOW;
		if ((bit(11) & readMode)>0) digitalInput.p13 = ((byteBuffered & bit(11))>0)?HIGH:LOW;
		if ((bit(12) & readMode)>0) digitalInput.p14 = ((byteBuffered & bit(12))>0)?HIGH:LOW;
		if ((bit(13) & readMode)>0) digitalInput.p15 = ((byteBuffered & bit(13))>0)?HIGH:LOW;
		if ((bit(14) & readMode)>0) digitalInput.p16 = ((byteBuffered & bit(14))>0)?HIGH:LOW;
		if ((bit(15) & readMode)>0) digitalInput.p17 = ((byteBuffered & bit(15))>0)?HIGH:LOW;
#else
		if ((bit(0) & readMode)>0) digitalInput.p0 = ((byteBuffered & bit(0))>0)?HIGH:LOW;
		if ((bit(1) & readMode)>0) digitalInput.p1 = ((byteBuffered & bit(1))>0)?HIGH:LOW;
		if ((bit(2) & readMode)>0) digitalInput.p2 = ((byteBuffered & bit(2))>0)?HIGH:LOW;
		if ((bit(3) & readMode)>0) digitalInput.p3 = ((byteBuffered & bit(3))>0)?HIGH:LOW;
		if ((bit(4) & readMode)>0) digitalInput.p4 = ((byteBuffered & bit(4))>0)?HIGH:LOW;
		if ((bit(5) & readMode)>0) digitalInput.p5 = ((byteBuffered & bit(5))>0)?HIGH:LOW;
		if ((bit(6) & readMode)>0) digitalInput.p6 = ((byteBuffered & bit(6))>0)?HIGH:LOW;
		if ((bit(7) & readMode)>0) digitalInput.p7 = ((byteBuffered & bit(7))>0)?HIGH:LOW;
		if ((bit(8) & readMode)>0) digitalInput.p8 = ((byteBuffered & bit(8))>0)?HIGH:LOW;
		if ((bit(9) & readMode)>0) digitalInput.p9 = ((byteBuffered & bit(9))>0)?HIGH:LOW;
		if ((bit(10) & readMode)>0) digitalInput.p10 = ((byteBuffered & bit(10))>0)?HIGH:LOW;
		if ((bit(11) & readMode)>0) digitalInput.p11 = ((byteBuffered & bit(11))>0)?HIGH:LOW;
		if ((bit(12) & readMode)>0) digitalInput.p12 = ((byteBuffered & bit(12))>0)?HIGH:LOW;
		if ((bit(13) & readMode)>0) digitalInput.p13 = ((byteBuffered & bit(13))>0)?HIGH:LOW;
		if ((bit(14) & readMode)>0) digitalInput.p14 = ((byteBuffered & bit(14))>0)?HIGH:LOW;
		if ((bit(15) & readMode)>0) digitalInput.p15 = ((byteBuffered & bit(15))>0)?HIGH:LOW;
#endif
		if ((readMode & byteBuffered)>0){
			byteBuffered = ~readMode & byteBuffered;
			DEBUG_PRINT("Buffer hight value readed set readed %i\n",byteBuffered);
		}
		DEBUG_PRINT("Return value/n");
		return digitalInput;
	};
#else
	/**
	 * Read value of all INPUT pin in byte format for low memory usage
	 * Debounce read more fast than 10millis, non managed for interrupt mode
	 * @return
	 */
	uint16_t PCF8575::digitalReadAll(void){
		DEBUG_PRINT("force read from buffer \n");
		readBuffer(true); 

		uint16_t byteRead = byteBuffered;

		if ((readMode & byteBuffered)>0){
			byteBuffered = ~readMode & byteBuffered;
			DEBUG_PRINT("Buffer set to low to read buffer only one time. byteBuffered:%i \n",byteBuffered );
		}
		DEBUG_PRINT("Return value %i\n",byteRead);
		return byteRead;
	};
#endif

/**
 * Read value of specified pin
 * Debounce read more fast than 10millis, non managed for interrupt mode
 * @param pin
 * @return
 */
uint8_t PCF8575::digitalRead(uint8_t pin){
	uint8_t value = LOW;
	if ((bit(pin) & writeMode)>0){
		DEBUG_PRINT("Pin in write mode, return value \n");
		DEBUG_PRINT("Write data: %i for pin: %i  value: %i",writeByteBuffered, pin,value);
		if ((bit(pin) & writeByteBuffered)>0){
			  value = HIGH;
		  }else{
			  value = LOW;
		  }
		return value;
	}

	DEBUG_PRINT("Read pin %i" ,pin);
	// Check if pin already HIGH than read and prevent reread of i2c
	if ((bit(pin) & byteBuffered)>0){
				DEBUG_PRINT("Pin already up \n");
				value = HIGH;
	}
	else {
		DEBUG_PRINT("read from buffer \n");
		readBuffer(false); 
		if ((bit(pin) & byteBuffered)>0){
			value = HIGH;
		}
	}
	DEBUG_PRINT("Buffer value %i",byteBuffered );
	// If HIGH set to low to read buffer only one time
	if (value==HIGH){
		byteBuffered = ~bit(pin) & byteBuffered;
		DEBUG_PRINT("Buffer set to low to read buffer only one time. byteBuffered:%i \n",byteBuffered );
	}
	DEBUG_PRINT("Return value %i \n",value);
	return value;
};

/**
 * Read value from i2c and bufferize it
 * @param force
 */
void PCF8575::readBuffer(bool force){
	if (std::chrono::duration_cast<std::chrono::milliseconds>(timer.elapsed_time()).count() > READ_ELAPSED_TIME || force){
			/*specific function for mbed*/
			char data[2];
			if(_i2c->read(_i2caddr<<1 | 0x01 , data, 2,true)){printf("I2C ERR: no ack on read\n");}
			uint16_t iInput =  (data[1] << 8) | data[0];// Read a uint16_t
			DEBUG_PRINT("Input %i \n",iInput ); 
			if ((iInput & readMode)>0){
				byteBuffered = byteBuffered | (uint16_t)iInput; 
			}
		 	DEBUG_PRINT("byteBuffered %i \n",byteBuffered );
	}
}

/**
 * Write on pin
 * @param pin
 * @param value
 */
void PCF8575::digitalWrite(uint8_t pin, uint8_t value){
	 
	if (value==HIGH){
		writeByteBuffered = writeByteBuffered | bit(pin);
	}else{
		writeByteBuffered = writeByteBuffered & ~bit(pin);
	}
	writeByteBuffered = writeByteBuffered & writeMode;

	/*specific function for mbed*/
	char data[2];
    data[0]=writeByteBuffered;
    data[1]=writeByteBuffered>>8;
    if(_i2c->write(_i2caddr<<1 | 0x00 , data, 2,true)) {  printf("I2C ERR: No ACK on i2c write!"); }

};
