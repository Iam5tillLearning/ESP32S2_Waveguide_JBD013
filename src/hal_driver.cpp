/*
 * @Description: About API function of hardware layer
 * @version: 1.3
 * @Autor: lmx
 * @LastEditors: lmx
 */
#include "hal_driver.h"
#include "jbd013_api.h"
// 测试添加
#include "driver/gpio.h"
#include "SPI.h"
#include <esp32-hal-gpio.h>
#include <Arduino.h>
// 引脚定义
#define TFT_CLK GPIO_NUM_4
#define TFT_CS0 GPIO_NUM_14
#define TFT_CS1 GPIO_NUM_18
#define TFT_D0 GPIO_NUM_6
#define TFT_D1 GPIO_NUM_8
#define TFT_D2 GPIO_NUM_10
#define TFT_D3 GPIO_NUM_12

/**
 * @description: Delay N us
 * @paran:
 * @return {*}
 * @author: lmx
 * @param {u32} val：delay time，unit：us
 */
void delay_us(u32 val)
{
    //测试添加
    delayMicroseconds(val);
}

/**
 * @description: Delay N ms
 * @paran:
 * @return {*}
 * @author: lmx
 * @param {u32} val：delay time，unit：ms
 */
void delay_ms(u32 val)
{
    delay_us(val * 1000);
}

/**
 * @description: Set spi_cs pin status
 * @paran:
 * @return {*}
 * @author: lmx
 * @param {u8} val：spi_cs pin status bit
 */
void set_spi_cs_pin(u8 val)
{
    // 测试添加
    //digitalWrite(5,val);
    digitalWrite(33,val);
}

//测试添加
//配置QSPI
//QSPI Qspi1 = QSPI(TFT_CLK, TFT_D0, TFT_D1, TFT_D2, TFT_D3, TFT_CS0);
/**
 * @description: Send a byte of data
 * @paran:
 * @return {*}
 * @author: lmx
 * @param {u8} param：Data sent
 */
void spi_tx_byte(u8 param)
{
    // 测试添加
    // 使用SPI设备发送数据
    //SPI.transfer(param);
    SPI.write(param);
    //SPI.endTransaction();
}

/**
 * @description: Receive a byte of data
 * @paran:
 * @return {u8}：Received data
 * @author: lmx
 */
u8 spi_rx_byte(void)
{
    // 测试添加
    uint8_t data = SPI.transfer(0x00); // 发送一个空字节来接收数据
    //SPI.endTransaction();
    return data;
}

/**
 * @description: Write a byte of data
 * @paran:
 * @return {*}
 * @author: lmx
 * @param {u8} param：Data written
 */
void spi_wr_byte(u8 param)
{
    set_spi_cs_pin(SET_LOW);
    //digitalWrite(5,0);
    spi_tx_byte(param);
    //digitalWrite(5,1);
    set_spi_cs_pin(SET_HIGH);
}

/**
 * @description: Read a byte of data
 * @paran:
 * @return {u8}：Data read
 * @author: lmx
 */
u8 spi_rd_byte(u8 cmd)
{
    u8 ret;

    set_spi_cs_pin(SET_LOW);
    spi_tx_byte(cmd);
    ret = spi_rx_byte();
    set_spi_cs_pin(SET_HIGH);

    return ret;
}

/**
 * @description: Write multiple bytes data
 * @paran:
 * @return {*}
 * @author: lmx
 * @param {u8} cmd：SPI instruction of JBD013VGA panel
 * @param {u8} *pBuf：Pointer to write data
 * @param {u32} len：Length of data written
 */
void spi_wr_bytes(u8 cmd, u8 *pBuf, u32 len)
{
    u32 i;

    set_spi_cs_pin(SET_LOW);
    spi_tx_byte(cmd);
    for (i = 0; i < len; i++)
    {
        spi_tx_byte(pBuf[i]);
    }
    set_spi_cs_pin(SET_HIGH);
}

/**
 * @description: Read multiple bytes data
 * @paran:
 * @return {*}
 * @author: lmx
 * @param {u8} cmd：SPI instruction of JBD013VGA panel
 * @param {u8} *pBuf：Pointer to receive data
 * @param {u32} len：Length of received data
 */
