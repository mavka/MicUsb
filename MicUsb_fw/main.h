/*
 * main.h
 *
 *  Created on: 15 сент. 2014 г.
 *      Author: g.kruglov
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "ch.h"
#include "kl_lib.h"
#include "uart.h"
#include "evt_mask.h"
#include "board.h"

#define APP_NAME            "MicUSB"
#define APP_VERSION         __DATE__ " " __TIME__

enum AmpLvl_t {alvlLow = 0, alvlMid = 1, alvlHi = 2};

class App_t {
private:
    thread_t *PThread;
public:
    AmpLvl_t AmpLvl;
    // Eternal methods
    void InitThread() { PThread = chThdGetSelfX(); }
    void SignalEvt(eventmask_t Evt) {
        chSysLock();
        chEvtSignalI(PThread, Evt);
        chSysUnlock();
    }
    void SignalEvtI(eventmask_t Evt) { chEvtSignalI(PThread, Evt); }
    void OnCmd(Shell_t *PShell);
    // Inner use
    void ITask();
};

extern App_t App;

void OnSOF(USBDriver *usbp);

#endif /* MAIN_H_ */
