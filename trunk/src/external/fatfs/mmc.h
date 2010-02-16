#ifndef MMC_H
#define MMC_H

#include "hw_memmap.h"
#include "hw_types.h"
#include "gpio.h"
#include "ssi.h"
#include "sysctl.h"
#include "diskio.h"

#include "ssi_hw.h"

void SELECT(void);
void DESELECT(void);
void xmit_spi(BYTE dat);
BYTE rcvr_spi(void);
BYTE wait_ready(void);
void send_initial_clock_train(void);
void power_on(void);
void set_max_speed(void);
void power_off(void);
int chk_power(void);
BOOL rcvr_datablock(BYTE *buff, UINT btr);
#if _READONLY == 0
BOOL xmit_datablock(const BYTE *buff, BYTE token);
DRESULT disk_write(BYTE drv, const BYTE *buff, DWORD sector, BYTE count);
#endif /* _READONLY */
BYTE send_cmd(BYTE cmd, DWORD arg);
DSTATUS disk_initialize(BYTE drv);
DSTATUS disk_status(BYTE drv);
DRESULT disk_read(BYTE drv, BYTE *buff, DWORD sector, BYTE count);
DRESULT disk_ioctl(BYTE drv, BYTE ctrl, void *buff);
void disk_timerproc(void);
DWORD get_fattime(void);

#endif
