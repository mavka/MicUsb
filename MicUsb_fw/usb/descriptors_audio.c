/*
 * descriptors_audio.c
 *
 *  Created on: 04 09 2015 ã.
 *      Author: Kreyl
 */

#include "ch.h"
#include "hal.h"
#include "descriptors_audio.h"
#include "board.h"

#if 1 // ==== Constants (not to change) ====
#define EP_DIR_IN           0x80
#define EP_DIR_OUT          0x00
// Descriptor-type endpoint codes
#define EP_TYPE_CONTROL     0x00
#define EP_TYPE_ISOCHRONOUS 0x01
#define EP_TYPE_BULK        0x02
#define EP_TYPE_INTERRUPT   0x03
#endif

#if 1 // ==== USB Device Descriptor ====
static const uint8_t DeviceDescriptorData[18] = {
  USB_DESC_DEVICE       (0x0200,        // bcdUSB (2.0).
                         0x00,          // bDeviceClass: Device defined at Interface level
                         0x00,          // bDeviceSubClass: Unused
                         0x00,          // bDeviceProtocol: Unused
                         64,            // bMaxPacketSize.
                         USBD_VID,      // idVendor (ST).
                         USBD_PID,      // idProduct.
                         0x0200,        // bcdDevice.
                         1,             // iManufacturer.
                         2,             // iProduct.
                         3,             // iSerialNumber.
                         1)             // bNumConfigurations.
};

// Device Descriptor wrapper
static const USBDescriptor DeviceDescriptor = {
  sizeof DeviceDescriptorData,
  DeviceDescriptorData
};
#endif

#if 1 // ==== Configuration Descriptor tree ====
#define USB_CONFIG_POWER_MA(mA) ((mA) >> 1)
#define CS_INTERFACE            0x24    // Descriptor type
#define USB_DESC_TRIBYTE(w)             \
        (uint8_t)((w) & 255U),          \
        (uint8_t)(((w) >> 8)  & 255U),  \
        (uint8_t)(((w) >> 16) & 255U)

