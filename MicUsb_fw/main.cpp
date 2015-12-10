/*
 * main.cpp
 *
 *  Created on: 20 февр. 2014 г.
 *      Author: g.kruglov
 */

#include "main.h"
#include "usb_audio.h"

App_t App;

const PinOutput_t LedUSB = {LEDUSB_GPIO, LEDUSB_PIN, omPushPull};

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

    // Leds
    LedUSB.Init();
    PinSetupOut(GPIOB, 0, omPushPull);
    PinSetupOut(GPIOB, 4, omPushPull);

    // ==== Init hardware ====
    Uart.Init(115200, UART_GPIO, UART_TX_PIN, UART_GPIO, UART_RX_PIN);
    Uart.Printf("\r%S %S\r", APP_NAME, APP_VERSION);

    Clk.PrintFreqs();
    if(ClkResult != 0) Uart.Printf("XTAL failure\r");

    App.InitThread();

    UsbAu.Connect();

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
            Uart.Printf("START_LISTEN\r");
        }
        if(EvtMsk & EVTMSK_STOP_LISTEN) {
            Uart.Printf("STOP_LISTEN\r");
        }
#endif

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
