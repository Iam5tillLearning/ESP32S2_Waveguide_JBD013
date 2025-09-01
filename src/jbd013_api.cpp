/*send_cmd
 * @Description: API function of JBD013VGA panel
 * @version: 1.3
 * @Autor: lmx
 * @LastEditors: lmx
 */
#include "jbd013_api.h"
#include "string.h"

/**
 * @description: Send command
 * @paran: 
 * @return {*}
 * @author: lmx
 * @param {u8} cmd：SPI instruction of JBD013VGA panel
 * 这段代码定义了一个发送命令的函数，函数名为send_cmd，
 * 参数是一个u8类型的变量cmd，表示JBD013VGA面板的SPI指令。
 * 在函数中，调用了spi_wr_byte函数，将cmd作为参数传递给该函数，来实现将命令发送给面板。
 */
void send_cmd(u8 cmd)
{
    spi_wr_byte(cmd);
}

/**
 * @description: Read the ID of the panel
 * @paran: 
 * @return {u32}：ID of the panel
 * @author: lmx
 * 这段代码定义了一个读取面板ID的函数，函数名为read_id，
 * 返回值是一个u32类型的变量，表示面板的ID。
 * 函数内部定义了一个u32类型的变量ret和一个长度为3的u8类型数组pBuf。
 * 调用了spi_rd_bytes函数，将SPI_RD_ID作为参数传递给该函数，将读取到的ID存储在pBuf数组中。
 * 最后，将pBuf数组中的三个元素按照特定的方式合并为一个u32类型的ID，并返回。
 */
u32 read_id(void)
{
    u32 ret;
    u8 pBuf[3];

    spi_rd_bytes(SPI_RD_ID, pBuf, sizeof(pBuf));
    ret = pBuf[0] << 16 | pBuf[1] << 8 | pBuf[2];

    return ret;
}

/**
 * @description: Read the unique ID of the panel
 * @paran: 
 * @return {*}
 * @author: lmx
 * @param {u8} *pBuf：The receive pointer of uid. 
 * The corresponding cache space of the pointer should be greater than or equal to 15bytes
 * 这段代码定义了一个读取面板唯一ID的函数，函数名为read_uid，
 * 没有返回值。函数接受一个u8类型的指针pBuf作为参数，表示唯一ID的接收指针。
 * 在函数内部，调用了spi_rd_bytes函数，将SPI_RD_UID作为参数传递给该函数，
 * 将读取到的唯一ID存储在pBuf指针指向的内存空间中。
 */
void read_uid(u8 *pBuf)
{
    spi_rd_bytes(SPI_RD_UID, pBuf, 15);
}

/**
 * @description: Write status register
 * @paran: 
 * @return {*}
 * @author: lmx
 * @param {u8} regAddr：Register address
 * @param {u8} data：Write data
 * 这段代码定义了一个写状态寄存器的函数，函数名为wr_status_reg，没有返回值。
 * 函数接受两个参数，一个是u8类型的寄存器地址regAddr，另一个是u8类型的写入数据data。
 * 在函数内部，调用了spi_wr_bytes函数，将regAddr作为寄存器地址，&data作为写入数据并传递给该函数。
 */
void wr_status_reg(u8 regAddr, u8 data)
{
    spi_wr_bytes(regAddr, &data, 1);
}

/**
 * @description: Read status register
 * @paran: 
 * @return {u8}：Register data
 * @author: lmx
 * @param {u8} regAddr：Register address
 * 这段代码定义了一个读取状态寄存器的函数，函数名为rd_status_reg，
 * 返回值是一个u8类型的寄存器数据。函数接受一个u8类型的寄存器地址regAddr作为参数。
 * 在函数内部，调用了spi_rd_byte函数，将regAddr作为寄存器地址传递给该函数，并返回读取到的寄存器数据。
 */
u8 rd_status_reg(u8 regAddr)
{
    return spi_rd_byte(regAddr);
}

