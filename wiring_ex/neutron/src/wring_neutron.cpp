/**
 ******************************************************************************
 * @file     : neutron_sensors.cpp
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
  *******************************************************************************/
#include "wiring_neutron.h"

extern struct bmi160_t s_bmi160;
#define I2C_BUFFER_LEN 256
#define C_BMI160_THIRTY_U8X		(30)
#define BMI160_I2C_FREQUENCY (100*1000)
#define	BMI160_MAG_INTERFACE_OFF_PRIMARY_ON		(0x00)
#define	BMI160_MAG_INTERFACE_ON_PRIMARY_ON		(0x02)

TwoWire Wire1(HAL_I2C_INTERFACE2);

signed char BMI160_I2C_bus_read(unsigned char device_addr, unsigned char reg_addr,unsigned char *reg_data, unsigned char cnt)
{
    Wire1.beginTransmission(device_addr);
    Wire1.write(reg_addr);
    Wire1.endTransmission(0);
    Wire1.beginTransmission(device_addr);
    Wire1.requestFrom(device_addr, cnt);
    if(!Wire1.available())
    {
        return -1;
    }
    for(uint8_t i = 0; i < cnt; i++)
    {
        *(reg_data + i) = Wire1.read();
    }
    return 0;
}


signed char BMI160_I2C_bus_write(unsigned char device_addr, unsigned char reg_addr,unsigned char *reg_data, unsigned char cnt)
{
    int iError = 0;
    Wire1.beginTransmission(device_addr);
    Wire1.write(reg_addr);
    for(uint8_t i = 0; i < cnt; i++)
    {
        Wire1.write(*(reg_data + i));
    }
    iError = Wire1.endTransmission();
    return iError;
}

void BMI160_delay_msec(BMI160_MDELAY_DATA_TYPE msec) //delay in milliseconds
{
    delay(msec);

}

// BMI160 accelerometer and gyroscope
BMI160Sensor::BMI160Sensor()
{
    isEnabledFlag = false;
}

bool BMI160Sensor::isEnabled(void)
{
    return isEnabledFlag;
}

void BMI160Sensor::setRange(uint8_t accel_range, uint8_t gyro_range)
{
    bmi160_set_range(accel_range, gyro_range);
}


void BMI160Sensor::begin(void)
{
    if(isEnabledFlag)
        return;
    Wire1.begin();
    s_bmi160.bus_read = BMI160_I2C_bus_read;
    s_bmi160.bus_write = BMI160_I2C_bus_write;
    s_bmi160.delay_msec = BMI160_delay_msec;
    s_bmi160.dev_addr = BMI160_I2C_ADDR1;

    bmi160_init(&s_bmi160);
    bmi160_config_running_mode(STANDARD_UI_9DOF_FIFO);  // the accelerometer range is 16G, the gyroscope is 2000
    isEnabledFlag = true;
}

void BMI160Sensor::getAccelData(int16_t accel[3])
{
    struct bmi160_accel_t accelData;
    bmi160_read_accel_xyz(&accelData);
    // change to the board coordinate
    accel[0] =  accelData.x;
    accel[1] = -accelData.y;
    accel[2] = -accelData.z;
}

void BMI160Sensor::getGyroData(int16_t gyro[3])
{
    struct bmi160_gyro_t gyroData;
    bmi160_read_gyro_xyz(&gyroData);
    // change to the board coordinate
    gyro[0] =  gyroData.x;
    gyro[1] = -gyroData.y;
    gyro[2] = -gyroData.z;
}

void BMI160Sensor::getMagData(int16_t mag[3])
{
    struct bmi160_mag_t magData;
    bmi160_read_mag_xyz(&magData, 0); // 0 is BMM150
    // change to the board coordinate
    mag[0] =  magData.y;
    mag[1] = -magData.x;
    mag[2] =  magData.z;
}

void BMI160Sensor::getAccelGyroData(int16_t accel[3], int16_t gyro[3] )
{
    getAccelData(accel);
    getGyroData(gyro);
}

void BMI160Sensor::getAccelGyroMagData(int16_t accel[3], int16_t gyro[3], int16_t mag[3])
{
    getAccelData(accel);
    getGyroData(gyro);
    getMagData(mag);
}


#define I2C_ADDR_BMP280	0x76
#define I2C_CHANNEL_BMP280	1
#define I2C_FREC_BMP280	100*1000
#define I2C_BUFFER_LEN 256


char BMP280_I2C_bus_read(unsigned char device_addr, unsigned char reg_addr,unsigned char *reg_data, unsigned char cnt)
{
    Wire1.beginTransmission(device_addr);
    Wire1.write(reg_addr);
    Wire1.endTransmission(0);
    Wire1.beginTransmission(device_addr);
    Wire1.requestFrom(device_addr, cnt);
    if(!Wire1.available())
    {
        return -1;
    }
    for(uint8_t i = 0; i < cnt; i++)
    {
        *(reg_data + i) = Wire1.read();
    }
    return 0;
}


char BMP280_I2C_bus_write(unsigned char device_addr, unsigned char reg_addr,unsigned char *reg_data, unsigned char cnt)
{
    int iError = 0;
    Wire1.beginTransmission(device_addr);
    Wire1.write(reg_addr);
    for(uint8_t i = 0; i < cnt; i++)
    {
        Wire1.write(*(reg_data + i));
    }
    iError = Wire1.endTransmission();
    return iError;
}



void BMP280_delay_msec(BMP280_U16_t msec) //delay in milliseconds
{
    delay(msec);
}

// BMP280 barometer
BMP280Sensor::BMP280Sensor()
{
    isEnabledFlag = false;
}

