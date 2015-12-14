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
    uint8_t IBuf[BUF_CNT];

public:
    void Init() { chOQObjectInit(&oqueue, IBuf, BUF_CNT, onotify, NULL); }
    bool IsListening = false;
    void Connect();
    void Disconnect();
    bool IsActive() { return false; }
    // Data
    void SendBufI(uint8_t *Ptr, uint32_t Len);

    output_queue_t oqueue;

    void Put(int16_t AValue) {
        chOQWriteTimeout(&oqueue, (uint8_t*)&AValue, 2, TIME_INFINITE);
    }

//    CircBufNumber_t<int16_t, BUF_CNT> Buf;
};

extern UsbAudio_t UsbAu;

#endif /* USB_USB_AUDIO_H_ */