/**
 * @description: Write offset register
 * @paran: 
 * @return {*}
 * @author: lmx
 * @param {u8} row：Row offset address（0~31）
 * @param {u8} col：Col offset address（0~31）
 * 这段代码定义了一个写偏移寄存器的函数，函数名为wr_offset_reg，没有返回值。
 * 函数接受两个参数，一个是u8类型的行偏移地址row，另一个是u8类型的列偏移地址col。
 * 在函数内部，定义了一个长度为2的u8类型数组pBuf，将row和col分别存储在数组中。
 * 然后调用了spi_wr_bytes函数，将SPI_WR_OFFSET_REG作为寄存器地址，pBuf作为写入的数据数组，并传递给该函数。
 * 接着调用了send_cmd函数发送命令SPI_SYNC，实现偏移寄存器数据的同步。
 * 最后，延时1毫秒，等待同步操作完成。
 */
void wr_offset_reg(u8 row, u8 col)
{
    u8 pBuf[2];

    pBuf[0] = row;
    pBuf[1] = col;
    spi_wr_bytes(SPI_WR_OFFSET_REG, pBuf, 2);
    send_cmd(SPI_SYNC); //Synchronous offset reg data
    delay_ms(1);        //SYNC needs 1ms time(8MHz system clock) or 0.5ms time(16MHz system clock) to finish operation.
}

/**
 * @description: Read offset register
 * @paran: 
 * @return {u16}：returnData = row << 8 | col
 * @author: lmx
 * 这段代码定义了一个读取偏移寄存器的函数，函数名为rd_offset_reg，返回值是一个u16类型的数据，
 * 该数据的值是row左移8位后与col进行按位或操作的结果。函数没有接受任何参数。
 * 在函数内部，定义了一个长度为2的u8类型数组pBuf和一个u16类型的变量ret。
 * 调用了spi_rd_bytes函数，将SPI_RD_OFFSET_REG作为寄存器地址，pBuf作为接收数据的数组，并传递给该函数。
 * 接着，将pBuf数组中的两个元素按照特定的方式合并为一个u16类型的数据，并将其赋值给ret变量，最后返回ret。
 */
u16 rd_offset_reg(void)
{
    u8 pBuf[2];
    u16 ret;

    spi_rd_bytes(SPI_RD_OFFSET_REG, pBuf, sizeof(pBuf));
    ret = pBuf[0] << 8 | pBuf[1];

    return ret;
}

/**
 * @description: Write current register
 * @paran: 
 * @return {*}
 * @author: lmx
 * @param {u8} param：Write data（0~63）
 * 这段代码定义了一个写入电流寄存器的函数，函数名为wr_cur_reg，没有返回值。
 * 函数接受一个u8类型的参数param，表示要写入的数据（范围为0~63）。
 * 在函数内部，调用了spi_wr_bytes函数，将SPI_WR_CURRENT_REG作为寄存器地址，&param作为写入的数据并传递给该函数。
 */
void wr_cur_reg(u8 param)
{
    spi_wr_bytes(SPI_WR_CURRENT_REG, &param, 1);
}

/**
 * @description: Read current register
 * @paran: 
 * @return {u8}：Read data
 * @author: lmx
 * 这段代码定义了一个读取电流寄存器的函数，函数名为rd_cur_reg，返回值是一个u8类型的数据。
 * 函数没有接受任何参数。
 * 在函数内部，调用了spi_rd_byte函数，将SPI_RD_CURRENT_REG作为寄存器地址传递给该函数，并返回读取到的寄存器数据。
 */
u8 rd_cur_reg(void)
{
    return spi_rd_byte(SPI_RD_CURRENT_REG);
}

