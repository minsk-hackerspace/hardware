Кардридер IronLogic Z-2 USB:
```
$ dmesg
[134226.883092] usb 3-1: new full-speed USB device number 3 using uhci_hcd
[134227.072115] usb 3-1: New USB device found, idVendor=0403, idProduct=1234
[134227.072120] usb 3-1: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[134227.072124] usb 3-1: Product: USB IronLogic RFID Adapter
[134227.072126] usb 3-1: Manufacturer: ILogic
[134227.072129] usb 3-1: SerialNumber: IL03E3BH
```

Определяется, как USB девайс. Если карточку поднести, то пикает и
мигает зелёненьким, как положено, но как получить с этого устройства
информацию на Linux пока совершенно непонятно.

