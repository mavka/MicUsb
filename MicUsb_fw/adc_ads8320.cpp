/*
 * adc_ads8320.cpp
 *
 *  Created on: 06 марта 2014 г.
 *      Author: g.kruglov
 */

#include "adc_ads8320.h"
#include "uart.h"
#include "core_cmInstr.h"
#include "main.h"

Adc_t Adc;

extern "C" {
// DMA irq
void SIrqDmaHandler(void *p, uint32_t flags) { Adc.IrqDmaHandler(); }
} // extern c

void Adc_t::Init() {
    PinSetupOut(ADC_GPIO, ADC_CSIN_PIN, omPushPull, pudNone);
    PinSetupAlterFunc(ADC_GPIO, ADC_SCK_PIN, omPushPull, pudNone, ADC_SPI_AF);
    PinSetupAlterFunc(ADC_GPIO, ADC_DOUT_PIN, omPushPull, pudNone, ADC_SPI_AF);
    CsHi();
    // ==== SPI ====    MSB first, master, ClkLowIdle, FirstEdge, Baudrate=...
    // Select baudrate (2.4MHz max): APB=32MHz => div = 16
    ISpi.Setup(SPI_ADC, boMSB, cpolIdleLow, cphaFirstEdge, sbFdiv16);
    ISpi.SetRxOnly();
    ISpi.EnableRxDma();
    // ==== DMA ====
    dmaStreamAllocate     (DMA_ADC, IRQ_PRIO_MEDIUM, SIrqDmaHandler, NULL);
    dmaStreamSetPeripheral(DMA_ADC, &SPI_ADC->DR);
    dmaStreamSetMode      (DMA_ADC, ADC_DMA_MODE);
}

uint16_t Adc_t::Measure() {
    ISpi.Enable();
    CsLo();
    uint8_t b;
    uint32_t r = 0;
    b = ISpi.ReadWriteByte(0);
    r = b;
    b = ISpi.ReadWriteByte(0);
    r = (r << 8) | b;
    b = ISpi.ReadWriteByte(0);
    r = (r << 8) | b;
    CsHi();
    ISpi.Disable();
    r >>= 2;
    r &= 0xFFFF;
    return r;
}

void Adc_t::StartDMAMeasure() {
    (void)SPI_ADC->DR;  // Clear input register
    dmaStreamSetMemory0(DMA_ADC, &Rslt);
    dmaStreamSetTransactionSize(DMA_ADC, 3);
    dmaStreamSetMode(DMA_ADC, ADC_DMA_MODE);
    dmaStreamEnable(DMA_ADC);
    CsLo();
    ISpi.Enable();
}

void Adc_t::IrqDmaHandler() {
    chSysLockFromISR();
    ISpi.Disable();
    CsHi();
    dmaStreamDisable(DMA_ADC);
    Rslt = __REV(Rslt);
    Rslt >>= 10;
    Rslt &= 0xFFFF;
//    Uart.PrintfI("%u\r", Rslt);
    App.SignalEvtI(EVTMSK_ADC_DONE);
    chSysUnlockFromISR();
}
