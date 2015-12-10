/*
 * main.cpp
 *
 *  Created on: 20 февр. 2014 г.
 *      Author: g.kruglov
 */

#include "main.h"
#include "usb_audio.h"
#include "adc_ads8320.h"

App_t App;
Timer_t SamplingTmr = {SAMPLING_TMR};

const PinOutput_t LedUSB = {LEDUSB_GPIO, LEDUSB_PIN, omPushPull};
const PinOutput_t Led[3] = {
        {LED1_GPIO, LED1_PIN, omPushPull},
        {LED3_GPIO, LED3_PIN, omPushPull},
        {LED4_GPIO, LED4_PIN, omPushPull},
};

int main(void) {

    // ==== Setup clock frequency ====
    uint8_t ClkResult = 1;
    SetupVCore(vcore1V8);
    Clk.SetupFlashLatency(32);  // Setup Flash Latency for clock in MHz
    Clk.SetupPLLMulDiv(pllMul8, pllDiv3);
    Clk.SetupBusDividers(ahbDiv1, apbDiv1, apbDiv1);
    ClkResult = Clk.SwitchToPLL();
    Clk.UpdateFreqValues();

    // Init OS
    halInit();
    chSysInit();

    // ==== Init hardware ====
    Uart.Init(115200, UART_GPIO, UART_TX_PIN, UART_GPIO, UART_RX_PIN);
    Uart.Printf("\r%S %S\r", APP_NAME, APP_VERSION);

    Clk.PrintFreqs();
    if(ClkResult != 0) Uart.Printf("XTAL failure\r");

    App.InitThread();

    // Leds
    LedUSB.Init();
    for(uint8_t i=0; i<3; i++) Led[i].Init();

    Adc.Init();

    // ==== Sampling timer ====
    SamplingTmr.Init();
    SamplingTmr.SetUpdateFrequency(16000); // Start Fsmpl value
    SamplingTmr.EnableIrq(SAMPLING_TMR_IRQ, IRQ_PRIO_MEDIUM);
    SamplingTmr.EnableIrqOnUpdate();
    SamplingTmr.Enable();

    // Connect USB
//    UsbAu.Connect();

    // Main cycle
    App.ITask();
}

__attribute__ ((__noreturn__))
void App_t::ITask() {
    while(true) {
        uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);
        if(EvtMsk & EVTMSK_UART_NEW_CMD) {
            OnCmd((Shell_t*)&Uart);
            Uart.SignalCmdProcessed();
        }
#if 1 // ==== USB ====
        if(EvtMsk & EVTMSK_USB_READY) {
            Uart.Printf("UsbReady\r");
            LedUSB.SetHi();
        }
        if(EvtMsk & EVTMSK_START_LISTEN) {
//            Uart.Printf("START_LISTEN\r");
        }
        if(EvtMsk & EVTMSK_STOP_LISTEN) {
//            Uart.Printf("STOP_LISTEN\r");
        }
#endif

        if(EvtMsk & EVTMSK_ADC_DONE) {
            Led[0].SetLo();
        }

    } // while true
}

#if 1 // ======================= Command processing ============================
void App_t::OnCmd(Shell_t *PShell) {
	Cmd_t *PCmd = &PShell->Cmd;
    __attribute__((unused)) int32_t dw32 = 0;  // May be unused in some configurations
    Uart.Printf("\r%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) {
        PShell->Ack(OK);
    }

    else PShell->Ack(CMD_UNKNOWN);
}

#endif

#if 1 // ============================= IRQ =====================================
// Sampling IRQ: start new measurement. ADC will inform app when completed.
#if 1 // ==== Sampling Timer =====
extern "C" {
void SAMPLING_TMR_IRQHandler(void) {
    CH_IRQ_PROLOGUE();
    chSysLockFromISR();
    if(SAMPLING_TMR->SR & TIM_SR_UIF) {
        SAMPLING_TMR->SR &= ~TIM_SR_UIF;
        Adc.StartDMAMeasure();
        Led[0].SetHi();
    }
    chSysUnlockFromISR();
    CH_IRQ_EPILOGUE();
}
}
#endif

#endif
