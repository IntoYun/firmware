#ifndef ARDUINO_H_
#define ARDUINO_H_

#ifdef INTOROBOT_WIRING_ARDUINO_COMPATIBILTY
#undef INTOROBOT_WIRING_ARDUINO_COMPATIBILTY
#endif

#define INTOROBOT_WIRING_ARDUINO_COMPATIBILTY 1

#include "application.h"

#if INTOROBOT_WIRING_ARDUINO_COMPATIBILTY == 1

#ifndef ARDUINO
#define ARDUINO 10800
#endif

#include "math.h"

#ifndef isnan
#error isnan is not defined please ensure this header is included before any STL headers
#endif

#include "avr/pgmspace.h"
#include "wiring_arduino_constants.h"
#include "wiring_arduino_binary.h"

#undef F
#define F(X) (reinterpret_cast<const __FlashStringHelper*>(X))

#ifndef makeWord
inline uint16_t makeWord(uint16_t w) {
  return w;
}

inline uint16_t makeWord(uint8_t h, uint8_t l) {
  return ((uint16_t)h << 8) | ((uint16_t)l);
}
#endif

#ifndef word
#define word(...) makeWord(__VA_ARGS__)
#endif

#ifndef WEAK
#define WEAK __attribute__ ((weak))
#endif

#ifndef SYSTEM_CORE_CLOCK
#define SYSTEM_CORE_CLOCK             SystemCoreClock
#endif

#ifndef clockCyclesPerMicrosecond
#define clockCyclesPerMicrosecond()   (SystemCoreClock / 1000000L)
#endif

#ifndef clockCyclesToMicroseconds
#define clockCyclesToMicroseconds(a)  (((a) * 1000L) / (SystemCoreClock / 1000L))
#endif

#ifndef microsecondsToClockCycles
#define microsecondsToClockCycles(a)  ((a) * (SystemCoreClock / 1000000L))
#endif

#ifndef digitalPinToInterrupt
#define digitalPinToInterrupt(P)      (P)
#endif

inline void yield() {
#if PLATFORM_THREADING
  os_thread_yield();
#endif // PLATFORM_THREADING
}

#ifndef PINS_COUNT
#define PINS_COUNT           TOTAL_PINS
#endif

#ifndef NUM_DIGITAL_PINS
#define NUM_DIGITAL_PINS     TOTAL_PINS
#endif

#ifndef NUM_ANALOG_INPUTS
#define NUM_ANALOG_INPUTS    TOTAL_ANALOG_PINS
#endif

#ifndef NUM_ANALOG_OUTPUTS
#define NUM_ANALOG_OUTPUTS   TOTAL_DAC_PINS
#endif

#ifndef analogInputToDigitalPin
#define analogInputToDigitalPin(p)  (((p < TOTAL_ANALOG_PINS) && (p >= 0)) ? (p) + FIRST_ANALOG_PIN : -1)
#endif

#ifndef _BV
#define _BV(x)  (((uint32_t)1) << (x))
#endif

// XXX
#ifndef sei
#define sei() HAL_enable_irq(0)
#endif

// XXX
#ifndef cli
#define cli() (void)HAL_disable_irq()
#endif

#ifndef cbi
#define cbi(sfr, bit) ((sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) ((sfr) |= _BV(bit))
#endif

// XXX
#ifndef RwReg
typedef volatile uint32_t RwReg;
#endif

// C++ only
#ifdef __cplusplus

#ifndef isnan
#define isnan std::isnan
#endif

#ifndef isinf
#define isinf std::isinf
#endif

// Hardware serial defines

#ifdef FIRMWARE_CONFIG_WIRING_USARTSERIAL
#ifndef UBRRH
#define UBRRH
#endif
#endif

#ifdef FIRMWARE_CONFIG_WIRING_USARTSERIAL1
#ifndef UBRR1H
#define UBRR1H
#endif
#endif

