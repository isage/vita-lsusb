#include <psp2/kernel/clib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usbd.h"

#define dump(...) do { \
    sceClibPrintf(__VA_ARGS__); \
    fprintf(fp, __VA_ARGS__); \
} while (0);


int main(int argc, char *argv[]) {
    SceUID uid;
    SceUsbdDeviceInfo info[8];

    sceUsbdInit(&uid);

    sceUsbdGetDeviceList(uid, 8, (SceUsbdDeviceInfo *)&info);

    FILE* fp = fopen("ux0:/data/lsusb.txt","wb");

    static const char * const typeattr[] = {
        "Control",
        "Isochronous",
        "Bulk",
        "Interrupt"
    };
    static const char * const syncattr[] = {
        "None",
        "Asynchronous",
        "Adaptive",
        "Synchronous"
    };
    static const char * const usage[] = {
        "Data",
        "Feedback",
        "Implicit feedback Data",
        "(reserved)"
    };

    int i = 0;
    while (i < 8 && info[i].device_num != 0xffffffff) {
        unsigned int device_id = (info[i].port << 16) + info[i].device_num;
        int size = sceUsbdGetDescriptorSize(uid, device_id);

        uint8_t* descr = malloc(size);

        sceUsbdGetDescriptor(uid, device_id,  descr, size);
        int speed;
        sceUsbdGetDeviceSpeed(uid, device_id,  &speed);

        dump("Bus %x Device %x (%s):\n", info[i].port, info[i].device_num, (speed == 0) ? "Low-speed" : ((speed == 1) ? "High-speed" : "Full-speed") );

        uint8_t *ptr = descr;
        while (ptr - descr < size)
        {
            switch(ptr[1]) {
                case USB_DESCRIPTOR_DEVICE:
                    {
                        USB_DEVICE_DESCRIPTOR *d = (USB_DEVICE_DESCRIPTOR*)ptr;
                        ptr += ptr[0];
                        dump("Device Descriptor:\n");
                        dump("  bLength:            0x%02x\n", d->bLength);
                        dump("  bDescriptorType:    0x%02x\n", d->bDescriptorType);
                        dump("  bcdUSB:             %2x.%02x\n", d->bcdUSB >> 8, d->bcdUSB & 0xff);
                        dump("  bDeviceClass:       0x%02x\n", d->bDeviceClass);
                        dump("  bDeviceSubClass:    0x%02x\n", d->bDeviceSubClass);
                        dump("  bDeviceProtocol:    0x%02x\n", d->bDeviceProtocol);
                        dump("  bMaxPacketSize0:    0x%02x\n", d->bMaxPacketSize0);
                        dump("  idVendor:           0x%04x\n", d->idVendor);
                        dump("  idProduct:          0x%04x\n", d->idProduct);
                        dump("  bcdDevice:          %2x.%02x\n", d->bcdDevice >> 8, d->bcdDevice & 0xff);
                        dump("  iManufacturer:      0x%02x\n", d->iManufacturer);
                        dump("  iProduct:           0x%02x\n", d->iProduct);
                        dump("  iSerialNumber:      0x%02x\n", d->iSerialNumber);
                        dump("  bNumConfigurations: 0x%02x\n", d->bNumConfigurations);

                        break;
                    }
                case USB_DESCRIPTOR_CONFIGURATION:
                    {
                        USB_CONFIGURATION_DESCRIPTOR *d = (USB_CONFIGURATION_DESCRIPTOR*)ptr;
                        ptr += ptr[0];
                        dump("  Configuration Descriptor:\n");
                        dump("    bLength:              0x%02x\n", d->bLength);
                        dump("    bDescriptorType:      0x%02x\n", d->bDescriptorType);
                        dump("    wTotalLength:         0x%04x\n", d->wTotalLength);
                        dump("    bNumInterfaces:       0x%02x\n", d->bNumInterfaces);
                        dump("    bConfigurationValue:  0x%02x\n", d->bConfigurationValue);
                        dump("    iConfiguration:       0x%02x\n", d->iConfiguration);
                        dump("    bmAttributes:         0x%02x\n", d->bmAttributes);
                        if (!(d->bmAttributes & 0x80))
                        {
                            dump("      (Missing must-be-set bit!)\n");
                        }
                        if (d->bmAttributes & 0x40)
                        {
                            dump("      Self Powered\n");
                        }
                        else
                        {
                            dump("      (Bus Powered)\n");
                        }
                        if (d->bmAttributes & 0x20)
                        {
                            dump("      Remote Wakeup\n");
                        }
                        if (d->bmAttributes & 0x10)
                        {
                            dump("      Battery Powered\n");
                        }
                        dump("    bMaxPower:            0x%04x (%5umA)\n", d->bMaxPower, d->bMaxPower * 2);
                        break;
                    }
                case USB_DESCRIPTOR_INTERFACE:
                    {
                        USB_INTERFACE_DESCRIPTOR *d = (USB_INTERFACE_DESCRIPTOR*)ptr;
                        ptr += ptr[0];
                        dump("    Interface Descriptor:\n");
                        dump("      bLength:                 0x%02x\n", d->bLength);
                        dump("      bDescriptorType:         0x%02x\n", d->bDescriptorType);
                        dump("      bInterfaceNumber:        0x%04x\n", d->bInterfaceNumber);
                        dump("      bAlternateSetting:       0x%02x\n", d->bAlternateSetting);
                        dump("      bNumEndpoints:           0x%02x\n", d->bNumEndpoints);
                        dump("      bInterfaceClass:         0x%02x\n", d->bInterfaceClass);
                        dump("      bInterfaceSubClass:      0x%02x\n", d->bInterfaceSubClass);
                        dump("      bInterfaceProtocol:      0x%02x\n", d->bInterfaceProtocol);
                        dump("      iInterface:              0x%02x\n", d->iInterface);
                        break;
                    }
                case USB_DESCRIPTOR_ENDPOINT:
                    {
                        USB_ENDPOINT_DESCRIPTOR *d = (USB_ENDPOINT_DESCRIPTOR*)ptr;
                        ptr += ptr[0];
                        dump("      Endpoint Descriptor:\n");
                        dump("        bLength:                 0x%02x\n", d->bLength);
                        dump("        bDescriptorType:         0x%02x\n", d->bDescriptorType);
                        dump("        bEndpointAddress:        0x%02x  EP %u %s\n", d->bEndpointAddress, d->bEndpointAddress & 0x0f, (d->bEndpointAddress & 0x80) ? "IN" : "OUT");

                        dump("        bmAttributes:            0x%02x\n", d->bmAttributes);
                        dump("          Transfer Type:         %s\n", typeattr[d->bmAttributes & 3]);
                        dump("          Synch Type:            %s\n", syncattr[(d->bmAttributes >> 2) & 3]);
                        dump("          Usage Type:            %s\n", usage[(d->bmAttributes >> 4) & 3]);

                        dump("        wMaxPacketSize:          0x%02x\n", d->wMaxPacketSize);
                        dump("        bInterval:               0x%02x\n", d->bInterval);
                        break;
                    }
                default:
                    dump("unhandled dtype: %x\n", ptr[1]);
                    ptr += ptr[0];
                    break;
            };
        }
        dump("\n\n", ptr[1]);

        free(descr);
        i++;
    }

    fclose(fp);

    sceUsbdEnd(uid);

    return 0;
}
