/*
 * usb_cdc.cpp
 *
 *  Created on: 03 сент. 2015 г.
 *      Author: Kreyl
 */

#include "ch.h"
#include "hal.h"
#include "usb_audio.h"
#include "usb.h"
#include "usb_lld.h"
#include "descriptors_audio.h"
#include "main.h"

UsbAudio_t UsbAu;

#define USBDrv      USBD1   // USB driver to use

static bool OnSetupPkt(USBDriver *usbp);
static void OnDataTransmitted(USBDriver *usbp, usbep_t ep);

const int16_t sinconst[16] = {
        0, 1000, 3000, 5000, 7000, 5000, 3000, 1000, 0, -1000, -3000, -5000, -7000, -5000, -3000, -1000
};

#if 1 // ========================== Endpoints ==================================
// ==== EP1 ====
static USBInEndpointState ep1instate;
// EP1 initialization structure (both IN and OUT).
static const USBEndpointConfig ep1config = {
    USB_EP_MODE_TYPE_ISOC,
    NULL,                   // setup_cb
    OnDataTransmitted,      // in_cb
    NULL,                   // out_cb
    64,                     // in_maxsize
    0,                      // out_maxsize
    &ep1instate,            // in_state
    NULL,                   // out_state
    2,                      // in_multiplier: Determines the space allocated for the TXFIFO as multiples of the packet size
    NULL                    // setup_buf: Pointer to a buffer for setup packets. Set this field to NULL for non-control endpoints
};
#endif

#if 1 // ======================== Events & Config ==============================
static void usb_event(USBDriver *usbp, usbevent_t event) {
    switch (event) {
        case USB_EVENT_RESET:
            return;
        case USB_EVENT_ADDRESS:
            return;
        case USB_EVENT_CONFIGURED:
            chSysLockFromISR();
            /* Enable the endpoints specified in the configuration.
            Note, this callback is invoked from an ISR so I-Class functions must be used.*/
            usbInitEndpointI(usbp, EP_DATA_IN_ID, &ep1config);
            App.SignalEvtI(EVTMSK_USB_READY);
            chSysUnlockFromISR();
            return;
        case USB_EVENT_SUSPEND:
            return;
        case USB_EVENT_WAKEUP:
            return;
        case USB_EVENT_STALLED:
            return;
    } // switch
}

// ==== USB driver configuration ====
const USBConfig UsbCfg = {
    usb_event,          // This callback is invoked when an USB driver event is registered
    GetDescriptor,      // Device GET_DESCRIPTOR request callback
    OnSetupPkt,         // This hook allows to be notified of standard requests or to handle non standard requests
    NULL                // Start Of Frame callback
};


//static int16_t Buf2Send[BUF_CNT];
//static bool JustSent = false;
void OnDataTransmitted(USBDriver *usbp, usbep_t ep) {
//    if(JustSent) {
////        JustSent = false;
//        usbPrepareTransmit(&USBDrv, EP_DATA_IN_ID, NULL, 0);
//        chSysLockFromISR();
//        usbStartTransmitI(&USBDrv, EP_DATA_IN_ID);
//        chSysUnlockFromISR();
//    }
//    else {
//        // Copy data to Buf2Send
//        uint32_t Cnt = 33;//UsbAu.Buf.GetFullCount();
//        UsbAu.Buf.Get(Buf2Send, Cnt);
//    //    if(r != OK) Uart.PrintfI("r = %u\r", r);
//        // Send data
//        usbPrepareTransmit(&USBDrv, EP_DATA_IN_ID, (uint8_t*)Buf2Send, Cnt*2); // 2 bytes per sample
//        chSysLockFromISR();
//        usbStartTransmitI(&USBDrv, EP_DATA_IN_ID);
//        chSysUnlockFromISR();
////        JustSent = true;
//    }
}

void onotify(io_queue_t *qp) {
    size_t n;

    /* If the USB driver is not in the appropriate state then transactions  must not be started.*/
    if (usbGetDriverStateI(&USBDrv) != USB_ACTIVE) {
      return;
    }

    /* If there is not an ongoing transaction and the output queue contains
       data then a new transaction is started.*/
    if (!usbGetTransmitStatusI(&USBDrv, EP_DATA_IN_ID)) {
      if ((n = oqGetFullI(&UsbAu.oqueue)) > 0U) {
          Uart.PrintfI("n=%u\r", n);
        osalSysUnlock();

        usbPrepareQueuedTransmit(&USBDrv,
                EP_DATA_IN_ID,
                &UsbAu.oqueue, n);

        osalSysLock();
        (void) usbStartTransmitI(&USBDrv, EP_DATA_IN_ID);
      }
    }
}
#endif

struct SetupPkt_t {
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
};

/* ==== Setup Packet handler ====
 * true         Message handled internally.
 * false        Message not handled. */
bool OnSetupPkt(USBDriver *usbp) {
    SetupPkt_t *Setup = (SetupPkt_t*)usbp->setup;
//    Uart.PrintfI("%X %X %X %X %X\r", Setup->bmRequestType, Setup->bRequest, Setup->wValue, Setup->wIndex, Setup->wLength);
    if(Setup->bmRequestType == 0x01) { // Host2Device, standard, recipient=interface
        if(Setup->bRequest == USB_REQ_SET_INTERFACE) {
            if(Setup->wIndex == 1) {
                usbSetupTransfer(usbp, NULL, 0, NULL);
                // wValue contains alternate setting
                chSysLockFromISR();
                if(Setup->wValue == 1) {    // Transmission on
                    App.SignalEvtI(EVTMSK_START_LISTEN);
                    UsbAu.IsListening = true;
                }
                else {
                    App.SignalEvtI(EVTMSK_STOP_LISTEN);
                    UsbAu.IsListening = false;
                }
                chSysUnlockFromISR();
                return true;
            }
        }
    }
    return false;
}

void UsbAudio_t::SendBufI(uint8_t *Ptr, uint32_t Len) {
    if(!IsListening) return;
    // If the USB driver is not in the appropriate state then transactions must not be started
    if(usbGetDriverStateI(&USBDrv) != USB_ACTIVE) return;
    // If there is not an ongoing transaction and Len != 0
    if(!usbGetTransmitStatusI(&USBDrv, EP_DATA_IN_ID)) {
        if(Len > 0) {
            usbPrepareTransmit(&USBD1, EP_DATA_IN_ID, Ptr, Len);
            chSysLockFromISR();
            usbStartTransmitI(&USBD1, EP_DATA_IN_ID);
            chSysUnlockFromISR();
        }
    }
}

void UsbAudio_t::Connect() {
    usbDisconnectBus(&USBDrv);
    chThdSleepMilliseconds(1500);
    usbStart(&USBDrv, &UsbCfg);
    usbConnectBus(&USBDrv);
}
