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

#ifndef __SPARK_WIRING_USBSERIAL_H
#define __SPARK_WIRING_USBSERIAL_H

#include "wiring_stream.h"
#include "wiring_platform.h"
#include "usb_hal.h"
#include "system_task.h"
#include "wiring_startup.h"
#include "concurrent_hal.h"

class USBSerial : public Stream
{
public:
	// public methods
	USBSerial();
    virtual ~USBSerial() {};

    unsigned int baud() { return USB_USART_Baud_Rate(); }

    operator bool() { return baud()!=0; }

	void begin(long speed);
	void end();
	int peek();

	virtual size_t write(uint8_t byte);
	virtual int read();
	virtual int availableForWrite(void);
	virtual int available();
	virtual void flush();

	virtual void blockOnOverrun(bool);

#if 0
//#if PLATFORM_THREADING
	os_mutex_recursive_t get_mutex()
	{
		return os_mutex_recursive_t(system_internal(2, nullptr));
	}
#endif

	bool try_lock()
	{
#if 0
//#if PLATFORM_THREADING
		return !os_mutex_recursive_trylock(get_mutex());
#else
		return true;
#endif
	}

	void lock()
	{
#if 0
//#if PLATFORM_THREADING
		os_mutex_recursive_lock(get_mutex());
#endif
	}

	void unlock()
	{
#if 0
//#if PLATFORM_THREADING
		os_mutex_recursive_unlock(get_mutex());
#endif
	}

	using Print::write;

private:
	bool _blocking;
};

extern USBSerial& _fetch_global_serial();

#define Serial _fetch_global_serial()
#define SerialUSB _fetch_global_serial()

#endif
