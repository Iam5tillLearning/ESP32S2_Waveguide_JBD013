/*
 * @Description: About API function of hardware layer
 * @version: 1.1
 * @Autor: lmx
 * @LastEditors: lmx
 */
#ifndef HAL_DRIVER_H_
#define HAL_DRIVER_H_

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

#define SET_LOW 0  //Set spi_cs pin to low level
#define SET_HIGH 1 //Set spi_cs pin to high level

void delay_us(u32 val);                                             //Delay N us
void delay_ms(u32 val);                                             //Delay N ms
void spi_wr_byte(u8 param);                                         //Write a byte of data
u8 spi_rd_byte(u8 cmd);                                             //Read a byte of data
void spi_wr_bytes(u8 cmd, u8 *pBuf, u32 len);                       //Write multiple bytes data
void spi_rd_bytes(u8 cmd, u8 *pBuf, u32 len);                       //Read multiple bytes data
void spi_rd_cache(u16 col, u16 row, u8 *pBuf, u32 len);             //Read data from the panel cache
void spi_wr_cache(u16 col, u16 row, u8 *pBuf, u32 len);             //Write data to the cache in the panel
void spi_rd_temperature_sensor(u8 sensorId, u8 *pBuf, u16 bufSize); //Read the data of the temperature sensor inside the panel
#endif