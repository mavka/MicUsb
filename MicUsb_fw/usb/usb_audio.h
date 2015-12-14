/*
 * usb_audio.h
 *
 *  Created on: 05 сент. 2015 г.
 *      Author: Kreyl
 */

#ifndef USB_USB_AUDIO_H_
#define USB_USB_AUDIO_H_

#include "kl_buf.h"
#include "ch.h"

#define BUF_CNT     128

void onotify(io_queue_t *qp);

class UsbAudio_t {
private:
    int16_t Buf2Send[BUF_CNT];
public:
    bool IsListening = false;
    void Connect();
    void Disconnect();
    bool IsActive() { return false; }
    // Data
    void SendBufI(uint8_t *Ptr, uint32_t Len);

    void Put(int16_t AValue) {
        Buf.Put(AValue);
        CheckAndSend();
    }
    void CheckAndSend() {
        if(Buf.GetFullCount() >= 32) {
            chSysLock();
//            chSysLockFromISR();
            Buf.Get(Buf2Send, 32);
            SendBufI((uint8_t*)Buf2Send, 64);
//            Buf.Flush();
            chSysUnlock();
//            chSysUnlockFromISR();
        }
    }

    CircBufNumber_t<int16_t, BUF_CNT> Buf;
};

extern UsbAudio_t UsbAu;

#endif /* USB_USB_AUDIO_H_ */