void spi_rd_bytes(u8 cmd, u8 *pBuf, u32 len)
{
    u32 i;

    set_spi_cs_pin(SET_LOW);
    spi_tx_byte(cmd);
    for (i = 0; i < len; i++)
    {
        pBuf[i] = spi_rx_byte();
    }

    set_spi_cs_pin(SET_HIGH);
}

/**
 * @description: Read data from the panel cache
 * @paran:
 * @return {*}
 * @author: lmx
 * @param {u16} col：The starting column address of the display area（0~639）
 * @param {u16} row：The starting row address of the display area（0~479）
 * @param {u8} *pBuf：Pointer to receive data
 *  Gray：It is the gray data of pixel, which is composed of 4 bits
 *  pBuf[N] = GrayN << 4 | GrayN+1
 * @param {u32} len: Length of received data（MaxLen=640*480/2=153600）
 */
void spi_rd_cache(u16 col, u16 row, u8 *pBuf, u32 len)
{
    u32 i;
    u32 addr;

    addr = ((row & 0x1ff) << 10) | (col & 0x3ff);
    set_spi_cs_pin(SET_LOW);
    spi_tx_byte(SPI_RD_CACHE);     // CMD
    spi_tx_byte((u8)(addr >> 16)); // Addr
    spi_tx_byte((u8)(addr >> 8));  // Addr
    spi_tx_byte((u8)(addr));       // Addr
    spi_tx_byte(0xff);             // Dummy
    for (i = 0; i < len; i++)      // Pixel data
    {
        pBuf[i] = spi_rx_byte();
    }
    set_spi_cs_pin(SET_HIGH);
}

/**
 * @description: Write data to the cache in the panel
 * @paran: The data of a pixel is 4 bits, so the minimum number of changed pixels is 2
 * @return {*}
 * @author: lmx
 * @param {u16} col：The starting column address of the display area（0~639）
 * @param {u16} row：The starting row address of the display area（0~479）
 * @param {u8} *pBuf：Pointer to write data
 *  Gray：It is the gray data of pixel, which is composed of 4 bits
 *  pBuf[N] = GrayN << 4 | GrayN+1
 * @param {u32} len: Length of send data（MaxLen=640*480/2=153600）
 */
void spi_wr_cache(u16 col, u16 row, u8 *pBuf, u32 len)
{
    u8 tmpData;
    u32 i;
    u32 addr;
    u16 eCol;
    u16 eRow;

    eCol = col + (len * 2) % 640;
    eRow = row + (len * 2) / 640;
    spi_rd_cache(eCol, eRow, &tmpData, 1);

    addr = ((row & 0x1ff) << 10) | (col & 0x3ff);
    set_spi_cs_pin(SET_LOW);
    spi_tx_byte(SPI_WR_CACHE);     // CMD
    //spi_tx_byte(SPI_WR_CACHE_QSPI);  //测试修改
    spi_tx_byte((u8)(addr >> 16)); // Addr
    spi_tx_byte((u8)(addr >> 8));  // Addr
    spi_tx_byte((u8)(addr));       // Addr
    spi_tx_byte(0xff);             // Dummy
    for (i = 0; i < len; i++)      // Pixel data
    {
        spi_tx_byte(pBuf[i]);
    }
    spi_tx_byte(tmpData); // Write endPiexl gray and 4bit dummy data （Dummy data can be any value）
    set_spi_cs_pin(SET_HIGH);
}

/**
 * @description: Read the data of the temperature sensor inside the panel
 * @paran:
 * @return {*}
 * @author: lmx
 * @param {u8} sensorId：Temperature sensor ID（Range：0~3）
 * @param {u8} *pBuf：Data buf
 * @param {u16} len of pbuf(More than 2000 recommended)
 */
void spi_rd_temperature_sensor(u8 sensorId, u8 *pBuf, u16 len)
{
    u16 i;

    set_spi_cs_pin(SET_LOW);
    spi_tx_byte(SPI_RD_TEMP_SENSOR); // CMD
    spi_tx_byte(sensorId);           // sensorId
    spi_tx_byte(0);                  // dummy data
    spi_tx_byte(0);                  // dummy data
    for (i = 0; i < len; i++)        // Data buf
    {
        pBuf[i] = spi_rx_byte();
    }
    set_spi_cs_pin(SET_HIGH);
}
