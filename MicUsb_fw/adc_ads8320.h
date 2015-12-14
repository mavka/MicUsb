/*
 * adc.h
 *
 *  Created on: 06 ����� 2014 �.
 *      Author: g.kruglov
 */

#ifndef ADC_H_
#define ADC_H_

#include "kl_lib.h"
#include "board.h"

#define ADC_DMA_MODE    STM32_DMA_CR_CHSEL(ADC_DMA_CHNL) | \
                        DMA_PRIORITY_MEDIUM | \
                        STM32_DMA_CR_MSIZE_BYTE | \
                        STM32_DMA_CR_PSIZE_BYTE | \
                        STM32_DMA_CR_MINC | \
                        STM32_DMA_CR_DIR_P2M |    /* Direction is peripheral to memory */ \
                        STM32_DMA_CR_TCIE         /* Enable Transmission Complete IRQ */

class Adc_t {
private:
    Spi_t ISpi;
    void CsHi() { PinSet(ADC_GPIO, ADC_CSIN_PIN); }
    void CsLo() { PinClear(ADC_GPIO, ADC_CSIN_PIN); }
    uint32_t IRslt;
public:
    uint32_t Rslt;
    void Init();
    uint16_t Measure();
    void StartDMAMeasure();
    // Inner use
    void IrqDmaHandler();
};

extern Adc_t Adc;

#endif /* ADC_H_ */
