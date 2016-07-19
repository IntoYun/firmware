/**
 ******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
*/

#ifndef WIRING_USBSERIAL_H_
#define WIRING_USBSERIAL_H_

#include "wiring_stream.h"
#include "wiring_platform.h"
#include "usb_hal.h"
#include "system_task.h"
#include "wiring_startup.h"

class USBSerial : public Stream
{
public:
	// public methods
    USBSerial();
	USBSerial(HAL_USB_USART_Serial serial, const HAL_USB_USART_Config& conf);

    unsigned int baud();

    operator bool();
    bool isEnabled();
    bool isConnected();

	void begin(long speed = 9600);
	void end();
	int peek();

	virtual size_t write(uint8_t byte);
	virtual int read();
	virtual int availableForWrite(void);
	virtual int available();
	virtual void flush();

	virtual void blockOnOverrun(bool);

#if PLATFORM_THREADING
	os_mutex_recursive_t get_mutex()
	{
		return os_mutex_recursive_t(system_internal(2, nullptr));
	}
#endif

	bool try_lock()
	{
#if PLATFORM_THREADING
		return !os_mutex_recursive_trylock(get_mutex());
#else
		return true;
#endif
	}

	void lock()
	{
#if PLATFORM_THREADING
		os_mutex_recursive_lock(get_mutex());
#endif
	}

	void unlock()
	{
#if PLATFORM_THREADING
		os_mutex_recursive_unlock(get_mutex());
#endif
	}

	using Print::write;

private:
    HAL_USB_USART_Serial _serial;
	bool _blocking;
};

HAL_USB_USART_Config acquireSerialBuffer() __attribute__((weak));

extern USBSerial& _fetch_usbserial();
#define SerialUSB _fetch_usbserial()

#endif
