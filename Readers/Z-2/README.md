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

![Z-2 USB](Z-2_USB.jpg)

Определяется, как USB девайс. Если карточку поднести, то пикает и
мигает зелёненьким, как положено, но как получить с этого устройства
информацию на Linux пока совершенно непонятно.

Надо понять вообще, что это за устройство. USB девайсы обычно зашивают
всё необходимую информацию, которую можно опросить через `lsusb`:
```
$ sudo lsusb -v -d 0403:

Bus 003 Device 002: ID 0403:1234 Future Technology Devices International, Ltd IronLogic RFID Adapter [Z-2 USB]
Device Descriptor:
  bLength                18
  bDescriptorType         1
  bcdUSB               2.00
  bDeviceClass            0 
  bDeviceSubClass         0 
  bDeviceProtocol         0 
  bMaxPacketSize0         8
  idVendor           0x0403 Future Technology Devices International, Ltd
  idProduct          0x1234 IronLogic RFID Adapter [Z-2 USB]
  bcdDevice            6.00
  iManufacturer           1 ILogic
  iProduct                2 USB IronLogic RFID Adapter
  iSerial                 3 IL03E3BH
  bNumConfigurations      1
  Configuration Descriptor:
    bLength                 9
    bDescriptorType         2
    wTotalLength           32
    bNumInterfaces          1
    bConfigurationValue     1
    iConfiguration          0 
    bmAttributes         0xa0
      (Bus Powered)
      Remote Wakeup
    MaxPower              132mA
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        0
      bAlternateSetting       0
      bNumEndpoints           2
      bInterfaceClass       255 Vendor Specific Class
      bInterfaceSubClass    255 Vendor Specific Subclass
      bInterfaceProtocol    255 Vendor Specific Protocol
      iInterface              2 USB IronLogic RFID Adapter
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x81  EP 1 IN
        bmAttributes            2
          Transfer Type            Bulk
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0040  1x 64 bytes
        bInterval               0
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x02  EP 2 OUT
        bmAttributes            2
          Transfer Type            Bulk
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0040  1x 64 bytes
        bInterval               0
can't get device qualifier: Resource temporarily unavailable
can't get debug descriptor: Resource temporarily unavailable
Device Status:     0x0000
  (Bus Powered)
```

