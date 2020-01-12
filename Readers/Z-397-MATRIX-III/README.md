Кардридер IronLogic MATRIX-III и конвертер (RS-422/RS-485 в USB) Z-397:

```
$ dmesg
[69910.152034] usb 7-2: new full-speed USB device number 4 using uhci_hcd
[69910.370058] usb 7-2: New USB device found, idVendor=0403, idProduct=1235
[69910.370063] usb 7-2: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[69910.370066] usb 7-2: Product: USB <-> RS-485/422
[69910.370069] usb 7-2: Manufacturer: Pulsar-Telecom
[69910.370071] usb 7-2: SerialNumber: IL01Y2DI
```

У кардридера MATRIX-III нет USB интерфейса. Можно подключить его по
COM (RS-232). На ноутах таких портов уже не делают, поэтому нужен
конвертер.

IronLogic Z-397 - это не COM конвертер. У MATRIX-III есть ещё похожий
способ подключения RS-485 - если COM (RS-232) работает на 5 метрах, то
RS-485 держит до 1.2 километра. Для переключения Z-397 в режим работы
по RS-485, надо переключатели 1 и 2 под крышкой перевести в режим ON.
Заодно 3 и 4 туда же - 3 говорит использовать полудуплекс, и на другой
режим RS-485 не рассчитан, а 4 ставит заглушку, чтобы сигнал не
утекал =) потому что конвертер последний в линии и после него никаких
устройств нет.

Исследование совместимости IronLogic с Linux описано в
[../Z-2/README.md](../Z-2/README.md)