/**
 * @description: Write luminance register
 * @paran: 
 * @return {*}
 * @author: lmx
 * @param {u16} param：Write data
 * When the self refresh frequency is 25Hz, param (0 ~ 21331)
 * When the self refresh frequency is 50Hz, param (0 ~ 10664)
 * When the self refresh frequency is 75Hz, param (0 ~ 7109)
 * When the self refresh frequency is 100Hz, param (0 ~ 5331)
 * When the self refresh frequency is 125Hz, param (0 ~ 4264)
 * When the self refresh frequency is 150Hz, param (0 ~ 3366)
 * When the self refresh frequency is 175Hz, param (0 ~ 2907)
 * When the self refresh frequency is 200Hz, param (0 ~ 2558)
 * 这段代码定义了一个写亮度寄存器的函数，函数名为wr_lum_reg，没有返回值。
 * 函数接受一个u16类型的参数param，表示要写入的数据。
 * 在函数内部，定义了一个长度为2的u8类型数组pBuf，
 * 将param右移8位后的高8位存储在pBuf数组的第一个元素中，将param的低8位存储在pBuf数组的第二个元素中。
 * 然后调用了spi_wr_bytes函数，将SPI_WR_LUM_REG作为寄存器地址，pBuf作为写入的数据数组，并传递给该函数。
 */
void wr_lum_reg(u16 param)
{
    u8 pBuf[2];

    pBuf[0] = param >> 8;
    pBuf[1] = param;
    spi_wr_bytes(SPI_WR_LUM_REG, pBuf, 2);
}

/**
 * @description: Read luminance register
 * @paran: 
 * @return {u16}：Read data
 * @author: lmx
 * 这段代码定义了一个读取亮度寄存器的函数，函数名为rd_lum_reg，返回值是一个u16类型的数据。
 * 函数没有接受任何参数。在函数内部，定义了一个长度为2的u8类型数组pBuf和一个u16类型的变量ret。
 * 调用了spi_rd_bytes函数，将SPI_RD_LUM_REG作为寄存器地址，pBuf作为接收数据的数组，并传递给该函数。
 * 接着，将pBuf数组中的两个元素按照特定的方式合并为一个u16类型的数据，并将其赋值给ret变量，最后返回ret。
 */
u16 rd_lum_reg(void)
{
    u8 pBuf[2];
    u16 ret;

    spi_rd_bytes(SPI_RD_LUM_REG, pBuf, sizeof(pBuf));
    ret = pBuf[0] << 8 | pBuf[1];

    return ret;
}

/**
 * @description: Set mirror mode
 * @paran: 
 * @return {*}
 * @author: lmx
 * @param {u8} param：Set mirror mode
 * param = 0：Normal display
 * param = 1: Mirror left and right only
 * param = 2: Mirror up and down only
 * param = 3: Mirror up, down, left and right at the same time
 * 这段代码定义了一个设置镜像模式的函数，函数名为set_mirror_mode，没有返回值。
 * 函数接受一个u8类型的参数param，表示要设置的镜像模式。在函数内部，调用了send_cmd函数，发送命令SPI_DISPLAY_DEFAULT_MODE。
 * 根据param的值，判断是否需要发送SPI_DISPLAY_RL和SPI_DISPLAY_UD命令来设置左右镜像和上下镜像。
 * 最后，再发送命令SPI_SYNC进行同步操作，延时1毫秒，等待同步操作完成。
 */
void set_mirror_mode(u8 param)
{
    send_cmd(SPI_DISPLAY_DEFAULT_MODE);
    if (param == 1 || param == 3)
    {
        send_cmd(SPI_DISPLAY_RL);
    }
    if (param == 2 || param == 3)
    {
        send_cmd(SPI_DISPLAY_UD);
    }
    send_cmd(SPI_SYNC);
    delay_ms(1);
}

/**
 * @description: Write the data in cache to 0
 * @paran: 
 * @return {*}
 * @author: lmx
 * 这段代码定义了一个清空缓存的函数，函数名为clr_cache，没有返回值。
 * 在函数内部，定义了一个长度为10的u8类型数组pBuf
 * 和三个变量pBufLen、addrStep、rowCnt、colCnt。
 * pBufLen的值为pBuf数组的长度，addrStep的值为pBufLen乘以2。
 * 调用了memset函数，将pBuf数组的所有元素都设置为0。
 * 通过两个嵌套的for循环，将pBuf数组中的数据写入缓存中的每个地址。
 * 如果640除以addrStep的余数不为0，则需要额外写入一部分数据以保证完整。
 */