С помощью Google по ключевым параметрам можно понять, что
устройство поддерживает USB 2.0 (`bcdUSB`), версия самого
устройства 6.0 (`bcdDevice`), и относится оно к
[неопределённому классу](https://en.wikipedia.org/wiki/USB#Device_classes)
(`bDeviceClass`). По идее класс должен быть или 02h или 0Bh,
но видно не судьба. Впрочем, спецификация говорит, что при
00h смотреть надо на `Interface Descriptor`.

`Interface Descriptor` совсем не радует. Он показывает, что
у нас есть 2 точки - один вход принимающий пакеты по 64 байта,
и один выход такого же диаметра. `bInterfaceClass` и френды
указывают на то, что перед нами велосипед от производителя с
оригинальным протоколом, и, скорее всего, драйверами. Где их
брать и как вообще с ними взаимодействовать пока совершенно
непонятно.

Ссылки:

 - https://wiki.debian.org/HowToIdentifyADevice/USB
 - http://www.beyondlogic.org/usbnutshell/usb5.shtml

#### Часть вторая - вендор

Знающие люди в рассылке хакерспейса подсказали, что ядро
подгружает нужный модуль для общения с устройством по его
идентификаторам `idVendor:idProduct`, которые в этом случае
имеют значение `0403:1234`:

    idVendor           0x0403 Future Technology Devices International, Ltd
    idProduct          0x1234 IronLogic RFID Adapter [Z-2 USB]

Получается, что IronLogic сделала продукт на основе чипа
от FTDI. Можно поискать, какой модуль соответствует этим
параметрам.

Известные айдишники с именами модулей храняться в файле
``/lib/modules/`uname -r`/modules.alias``, где `uname -r`
будет именем текущего ядра. Файл генерируется автоматом,
опрашивая модули на предмет того, что они поддерживают.
Формат строк в файле `v0403p1234`, но таким образом он
ничего не находит, зато поиск по вендору даёт результат:

    $ less /lib/modules/`uname -r`/modules.alias | grep v0403
    ...    
    alias usb:v0403pFF00d*dc*dsc*dp*ic*isc*ip*in* ftdi_sio
    alias usb:v0403pF60Bd*dc*dsc*dp*ic*isc*ip*in* ftdi_sio
    ...

Можно подгрузить модуль `ftdi_sio` вручную и посмотреть,
что получится.

    $ sudo modprobe ftdi_sio
    $ dmesg
    ...
    [110607.581273] usbcore: registered new interface driver ftdi_sio
    [110607.581389] usbserial: USB Serial support registered for FTDI USB Serial Device
    
Драйвер загружен, но USB устройство он не видит, т.к. в логе
`dmesg` больше ничего нет. По идее `usbserial` должен был
создать `ttyUSB0` интерфейс, но его тоже нет:

    $ sudo ls -la /dev/ttyU*
    ls: cannot access '/dev/ttyU*': No such file or directory

На StackExchange откопалась
[нужная информация](http://unix.stackexchange.com/questions/67936/attaching-usb-serial-device-with-custom-pid-to-ttyusb0-on-embedded). Драйверам `usb` устройств можно добавлять новые id
для распознавания без пересборки драйвера:

    $ sudo su -c "echo 0403 1234 > /sys/bus/usb-serial/drivers/ftdi_sio/new_id"

Подробное описание `new_id` нашлось в документации Linux Kernel
https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-bus-usb

    $ dmesg
    ...
    [143066.669704] ftdi_sio 7-2:1.0: FTDI USB Serial Device converter detected
    [143066.670047] usb 7-2: Detected FT232RL
    [143066.673310] usb 7-2: FTDI USB Serial Device converter now attached to ttyUSB0

#### Часть третья - протокол

Сначала надо проверить, что драйвер загружен. Это можно сделать
из `user space` проверив, что `ftdi_sio` присутствует в списке
`/proc/modules`. Если драйвера нет, его потребуется `root`,
чтобы его включить:

    $ sudo modprobe ftdi_sio
    $ cat /proc/modules | grep ftdi_sio
    ftdi_sio 53248 0 - Live 0x0000000000000000
    usbserial 53248 1 ftdi_sio, Live 0x0000000000000000

`0` говорит о том, что модуль загружен, но не используется
https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/5/html/Deployment_Guide/s2-proc-modules.html

Затем надо скормить драйверу `vid pid` нового устройства,
чтобы он его опознал и добавил интерфейс `/dev/ttyUSBx`. При
этом `/proc/modules` показывает, что драйвер по прежнему не
используется.

    $ sudo su -c "echo 0403 1234 > /sys/bus/usb-serial/drivers/ftdi_sio/new_id"
    $ dmesg
    ...
    [125149.733270] usb 7-2: Detected FT232RL
    [125149.736195] usb 7-2: FTDI USB Serial Device converter now attached to ttyUSB0
    $ cat /proc/modules | grep ftdi_sio
    ftdi_sio 53248 0 - Live 0x0000000000000000
    usbserial 53248 1 ftdi_sio, Live 0x0000000000000000
    
Для работы с устройством нужен терминал, запущенный из-под
`root`. Послав `i` (выдрано из мануала) на скорости 9600
8-N-1 "no flow control" (оттуда же), можно получить
информацию об устройстве:

    $ sudo screen /dev/ttyUSB0

    USB Z-2 ADAPTER S/N:48715 [0301]
    CopyRight (C)2005 IronLogic, Saint-Petersburg,RUSSIA
    www.ironlogic.ru ph. +7(095)78-77066,+7(812)542-04-80
    Please send Email: marketing@ironlogic.ru
    Software version: 0001

Больше с ним ничего нельзя сделать. При поднесении меток,
автоматически отсылается их стандарт и номер в формате
Wiegand-26:

    Mifare[625BD85D] 091,55389 1K (0004,08)
    No card
    Mifare[425BDE5D] 091,56925 1K (0004,08)
    No card

`No card` появляется, если метку убрать.