#define CFG_TOTAL_LEN           100
static const uint8_t ConfigurationDescriptorData[CFG_TOTAL_LEN] = {
    // ==== Configuration Descriptor ====
    USB_DESC_CONFIGURATION(CFG_TOTAL_LEN, // wTotalLength
                         0x02,          // bNumInterfaces
                         0x01,          // bConfigurationValue
                         0,             // iConfiguration
                         0x80,          // bmAttributes (bus powered)
                         USB_CONFIG_POWER_MA(100)   // bMaxPower
                         ),

    // === Standard AC Interface Descriptor ====
    USB_DESC_INTERFACE  (0x00,          // bInterfaceNumber.
                         0x00,          // bAlternateSetting.
                         0x00,          // bNumEndpoints.
                         0x01,          // bInterfaceClass: AUDIO
                         0x01,          // bInterfaceSubClass: AUDIO_CONTROL
                         0x00,          // bInterfaceProtocol: unused
                         0),            // iInterface

    // ==== Class-specific AC Interface ====
    USB_DESC_BYTE        (0x09),    // bLength
    USB_DESC_BYTE        (CS_INTERFACE), // bDescriptorType (CS_INTERFACE)
    USB_DESC_BYTE        (0x01),    // bDescriptorSubtype: HEADER
    USB_DESC_WORD        (0x0100),  // bcdADC: Revision of class specification - 1.0
    USB_DESC_WORD        (0x001E),  // wTotalLength: Total size of class specific descriptors
    USB_DESC_BYTE        (0x01),    // bInCollection: Number of streaming interfaces
    USB_DESC_BYTE        (0x01),    // baInterfaceNr(1): AudioStreaming interface 1 belongs to this AudioControl interface

    // ==== Input Terminal ====
    /* This descriptor describes the Input Terminal that represents the microphone capsule, followed by the AD
     * converter. The resulting digital audio stream leaves the Input Terminal through the single Output Pin.
     * The audio channel cluster contains a single logical channel (bNrChannels=1) and there is no spatial
     * location associated with this mono channel (wChannelConfig=0x0000). */
    USB_DESC_BYTE        (0x0C),    // bLength
    USB_DESC_BYTE        (CS_INTERFACE), // bDescriptorType (CS_INTERFACE)
    USB_DESC_BYTE        (0x02),    // bDescriptorSubtype: INPUT_TERMINAL subtype
    USB_DESC_BYTE        (0x01),    // bTerminalID: ID of this Input Terminal
    USB_DESC_WORD        (0x0201),  // wTerminalType: Terminal is Microphone
    USB_DESC_BYTE        (0x00),    // bAssocTerminal: No association
    USB_DESC_BYTE        (0x01),    // bNrChannels: One channel
    USB_DESC_WORD        (0x0000),  // wChannelConfig: Mono sets no position bits
    USB_DESC_BYTE        (0x00),    // iChannelNames: Unused
    USB_DESC_BYTE        (0x00),    // iTerminal: Unused

    // ==== Output Terminal ====
    /* This descriptor describes the Output Terminal that represents the USB pipe to the Host PC. Its Input Pin
     * is directly connected to the Output Pin of the Input Terminal (bSourceID= Input Terminal ID). */
    USB_DESC_BYTE        (0x09),    // bLength
    USB_DESC_BYTE        (CS_INTERFACE), // bDescriptorType (CS_INTERFACE)
    USB_DESC_BYTE        (0x03),    // bDescriptorSubtype: OUTPUT_TERMINAL subtype
    USB_DESC_BYTE        (0x02),    // bTerminalID: ID of this Output Terminal
    USB_DESC_WORD        (0x0101),  // wTerminalType: USB Streaming
    USB_DESC_BYTE        (0x00),    // bAssocTerminal: No association
    USB_DESC_BYTE        (0x01),    // bSourceID: From Input Terminal
    USB_DESC_BYTE        (0x00),    // iTerminal: Unused

    // ==== AudioStreaming Interface Descriptor ==== has two possible alternate settings
    // Zero-bandwidth Alternate Setting 0
    USB_DESC_INTERFACE  (0x01,          // bInterfaceNumber.
                         0x00,          // bAlternateSetting.
                         0x00,          // bNumEndpoints.
                         0x01,          // bInterfaceClass: AUDIO
                         0x02,          // bInterfaceSubClass: AUDIO_STREAMING
                         0x00,          // bInterfaceProtocol: unused
                         0),            // iInterface.
    // Operational Alternate Setting 1
    USB_DESC_INTERFACE  (0x01,          // bInterfaceNumber.
                         0x01,          // bAlternateSetting.
                         0x01,          // bNumEndpoints.
                         0x01,          // bInterfaceClass: AUDIO
                         0x02,          // bInterfaceSubClass: AUDIO_STREAMING
                         0x00,          // bInterfaceProtocol: unused
                         0),            // iInterface.

    // ==== Class-specific AS General Interface ====
    USB_DESC_BYTE        (0x07),    // bLength
    USB_DESC_BYTE        (CS_INTERFACE), // bDescriptorType (CS_INTERFACE)
    USB_DESC_BYTE        (0x01),    // bDescriptorSubtype: GENERAL subtype
    USB_DESC_BYTE        (0x02),    // bTerminalLink: Unit ID of the Output Terminal
    USB_DESC_BYTE        (0x01),    // bDelay: Interface delay
    USB_DESC_WORD        (0x0001),  // wFormatTag: PCM Format

    // ==== Type I Format Type ====
    USB_DESC_BYTE        (0x0B),    // bLength
    USB_DESC_BYTE        (CS_INTERFACE), // bDescriptorType (CS_INTERFACE)
    USB_DESC_BYTE        (0x02),    // bDescriptorSubtype: FORMAT_TYPE subtype
    USB_DESC_BYTE        (0x01),    // bFormatType: FORMAT_TYPE_I
    USB_DESC_BYTE        (0x01),    // bNrChannels: One channel
    USB_DESC_BYTE        (0x02),    // bSubFrameSize: Two bytes per audio subframe
    USB_DESC_BYTE        (16),      // bBitResolution: 16 bits per sample
    USB_DESC_BYTE        (0x01),    // bSamFreqType: One frequency supported
#if SAMPLING_FREQ_KHZ == 8
    USB_DESC_TRIBYTE     (8000),    // 8000Hz
#else
    USB_DESC_TRIBYTE     (16000),   // 16000Hz
#endif

    // ==== Standard Endpoint Descriptor ====
    USB_DESC_BYTE        (0x09),    // bLength
    USB_DESC_BYTE        (0x05),    // bDescriptorType (ENDPOINT)
    USB_DESC_BYTE        (EP_DATA_IN_ID | EP_DIR_IN), // bEndpointAddress
    USB_DESC_BYTE        (0x01),    // bmAttributes: Isochronous, not shared
    USB_DESC_WORD        (64),      // wMaxPacketSize: 64
    USB_DESC_BYTE        (0x01),    // bInterval: One packet per frame
    USB_DESC_BYTE        (0x00),    // bRefresh: Unused
    USB_DESC_BYTE        (0x00),    // bSynchAddress: Unused

    // Class-specific Isochronous Audio Data Endpoint Descriptor
    USB_DESC_BYTE        (0x07),    // bLength
    USB_DESC_BYTE        (0x25),    // bDescriptorType (CS_ENDPOINT)
    USB_DESC_BYTE        (0x01),    // bDescriptorSubtype: GENERAL subtype
    USB_DESC_BYTE        (0x00),    // bmAttributes: No sampling frequency control, no pitch control, no packet padding
    USB_DESC_BYTE        (0x00),    // bLockDelayUnits: Unused
    USB_DESC_WORD        (0x0000),  // wLockDelay: Unused
};