void clr_cache(void)
{
    u8 pBuf[10];
    u8 addrStep;
    u32 pBufLen;
    u16 rowCnt;
    u16 colCnt;

    pBufLen = sizeof(pBuf);
    addrStep = pBufLen * 2;
    memset(pBuf, 0, pBufLen);
    for (rowCnt = 0; rowCnt < 480; rowCnt++)
    {
        for (colCnt = 0; colCnt < 640; colCnt += addrStep)
        {
            spi_wr_cache(colCnt, rowCnt, pBuf, pBufLen);
        }
        if (640 % addrStep != 0)
        {
            spi_wr_cache((640 - 640 % addrStep), rowCnt, pBuf, 640 % addrStep);
        }
    }
}

/**
 * @description: Display image
 * @paran: 
 * @return {*}
 * @author: lmx
 * @param {u8} *pBuf：Pointer to image data
 * @param {u32} len：Length of image data（0~153600）
 * 这段代码定义了一个显示图片的函数，函数名为display_image，没有返回值。
 * 函数接受两个参数，一个是u8类型的指针pBuf，表示指向图片数据的指针，
 * 另一个是u32类型的变量len，表示图片数据的长度（范围为0~153600）。
 * 在函数内部，调用了spi_wr_cache函数，将图片数据写入缓存中。
 * 然后调用send_cmd函数发送命令SPI_SYNC，实现缓存数据的同步。最后，延时1毫秒，等待同步操作完成。
 */
void display_image(u8 *pBuf, u32 len,u8 X,u8 Y)
{
    spi_wr_cache(X, Y, pBuf, len); //Write data to cache
    send_cmd(SPI_SYNC);            //Synchronous cache data
    delay_ms(1);                   //SYNC needs 1ms time(8MHz system clock) or 0.5ms time(16MHz system clock) to finish operation.
}

/**
 * @description: Reset panel
 * @paran: 
 * @return {*}
 * @author: lmx
 * 这段代码定义了一个复位面板的函数，函数名为panel_rst，没有返回值。
 * 函数内部调用了send_cmd函数两次，分别发送了SPI_RST_EN和SPI_RST命令，实现了面板的复位功能。
 * 最后，延时50毫秒，等待面板复位完成。
 */
void panel_rst(void)
{
    send_cmd(SPI_RST_EN);
    send_cmd(SPI_RST);
    delay_ms(50);
}

/**
 * @description: Initialize panel
 * @paran: 
 * @return {*}
 * @author: lmx
 * 这段代码定义了一个初始化面板的函数，函数名为panel_init，没有返回值。
 * 在函数内部，首先调用panel_rst函数进行面板复位。然后调用send_cmd函数发送命令SPI_WR_ENABLE，打开状态寄存器的写使能。
 * 再调用wr_status_reg函数，写入特定的值0x10到状态寄存器中，以关闭demura。
 * 通过调用clr_cache函数，将所有缓存数据清零。
 * 接着调用wr_offset_reg函数设置偏移寄存器的值，分别设置四个角以及实际偏移值，将屏幕居中。
 * 再调用wr_cur_reg函数设置电流寄存器的值为63。
 * 最后，通过调用send_cmd函数发送命令SPI_DISPLAY_ENABLE，打开显示使能。
 * 最后发送命令SPI_SYNC进行同步操作，并延时1毫秒，等待同步操作完成。
 */
