#ifndef FLASH_H__09_04_2025__11_10
#define FLASH_H__09_04_2025__11_10

#pragma once

#include "FLASH\FlashMem.h"
#include "MEM\mem.h"
#include "ComPort\ComPort.h"

typedef FlashMem::FLWB FLWB;

extern MB* AllocFlashBuffer(u32 minLen);
extern bool RequestBoot(Ptr<MB> &mb, ComPort::WriteBuffer *wb);
extern bool FlashLoadParams(void* data, u16 len);
extern bool FlashSaveParams(Ptr<MB> mb);
extern void FlashUpdate();
extern void FlashInit();

#endif //FLASH_H__09_04_2025__11_10
