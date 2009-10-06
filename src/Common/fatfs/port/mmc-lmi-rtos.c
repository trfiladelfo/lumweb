#include "diskio.h"
#include "hw_types.h"
#include "hw_sysctl.h"
#include "hw_memmap.h"
#include "hw_ssi.h"
#include "sysctl.h"
#include "ssi.h"
#include "gpio.h"
#include "sysctl.h"

// Scheduler includes.
#include "FreeRTOS.h"
#include "task.h"

#include "filesystem.h"


/* Definitions for MMC/SDC command */
#define CMD0    (0x40+0)    /* GO_IDLE_STATE */
#define CMD1    (0x40+1)    /* SEND_OP_COND */
#define CMD8    (0x40+8)    /* SEND_IF_COND */
#define CMD9    (0x40+9)    /* SEND_CSD */
#define CMD10    (0x40+10)    /* SEND_CID */
#define CMD12    (0x40+12)    /* STOP_TRANSMISSION */
#define CMD16    (0x40+16)    /* SET_BLOCKLEN */
#define CMD17    (0x40+17)    /* READ_SINGLE_BLOCK */
#define CMD18    (0x40+18)    /* READ_MULTIPLE_BLOCK */
#define CMD23    (0x40+23)    /* SET_BLOCK_COUNT */
#define CMD24    (0x40+24)    /* WRITE_BLOCK */
#define CMD25    (0x40+25)    /* WRITE_MULTIPLE_BLOCK */
#define CMD41    (0x40+41)    /* SEND_OP_COND (ACMD) */
#define CMD55    (0x40+55)    /* APP_CMD */
#define CMD58    (0x40+58)    /* READ_OCR */



// asserts the CS pin to the card
static
void SELECT(void)
{
    GPIOPinWrite(SDC_CS_GPIO_PORT_BASE, SDC_CS, 0);
}

// de-asserts the CS pin to the card
static
void DESELECT(void)
{
    GPIOPinWrite(SDC_CS_GPIO_PORT_BASE, SDC_CS, SDC_CS);
}


/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

static DSTATUS Stat = STA_NOINIT;    /* Disk status */

static BYTE CardType;            /* b0:MMC, b1:SDC, b2:Block addressing */

static BYTE PowerFlag = 0;     /* indicates if "power" is on */

/*-----------------------------------------------------------------------*/
/* Transmit a byte to MMC via SPI  (Platform dependent)                  */
/*-----------------------------------------------------------------------*/

static void xmit_spi(BYTE dat)
{
    DWORD rcvdat;

    SSIDataPut(SDC_SSI_BASE, dat); /* Write the data to the tx fifo */

    SSIDataGet(SDC_SSI_BASE, &rcvdat); /* flush data read during the write */
}


/*-----------------------------------------------------------------------*/
/* Receive a byte from MMC via SPI  (Platform dependent)                 */
/*-----------------------------------------------------------------------*/

static BYTE rcvr_spi (void)
{
    DWORD rcvdat;

    SSIDataPut(SDC_SSI_BASE, 0xFF); /* write dummy data */

    SSIDataGet(SDC_SSI_BASE, &rcvdat); /* read data frm rx fifo */

    return (BYTE)rcvdat;
}


static void rcvr_spi_m (BYTE *dst)
{
    *dst = rcvr_spi();
}

/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static BYTE wait_ready (void)
{
    BYTE res = 0;
    ;
    WORD count;


    count = TICK_MS(500);    /* Wait for ready in timeout of 500ms */
    rcvr_spi();
    do
    {
        res = rcvr_spi();
        if(res == 0xFF)
        {
            break;
        }
        vTaskDelay(1);
    }
    while(count--);
    return res;
}



/*-----------------------------------------------------------------------*/
/* Power Control  (Platform dependent)                                   */
/*-----------------------------------------------------------------------*/
/* When the target system does not support socket power control, there   */
/* is nothing to do in these functions and chk_power always returns 1.   */

