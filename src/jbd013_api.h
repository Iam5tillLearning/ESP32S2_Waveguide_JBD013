/*
 * @Description: API function of JBD013VGA panel
 * @version: 1.1
 * @Autor: lmx
 * @LastEditors: lmx
 */
#ifndef JBD013_API_H_
#define JBD013_API_H_

#include "hal_driver.h"

//***************** JBD013VGA instruction *****************//
#define SPI_RD_ID 0x9f
#define SPI_RD_UID 0xab
#define SPI_DEEP_POWER_DOWN 0xb9
#define SPI_RST_EN 0x66
#define SPI_RST 0x99
#define SPI_SYNC 0x97
#define SPI_DISPLAY_ENABLE 0xa3
#define SPI_DISPLAY_DISABLE 0xa9
#define SPI_DISPLAY_DEFAULT_MODE 0x71
#define SPI_DISPLAY_UD 0x72
#define SPI_DISPLAY_RL 0x73
#define SPI_WR_LUM_REG 0x36
#define SPI_RD_LUM_REG 0x37
#define SPI_WR_CURRENT_REG 0x46
#define SPI_RD_CURRENT_REG 0x47
#define SPI_WR_OFFSET_REG 0xc0
#define SPI_RD_OFFSET_REG 0xc1
#define SPI_WR_CACHE 0x02
#define SPI_RD_CACHE 0x03
#define SPI_WR_CACHE_QSPI 0x62
#define SPI_RD_CACHE_QSPI 0x63
#define SPI_WR_CACHE_1BIT_QSPI 0x52
#define SPI_RD_CACHE_1BIT_QSPI 0x53
#define SPI_WR_CACHE_FAST_1BIT_QSPI 0x54
#define SPI_WR_ENABLE 0x06
#define SPI_WR_DISABLE 0x04
#define SPI_WR_STATUS_REG1 0x01
#define SPI_RD_STATUS_REG1 0x05
#define SPI_WR_STATUS_REG2 0x31
#define SPI_RD_STATUS_REG2 0x35
#define SPI_WR_STATUS_REG3 0x57
#define SPI_RD_STATUS_REG3 0x59
#define SPI_RD_CHECK_SUM_REG 0x42
#define SPI_RD_OTP 0x81
#define SPI_WR_OTP 0x82
#define SPI_SELF_TEST_ALL_OFF 0x13
#define SPI_SELF_TEST_ALL_ON 0x14
#define SPI_SELF_TEST_CHK_I 0x15
#define SPI_SELF_TEST_CHK_II 0x16
#define SPI_RD_TEMP_SENSOR 0x26

//***************** JBD013VGA api *****************//
void send_cmd(u8 cmd);                          //Send command
u32 read_id(void);                              //Read the ID of the panel
void read_uid(u8 *pBuf);                        //Read the unique ID of the panel
void wr_status_reg(u8 regAddr, u8 data);        //Write status register
u8 rd_status_reg(u8 regAddr);                   //Read status register
void wr_offset_reg(u8 row, u8 col);             //Write offset register
u16 rd_offset_reg(void);                        //Read offset register
void wr_cur_reg(u8 param);                      //Write current register
u8 rd_cur_reg(void);                            //Read current register
void wr_lum_reg(u16 param);                     //Write luminance register
u16 rd_lum_reg(void);                           //Read luminance register
void set_mirror_mode(u8 param);                 //Set mirror mode
void clr_cache(void);                           //Write the data in cache to 0
void display_image(u8 *pBuf, u32 len,u8 X,u8 Y);          //Display image
void panel_rst(void);                           //Reset panel
void panel_init(void);                          //Initialize panel
float get_temperature_sensor_data(u8 sensorId); //Get temperature sensor data
#endif