bool BMP280Sensor::isEnabled(void)
{
    return isEnabledFlag;
}
void BMP280Sensor::begin(void)
{
    if (isEnabledFlag)
        return;
    Wire1.begin();
    bmp280.bus_read = BMP280_I2C_bus_read;
    bmp280.bus_write = BMP280_I2C_bus_write;
    bmp280.delay_msec = BMP280_delay_msec;

    bmp280_init(&bmp280);
    bmp280_set_osrs_t(BMP280_OVERSAMPLING_1X);
    bmp280_set_osrs_p(BMP280_OVERSAMPLING_8X);
    isEnabledFlag = true;
}

void BMP280Sensor::getTemperaturePressureData(double& temperature, double& pressure)
{
    BMP280_S32_t upressure, utemperature;
    bmp280_get_forced_uput(&upressure, &utemperature);

    double compensated_temperature_d, compensated_pressure_d;
    compensated_temperature_d = bmp280_compensate_T_double(utemperature);   //in DegC
    compensated_pressure_d = bmp280_compensate_P_double(upressure);     //in Pa

    temperature  = compensated_temperature_d;
    pressure = compensated_pressure_d;

}

void BMP280Sensor::getAltitudeData(double& alt)
{
    double temp_t,temp_p;
    getTemperaturePressureData(temp_t, temp_p);
    alt = (44330.0*(1-pow(temp_p/101325,1/5.255)));
}

// ALS-PT19-315
LightSensor::LightSensor()
{
    isEnabledFlag = false;
}
bool LightSensor::isEnabled(void)
{
    return isEnabledFlag;
}

void LightSensor::begin(void)
{
    if (isEnabledFlag)
        return;
    pinMode(LIGHT_SENSOR_UC, AN_INPUT);
    isEnabledFlag = true;
}

void LightSensor::getLightData(uint16_t& lightData)
{
    lightData = analogRead(LIGHT_SENSOR_UC);
}


// MIC AKU242
MICSensor::MICSensor()
{
    isEnabledFlag = false;
}

bool MICSensor::isEnabled(void)
{
    return isEnabledFlag;
}

void MICSensor::begin()
{
    if (isEnabledFlag)
        return;
    MicClass_Start_Hal(&mic_mems);
    isEnabledFlag = true;
}

void MICSensor::getMICData(char& micData)
{
    if(mic_mems.status_buf)
    {
        // for(int i = 0; i < 2048; i++)
        // {
        //     micData = mic_mems.P_buf[i];
        // }
        micData = mic_mems.P_buf[1024];
        mic_mems.status_buf = 0;
    }
}

void MICSensor::getMICDataToSerial(USARTSerial& serial)
{
    if(mic_mems.status_buf)
    {
        for(int i = 0; i < 2048; i++)
        {
            serial.write(mic_mems.P_buf[i]);
        }
        mic_mems.status_buf = 0;
    }
}

void MICSensor::getMICDataToUSBSerial(USBSerial& serialusb)
{
    if(mic_mems.status_buf)
    {
        for(int i = 0; i < 2048; i++)
        {
            serialusb.write(mic_mems.P_buf[i]);
        }
        mic_mems.status_buf = 0;
    }
}


BMI160Sensor BMI160;
BMP280Sensor BMP280;
LightSensor  lightSensor;
MICSensor    micSensor;

NeutronSensors::NeutronSensors()
{
    isEnabledFlag = false;
}

bool NeutronSensors::isEnabled(void)
{
    return isEnabledFlag;
}

void NeutronSensors::setBMI160Range(uint8_t accel_range, uint8_t gyro_range)
{
    BMI160.setRange(accel_range, gyro_range);
}
void NeutronSensors::begin(void)
{
    if (isEnabledFlag)
        return;
    BMI160.begin();
    BMP280.begin();
    lightSensor.begin();
    micSensor.begin();
    isEnabledFlag = true;
}


// BMI160 and BMM150 sensor
void NeutronSensors::getAccelData(int16_t accel[3])
{
    BMI160.getAccelData(accel);
}

void NeutronSensors::getGyroData(int16_t gyro[3])
{
    BMI160.getGyroData(gyro);
}

void NeutronSensors::getMagData(int16_t mag[3])
{
    BMI160.getMagData(mag);

}

void NeutronSensors::getAccelGyroData(int16_t accel[3], int16_t gyro[3])
{
    BMI160.getAccelGyroData(accel, gyro);
}

void NeutronSensors::getAccelGyroMagData(int16_t accel[3], int16_t gyro[3], int16_t mag[3])
{
    BMI160.getAccelGyroMagData(accel, gyro, mag);
}

// BMP280 sensor
void NeutronSensors::getPressureAltitudeData(double& pressure, double& altitude)
{
    double temperature;
    BMP280.getTemperaturePressureData(temperature, pressure);
    BMP280.getAltitudeData(altitude);
}

// Light sensor
void NeutronSensors::getLightData(uint16_t& lightData)
{
    lightSensor.getLightData(lightData);
}

// Mic sensor
void NeutronSensors::getMICData(char& micData)
{
    micSensor.getMICData(micData);
}

void NeutronSensors::getMICDataToSerial(USARTSerial& serial)
{
    micSensor.getMICDataToSerial(serial);
}

void NeutronSensors::getMICDataToUSBSerial(USBSerial& serialusb)
{
    micSensor.getMICDataToUSBSerial(serialusb);
}


// All Sernsors
void NeutronSensors::getAllDataExceptMic(int16_t accel[3], int16_t gyro[3], int16_t mag[3], double& temperature, double& pressure, double& alt, uint16_t& lightData)
{
    BMI160.getAccelGyroMagData(accel, gyro, mag);
    BMP280.getTemperaturePressureData(temperature, pressure);
    BMP280.getAltitudeData(alt);
    lightSensor.getLightData(lightData);
}

NeutronSensors Neutron;