static void power_on (void)
{
    unsigned int i;
    DWORD dat;

    /*
     * This doesn't really turn the power on, but initializes the
     * SSI port and pins needed to talk to the card.
     */

    /* Enable the peripherals used to drive the SDC on SSI, and the CS */
    SysCtlPeripheralEnable(SDC_SSI_SYSCTL_PERIPH);
    SysCtlPeripheralEnable(SDC_GPIO_SYSCTL_PERIPH);
    SysCtlPeripheralEnable(SDC_CS_GPIO_SYSCTL_PERIPH);

    /* Configure the appropriate pins to be SSI instead of GPIO */
    GPIODirModeSet(SDC_GPIO_PORT_BASE, SDC_SSI_PINS, GPIO_DIR_MODE_HW);
    GPIODirModeSet(SDC_CS_GPIO_PORT_BASE, SDC_CS, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(SDC_GPIO_PORT_BASE, SDC_SSI_CLK, GPIO_STRENGTH_4MA,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(SDC_GPIO_PORT_BASE, SDC_SSI_TX | SDC_SSI_RX,
                     GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(SDC_CS_GPIO_PORT_BASE, SDC_CS,
                     GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);

    /* Deassert the SSI0 chip select */
    GPIOPinWrite(SDC_CS_GPIO_PORT_BASE, SDC_CS, SDC_CS);

    /* Configure the SSI0 port */
    SSIConfig(SDC_SSI_BASE, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 400000, 8);
    SSIEnable(SDC_SSI_BASE);

    /* Set DI and CS high and apply more than 74 pulses to SCLK for the card */
    /* to be able to accept a native command. */
    for(i = 0 ; i < 16 ; i++)
    {
        /* Write DUMMY data. SSIDataPut() waits until there is room in the FIFO */
        SSIDataPut(SDC_SSI_BASE, 0xFF);

        /* Flush data read during data write. */
        SSIDataGet(SDC_SSI_BASE, &dat);
    }

    PowerFlag = 1;
}

// set the SSI speed to the max setting
static void set_max_speed(void)
{
    //Disable the SSI
    SSIDisable(SDC_SSI_BASE);

    // Configure the SSI0 port
    // set the speed to half the system clock
    SSIConfig(SDC_SSI_BASE, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER,
              SDC_MAX_SPEED, 8);

    // Enable the SSI
    SSIEnable(SDC_SSI_BASE);
}

static void power_off (void)
{
    PowerFlag = 0;
}

static int chk_power(void)        /* Socket power state: 0=off, 1=on */
{
    return PowerFlag;
}



/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/

static BOOL rcvr_datablock ( BYTE *buff, UINT btr)
{
    BYTE token;
    WORD count;


    count = TICK_MS(100);
    do
    {                            /* Wait for data packet in timeout of 100ms */
        token = rcvr_spi();
        if(token != 0xFF)
        {
            break;
        }
        vTaskDelay(1);
    }
    while ((token == 0xFF) && count--);

    if(token != 0xFE)
        return FALSE;    /* If not valid data token, retutn with error */

    do
    {                            /* Receive the data block into buffer */
        rcvr_spi_m(buff++);
        rcvr_spi_m(buff++);
    }
    while (btr -= 2);

    rcvr_spi();                        /* Discard CRC */
    rcvr_spi();

    return TRUE;                    /* Return with success */
}



/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
static BOOL xmit_datablock ( const BYTE *buff, BYTE token)
{
    BYTE resp, wc;


    if (wait_ready() != 0xFF)
    {
        return FALSE;
    }

    xmit_spi(token);                    /* Xmit data token */

    //Is data token
    if (token != 0xFD)
    {
        wc = 0;
        do
        {                            /* Xmit the 512 byte data block to MMC */
            xmit_spi(*buff++);
            xmit_spi(*buff++);
        }
        while (--wc);

        xmit_spi(0xFF);                    /* CRC (Dummy) */
        xmit_spi(0xFF);
        resp = rcvr_spi();                /* Reveive data response */

        if ((resp & 0x1F) != 0x05)        /* If not accepted, return with error */
        {
            return FALSE;
        }
    }

    return TRUE;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/

static BYTE send_cmd (BYTE cmd, DWORD arg)
{
    BYTE n, res;


    if (wait_ready() != 0xFF)
    {
        return 0xFF;
    }

    /* Send command packet */
    xmit_spi(cmd);                      /* Command */
    xmit_spi((BYTE)(arg >> 24));        /* Argument[31..24] */
    xmit_spi((BYTE)(arg >> 16));        /* Argument[23..16] */
    xmit_spi((BYTE)(arg >> 8));         /* Argument[15..8] */
    xmit_spi((BYTE)arg);                /* Argument[7..0] */
    n = 0;

    if (cmd == CMD0)
    {
        n = 0x95;            /* CRC for CMD0(0) */
    }
    else if (cmd == CMD8)
    {
        n = 0x87;            /* CRC for CMD8(0x1AA) */
    }

    xmit_spi(n);

    /* Receive command response */
    if (cmd == CMD12)
    {
        rcvr_spi();        /* Skip a stuff byte when stop reading */
    }

    n = 10;
    /* Wait for a valid response in timeout of 10 attempts */
    do
    {
        res = rcvr_spi();
    }
    while ((res & 0x80) && --n);

    return res;            /* Return with the response value */
}



/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (BYTE drv)
{
    BYTE n, ty, ocr[4];
    WORD count;


    if (drv)
    {
        return STA_NOINIT;            /* Supports only single drive */
    }

    if (Stat & STA_NODISK)
    {
        return Stat;    /* No card in the socket */
    }

    power_on();                            /* Force socket power on */

    for (n = 10; n; n--)
    {
        rcvr_spi();    /* 80 dummy clocks */
    }

    SELECT();                /* CS = L */
    ty = 0;

    //Enter Idle state
    if (send_cmd(CMD0, 0) == 1)
    {
        count = TICK_MS(1000);                        /* Initialization timeout of 1000 msec */
        if (send_cmd(CMD8, 0x1AA) == 1)     /* SDC Ver2+ */
        {
            for (n = 0; n < 4; n++)
            {
                ocr[n] = rcvr_spi();
            }
            if (ocr[2] == 0x01 && ocr[3] == 0xAA)  /* The card can work at vdd range of 2.7-3.6V */
            {
                do
                {
                    if (send_cmd(CMD55, 0) <= 1 && send_cmd(CMD41, 1UL << 30) == 0)
                    {
                        break;    /* ACMD41 with HCS bit */
                    }
                    vTaskDelay(1);
                }
                while (count--);
                if (count && send_cmd(CMD58, 0) == 0) /* Check CCS bit */
                {
                    for (n = 0; n < 4; n++)
                    {
                        ocr[n] = rcvr_spi();
                    }
                    ty = (ocr[0] & 0x40) ? 6 : 2;
                }
            }
        }
        else
        {                            /* SDC Ver1 or MMC */
            ty = (send_cmd(CMD55, 0) <= 1 && send_cmd(CMD41, 0) <= 1) ? 2 : 1;    /* SDC : MMC */
            do
            {
                if (ty == 2)
                {
                    if (send_cmd(CMD55, 0) <= 1 && send_cmd(CMD41, 0) == 0)
                    {
                        break;    /* ACMD41 */
                    }
                }
                else
                {
                    if (send_cmd(CMD1, 0) == 0)
                    {
                        break;                                /* CMD1 */
                    }
                }
                vTaskDelay(1);
            }
            while (count--);

            if (!count || send_cmd(CMD16, 512) != 0)    /* Select R/W block length */
            {
                ty = 0;
            }
        }
    }

    CardType = ty;
    DESELECT();            /* CS = H */
    rcvr_spi();            /* Idle (Release DO) */

    if (ty)  /* Initialization succeded */
    {
        Stat &= ~STA_NOINIT;        /* Clear STA_NOINIT */
        set_max_speed();
    }
    else /* Initialization failed */
    {
        power_off();
    }

    return Stat;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE drv        /* Physical drive nmuber (0) */
)
{
    if (drv)
        return STA_NOINIT;        /* Supports only single drive */
    return Stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE drv,            /* Physical drive nmuber (0) */
    BYTE *buff,            /* Pointer to the data buffer to store read data */
    DWORD sector,        /* Start sector number (LBA) */
    BYTE count            /* Sector count (1..255) */
)
{
    if (drv || !count)
        return RES_PARERR;
    if (Stat & STA_NOINIT)
        return RES_NOTRDY;

    if (!(CardType & 4))
        sector *= 512;    /* Convert to byte address if needed */

    SELECT();            /* CS = L */

    if (count == 1)
    {    /* Single block read */
        if ((send_cmd(CMD17, sector) == 0)    /* READ_SINGLE_BLOCK */
                && rcvr_datablock(buff, 512))
            count = 0;
    }
    else
    {                /* Multiple block read */
        if (send_cmd(CMD18, sector) == 0)
        {    /* READ_MULTIPLE_BLOCK */
            do
            {
                if (!rcvr_datablock(buff, 512))
                    break;
                buff += 512;
            }
            while (--count);
            send_cmd(CMD12, 0);                /* STOP_TRANSMISSION */
        }
    }

    DESELECT();            /* CS = H */
    rcvr_spi();            /* Idle (Release DO) */

    return count ? RES_ERROR : RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT disk_write (
    BYTE drv,            /* Physical drive nmuber (0) */
    const BYTE *buff,    /* Pointer to the data to be written */
    DWORD sector,        /* Start sector number (LBA) */
    BYTE count            /* Sector count (1..255) */
)
{
    if (drv || !count)
        return RES_PARERR;
    if (Stat & STA_NOINIT)
        return RES_NOTRDY;
    if (Stat & STA_PROTECT)
        return RES_WRPRT;

    if (!(CardType & 4))
        sector *= 512;    /* Convert to byte address if needed */

    SELECT();            /* CS = L */

    if (count == 1)
    {    /* Single block write */
        if ((send_cmd(CMD24, sector) == 0)    /* WRITE_BLOCK */
                && xmit_datablock(buff, 0xFE))
            count = 0;
    }
    else
    {                /* Multiple block write */
        if (CardType & 2)
        {
            send_cmd(CMD55, 0);
            send_cmd(CMD23, count);    /* ACMD23 */
        }
        if (send_cmd(CMD25, sector) == 0)
        {    /* WRITE_MULTIPLE_BLOCK */
            do
            {
                if (!xmit_datablock(buff, 0xFC))
                    break;
                buff += 512;
            }
            while (--count);
            if (!xmit_datablock(0, 0xFD))    /* STOP_TRAN token */
                count = 1;
        }
    }

    DESELECT();            /* CS = H */
    rcvr_spi();            /* Idle (Release DO) */

    return count ? RES_ERROR : RES_OK;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
    BYTE drv,        /* Physical drive nmuber (0) */
    BYTE ctrl,        /* Control code */
    void *buff        /* Buffer to send/receive control data */
)
{
    DRESULT res;
    BYTE n, csd[16], *ptr = (BYTE*)buff;
    WORD csize;


    if (drv)
        return RES_PARERR;

    res = RES_ERROR;

    if (ctrl == CTRL_POWER)
    {
        switch (*ptr)
        {
        case 0:        /* Sub control code == 0 (POWER_OFF) */
            if (chk_power())
                power_off();        /* Power off */
            res = RES_OK;
            break;
        case 1:        /* Sub control code == 1 (POWER_ON) */
            power_on();                /* Power on */
            res = RES_OK;
            break;
        case 2:        /* Sub control code == 2 (POWER_GET) */
            *(ptr+1) = (BYTE)chk_power();
            res = RES_OK;
            break;
        default :
            res = RES_PARERR;
        }
    }
    else
    {
        if (Stat & STA_NOINIT)
            return RES_NOTRDY;

        SELECT();        /* CS = L */

        switch (ctrl)
        {
        case GET_SECTOR_COUNT :    /* Get number of sectors on the disk (DWORD) */
            if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16))
            {
                if ((csd[0] >> 6) == 1)
                {    /* SDC ver 2.00 */
                    csize = csd[9] + ((WORD)csd[8] << 8) + 1;
                    *(DWORD*)buff = (DWORD)csize << 10;
                }
                else
                {                    /* MMC or SDC ver 1.XX */
                    n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
                    csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
                    *(DWORD*)buff = (DWORD)csize << (n - 9);
                }
                res = RES_OK;
            }
            break;

        case GET_SECTOR_SIZE :    /* Get sectors on the disk (WORD) */
            *(WORD*)buff = 512;
            res = RES_OK;
            break;

        case CTRL_SYNC :    /* Make sure that data has been written */
            if (wait_ready() == 0xFF)
                res = RES_OK;
            break;

        case MMC_GET_CSD :    /* Receive CSD as a data block (16 bytes) */
            if (send_cmd(CMD9, 0) == 0        /* READ_CSD */
                    && rcvr_datablock(ptr, 16))
                res = RES_OK;
            break;

        case MMC_GET_CID :    /* Receive CID as a data block (16 bytes) */
            if (send_cmd(CMD10, 0) == 0        /* READ_CID */
                    && rcvr_datablock(ptr, 16))
                res = RES_OK;
            break;

        case MMC_GET_OCR :    /* Receive OCR as an R3 resp (4 bytes) */
            if (send_cmd(CMD58, 0) == 0)
            {    /* READ_OCR */
                for (n = 0; n < 4; n++)
                    *ptr++ = rcvr_spi();
                res = RES_OK;
            }

            //        case MMC_GET_TYPE :    /* Get card type flags (1 byte) */
            //            *ptr = CardType;
            //            res = RES_OK;
            //            break;

        default:
                res = RES_PARERR;
        }

        DESELECT();            /* CS = H */
        rcvr_spi();            /* Idle (Release DO) */
    }
    return res;
}


/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */

DWORD get_fattime ()
{

    return    ((2000-1980) << 25)   // Year
              | (8UL << 21)         // Month
              | (18UL << 16)        // Day
              | (12U << 11)         // Hour
              | (0U << 5)           // Min
              | (0U >> 1)           // Sec
              ;

}