// Configuration Descriptor wrapper.
static const USBDescriptor ConfigurationDescriptor = {
    sizeof ConfigurationDescriptorData,
    ConfigurationDescriptorData
};
#endif

#if 1 // ==== Strings ====
//Macro to calculate the Unicode length of a string with a given number of Unicode characters
#define USB_STRING_LEN(UnicodeChars)      (2 + ((UnicodeChars) << 1))

typedef struct {
    uint8_t  bLength;                // Size of the descriptor, in bytes
    uint8_t  bDescriptorType;        // Type of the descriptor, given by the specific class
    uint16_t  bString[];
} __attribute__ ((__packed__)) StringDescriptor_t;

// U.S. English language identifier
static const uint8_t LanguageString[] = {
        USB_DESC_BYTE (4),      // bLength
        USB_DESC_BYTE (USB_DESCRIPTOR_STRING),  // bDescriptorType
        USB_DESC_WORD (0x0409)  // wLANGID (U.S. English)
};

// Vendor string
static const uint8_t VendorString[] = {
        USB_DESC_BYTE (USB_STRING_LEN(8)),
        USB_DESC_BYTE (USB_DESCRIPTOR_STRING),  // bDescriptorType
        USB_DESC_WORD('O'),
        USB_DESC_WORD('s'),
        USB_DESC_WORD('t'),
        USB_DESC_WORD('r'),
        USB_DESC_WORD('a'),
        USB_DESC_WORD('n'),
        USB_DESC_WORD('n'),
        USB_DESC_WORD('a'),
};

// Device Description string
static const uint8_t ProductString[] = {
        USB_DESC_BYTE (USB_STRING_LEN(11)),
        USB_DESC_BYTE (USB_DESCRIPTOR_STRING),  // bDescriptorType
        USB_DESC_WORD('A'),
        USB_DESC_WORD('u'),
        USB_DESC_WORD('d'),
        USB_DESC_WORD('i'),
        USB_DESC_WORD('o'),
        USB_DESC_WORD(' '),
        USB_DESC_WORD('I'),
        USB_DESC_WORD('n'),
        USB_DESC_WORD('p'),
        USB_DESC_WORD('u'),
        USB_DESC_WORD('t'),
};

// Serial number string
static const uint8_t SerialNumber[] = {
        USB_DESC_BYTE (USB_STRING_LEN(3)),
        USB_DESC_BYTE (USB_DESCRIPTOR_STRING),  // bDescriptorType
        USB_DESC_WORD('1'),
        USB_DESC_WORD('2'),
        USB_DESC_WORD('3'),
};


// Strings wrappers array
static const USBDescriptor DescStrings[] = {
        {sizeof LanguageString, LanguageString},
        {sizeof VendorString,   VendorString},
        {sizeof ProductString,  ProductString},
        {sizeof SerialNumber,   SerialNumber}
};
#endif

//extern void PrintfC(const char *format, ...);

// Handles the GET_DESCRIPTOR callback. All required descriptors must be handled here
const USBDescriptor *GetDescriptor(USBDriver *usbp, uint8_t dtype, uint8_t dindex, uint16_t lang) {
//    PrintfC("\rGetDsc: t=%u; i=%u", dtype, dindex);
    switch(dtype) {
        case USB_DESCRIPTOR_DEVICE:
            return &DeviceDescriptor;
            break;
        case USB_DESCRIPTOR_CONFIGURATION:
            return &ConfigurationDescriptor;
            break;
        case USB_DESCRIPTOR_STRING:
            if(dindex < 4) {
//                PrintfC("\rStr: %u  %A", DescStrings[dindex].ud_size, DescStrings[dindex].ud_string, DescStrings[dindex].ud_size, ' ');
                return &DescStrings[dindex];
            }
    } // switch
    return NULL;
}

