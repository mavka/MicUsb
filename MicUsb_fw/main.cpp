/*
 * main.cpp
 *
 *  Created on: 20 ����. 2014 �.
 *      Author: g.kruglov
 */

#include "main.h"

App_t App;

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

    PinSetupOut(GPIOB, 0, omPushPull);

    // ==== Init hardware ====
    Uart.Init(115200, UART_GPIO, UART_TX_PIN, UART_GPIO, UART_RX_PIN);
    Uart.Printf("\r%S %S\r", APP_NAME, APP_VERSION);

    PinSet(GPIOB, 0);

    Clk.PrintFreqs();
    if(ClkResult != 0) Uart.Printf("XTAL failure\r");

    App.InitThread();

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

    } // while true
}

#if 1 // ======================= Command processing ============================
void App_t::OnCmd(Shell_t *PShell) {
	Cmd_t *PCmd = &PShell->Cmd;
    __attribute__((unused)) int32_t dw32 = 0;  // May be unused in some configurations
    Uart.Printf("\r%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) PShell->Ack(OK);

    else PShell->Ack(CMD_UNKNOWN);
}

#endif