void panel_init(void)
{
    //Reset panel
    panel_rst();

    //Open status register write enable
    send_cmd(SPI_WR_ENABLE);

    //Close demura
    wr_status_reg(SPI_WR_STATUS_REG1, 0x10);

    //Set all cache data to 0
    clr_cache();

    //Set the offset in the upper left corner
    wr_offset_reg(0, 0);

    //Set the offset in the upper right corner
    wr_offset_reg(0, 20);

    //Set the offset in the lower left corner
    wr_offset_reg(24, 0);

    //Set the offset in the lower right corner
    wr_offset_reg(24, 20);

    //Set actual offset,center the screen
    wr_offset_reg(12, 10);

    //Set current reg
    wr_cur_reg(63);

    //Set display enable
    send_cmd(SPI_DISPLAY_ENABLE);

    //Synchronous setting
    send_cmd(SPI_SYNC);
    delay_ms(1);
}

/**
 * @description: Get temperature sensor data
 * @paran: 
 * @return {float}：Temperature data(Unit:℃)
 * @author: lmx
 * @param {u8} sensorId：Temperature sensor ID（Range：0~3）
 * 这段代码定义了一个获取温度传感器数据的函数，函数名为get_temperature_sensor_data，
 * 返回值为一个float类型的温度数据（单位：℃）。
 * 函数接受一个u8类型的参数sensorId，表示温度传感器的ID（范围为0~3）。
 * 在函数内部，定义了一个长度为2000的u8类型数组pBuf和一个长度为8的u8类型数组maskBuf，用于存储温度传感器数据和掩码数据。
 * 还定义了一些临时变量如tmpNum、isFlag、tmpVal、i、rxBitCnt和bitCnt。
 * 通过调用spi_rd_temperature_sensor函数，将sensorId作为参数传递给该函数，
 * 读取面板内部温度传感器的数据，并存储在pBuf数组中。
 * 接下来，通过for循环和一些逻辑判断的方式解析数据。
 * 遍历pBuf数组中的每个元素，如果元素的值大于0，则将其赋值给tmpNum。
 * 然后通过嵌套的for循环，遍历tmpNum的每个bit位，
 * 根据特定的位操作，解析出温度数据，并将其存储在tmpVal变量中。
 * 最后，通过一系列的数学计算，将tmpVal转换为温度数据（单位：℃），并返回。
 */
float get_temperature_sensor_data(u8 sensorId)
{
    u8 pBuf[2000];
    u8 maskBuf[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    u8 tmpNum;
    u8 isFlag;
    u16 tmpVal = 0;
    u16 i;
    char rxBitCnt = -2;
    char bitCnt;

    spi_rd_temperature_sensor(sensorId, pBuf, sizeof(pBuf)); //Read the data of the temperature sensor inside the panel

    for (i = 0, isFlag = 0; i < 2000; i++) //Parsing data
    {
        if (pBuf[i] > 0)
        {
            tmpNum = pBuf[i];

            for (bitCnt = 7; bitCnt >= 0; bitCnt--)
            {
                if (rxBitCnt == -2)
                {
                    if (isFlag == 0 && ((tmpNum & maskBuf[bitCnt]) >= 1))
                    {
                        isFlag = 1;
                    }
                    else if (isFlag == 1 && ((tmpNum & maskBuf[bitCnt]) == 0))
                    {
                        isFlag = 2;
                    }
                    else if (isFlag == 2 && ((tmpNum & maskBuf[bitCnt]) == 0))
                    {
                        isFlag = 3;
                    }
                    else if (isFlag == 3 && ((tmpNum & maskBuf[bitCnt]) >= 1))
                    {
                        isFlag = 4;
                        rxBitCnt = 11;
                        continue;
                    }
                    else
                    {
                        isFlag = 0;
                    }
                }
                if (rxBitCnt >= 0)
                {
                    tmpVal |= (((tmpNum & maskBuf[bitCnt]) >> bitCnt) << rxBitCnt);
                    rxBitCnt--;
                    if (rxBitCnt == -1)
                        break;
                }
            }
        }
        else
        {
            isFlag = 0;
        }
        if (rxBitCnt == -1)
            break;
    }

    return (float)((tmpVal - 1600.1) / 7.5817); //Return temperature data(Unit:℃)
}