#ifdef FIRMWARE_CONFIG_WIRING_USARTSERIAL2
#ifndef UBRR2H
#define UBRR2H
#endif
#endif

#ifdef FIRMWARE_CONFIG_WIRING_USARTSERIAL3
#ifndef UBRR3H
#define UBRR3H
#endif
#endif

#ifdef FIRMWARE_CONFIG_WIRING_USARTSERIAL4
#ifndef UBRR4H
#define UBRR4H
#endif
#endif

#ifdef FIRMWARE_CONFIG_WIRING_USARTSERIAL5
#ifndef UBRR5H
#define UBRR5H
#endif
#endif

typedef USARTSerial HardwareSerial;

#if !defined(SERIAL_PORT_MONITOR) && defined(FIRMWARE_CONFIG_WIRING_USARTSERIAL)
#define SERIAL_PORT_MONITOR               Serial
#endif

#if !defined(SERIAL_PORT_USBVIRTUAL) && defined(FIRMWARE_CONFIG_WIRING_USBSERIAL)
#define SERIAL_PORT_USBVIRTUAL            SerialUSB
#endif

#if !defined(SERIAL_PORT_HARDWARE_OPEN) && defined(FIRMWARE_CONFIG_WIRING_USARTSERIAL)
#define SERIAL_PORT_HARDWARE_OPEN         Serial
#endif

#if !defined(SERIAL_PORT_HARDWARE_OPEN1) && defined(FIRMWARE_CONFIG_WIRING_USARTSERIAL1)
#define SERIAL_PORT_HARDWARE_OPEN1        Serial1
#endif

#if !defined(SERIAL_PORT_HARDWARE_OPEN2) && defined(FIRMWARE_CONFIG_WIRING_USARTSERIAL2)
#define SERIAL_PORT_HARDWARE_OPEN2        Serial2
#endif

#if !defined(SERIAL_PORT_HARDWARE_OPEN3) && defined(FIRMWARE_CONFIG_WIRING_USARTSERIAL3)
#define SERIAL_PORT_HARDWARE_OPEN3        Serial3
#endif

#if !defined(SERIAL_PORT_HARDWARE_OPEN4) && defined(FIRMWARE_CONFIG_WIRING_USARTSERIAL4)
#define SERIAL_PORT_HARDWARE_OPEN4        Serial4
#endif

#if !defined(SERIAL_PORT_HARDWARE_OPEN5) && defined(FIRMWARE_CONFIG_WIRING_USARTSERIAL5)
#define SERIAL_PORT_HARDWARE_OPEN5        Serial5
#endif

#if !defined(SERIAL_PORT_HARDWARE) && defined(FIRMWARE_CONFIG_WIRING_USARTSERIAL)
#define SERIAL_PORT_HARDWARE              Serial
#endif

#if !defined(SERIAL_PORT_HARDWARE1) && defined(FIRMWARE_CONFIG_WIRING_USARTSERIAL1)
#define SERIAL_PORT_HARDWARE1             Serial1
#endif

#if !defined(SERIAL_PORT_HARDWARE2) && defined(FIRMWARE_CONFIG_WIRING_USARTSERIAL2)
#define SERIAL_PORT_HARDWARE2             Serial2
#endif

#if !defined(SERIAL_PORT_HARDWARE3) && defined(FIRMWARE_CONFIG_WIRING_USARTSERIAL3)
#define SERIAL_PORT_HARDWARE3             Serial3
#endif

#if !defined(SERIAL_PORT_HARDWARE4) && defined(FIRMWARE_CONFIG_WIRING_USARTSERIAL4)
#define SERIAL_PORT_HARDWARE4             Serial4
#endif

#if !defined(SERIAL_PORT_HARDWARE5) && defined(FIRMWARE_CONFIG_WIRING_USARTSERIAL5)
#define SERIAL_PORT_HARDWARE5             Serial5
#endif

#endif // __cplusplus

#endif // INTOROBOT_WIRING_ARDUINO_COMPATIBILTY

#endif	/* ARDUINO_H */
