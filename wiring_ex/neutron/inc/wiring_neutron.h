/**
 ******************************************************************************
 * @file     : neutron_sensors.h
 * @author   : robin
 * @version  : V1.0.0
 * @date     : 6-December-2014
 * @brief    :
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
#ifndef   NEUTRON_SENSOR_H_
#define   NEUTRON_SENSOR_H_

#include "math.h"

extern "C"
{
#include "bmi160.h"
#include "bmi160_support.h"
}
#include "BMP280.h"
#include "lib_mic_hal.h"

#include "wiring_i2c.h"
#include "wiring.h"
#include "wiring_usartserial.h"
#include "wiring_usbserial.h"

extern struct bmi160_t s_bmi160;
#define I2C_BUFFER_LEN 256
#define C_BMI160_THIRTY_U8X		(30)
#define BMI160_I2C_FREQUENCY (100*1000)
#define	BMI160_MAG_INTERFACE_OFF_PRIMARY_ON		(0x00)
#define	BMI160_MAG_INTERFACE_ON_PRIMARY_ON		(0x02)
signed char BMI160_I2C_bus_read(unsigned char device_addr, unsigned char reg_addr,unsigned char *reg_data, unsigned char cnt);
signed char BMI160_I2C_bus_write(unsigned char device_addr, unsigned char reg_addr,unsigned char *reg_data, unsigned char cnt);
void BMI160_delay_msec(BMI160_MDELAY_DATA_TYPE msec); //delay in milliseconds


// BMI160 accelerometer and gyroscope
class BMI160Sensor
{
    private:
        bool isEnabledFlag;

    public:
        BMI160Sensor();
        bool isEnabled(void);
        void setRange(uint8_t accel_range, uint8_t gyro_range);
        void begin(void);
        void getAccelData(int16_t accel[3]);
        void getGyroData(int16_t gyro[3]);
        void getMagData(int16_t mag[3]);// BMM150 magnetormeter
        void getAccelGyroData(int16_t accel[3], int16_t gyro[3]);
        void getAccelGyroMagData(int16_t accel[3], int16_t gyro[3], int16_t mag[3]);
};



char BMP280_I2C_bus_read(unsigned char device_addr, unsigned char reg_addr,unsigned char *reg_data, unsigned char cnt);
char BMP280_I2C_bus_write(unsigned char device_addr, unsigned char reg_addr,unsigned char *reg_data, unsigned char cnt);
void BMP280_delay_msec(BMP280_U16_t msec); //delay in milliseconds

// BMP280 barometer
class BMP280Sensor
{
    private:
        struct bmp280_t bmp280;
        bool isEnabledFlag;

    public:
        BMP280Sensor();
        bool isEnabled(void);
        void begin(void);
        void getTemperaturePressureData(double& temperature, double& pressure);
        void getAltitudeData(double& alt);
};

// ALS-PT19-315
class LightSensor
{
    private:
        bool isEnabledFlag;

    public:
        LightSensor();
        bool isEnabled(void);
        void begin(void);
        void getLightData(uint16_t& lightData);
};

// MIC AKU242
class MICSensor
{
    private:
        mic_mems_t mic_mems;
        bool isEnabledFlag;

    public:
        MICSensor();
        bool isEnabled(void);
        void begin(void);
        void getMICData(char& micData);
        void getMICDataToUSBSerial(USBSerial& serialusb);
        void getMICDataToSerial(USARTSerial& serial);
};


class NeutronSensors
{
    private:
        bool isEnabledFlag;


    public:
        NeutronSensors();
        bool isEnabled(void);
        void begin(void);

        // BMI160 and BMM150 sensor

        void setBMI160Range(uint8_t accel_range, uint8_t gyro_range);
        void getAccelData(int16_t accel[3]);
        void getGyroData(int16_t gyro[3]);
        void getMagData(int16_t mag[3]);
        void getAccelGyroData(int16_t accel[3], int16_t gyro[3]);
        void getAccelGyroMagData(int16_t accel[3], int16_t gyro[3], int16_t mag[3]);

        // BMP280 sensor
        void getPressureAltitudeData(double& pressure, double& altitude);

        // Light sensor
        void getLightData(uint16_t& lightData);

        // Mic sensor
        void getMICData(char& micData);
        void getMICDataToUSBSerial(USBSerial& serialusb);
        void getMICDataToSerial(USARTSerial& serial);

        //void getAllDataExceptMIC(int16_t accel[3], int16_t gyro[3], int16_t mag[3], double& pressure, double& alt, uint16_t& lightData);
        void getAllDataExceptMic(int16_t accel[3], int16_t gyro[3], int16_t mag[3], double& temperature, double& pressure, double& alt, uint16_t& lightData);

};
extern BMI160Sensor BMI160;
extern BMP280Sensor BMP280;
extern LightSensor  lightSensor;
extern MICSensor    micSensor;
extern NeutronSensors Neutron;
#endif /*NEUTRON_SENSOR_H_*/
