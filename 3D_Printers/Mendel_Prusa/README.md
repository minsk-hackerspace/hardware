Первый 3д-принтер в хакерспейсе.
=================================

Страдал и продолжает страдать, но при применении прямых рук даёт вполне сносное качество печати.

 - сопло 0.5 мм
 - размер области печати 150х150х90
 - питание 12V 400W должно хватать


Прошивка и issues: https://github.com/minsk-hackerspace/Marlin

Конфиги для slic3r: https://github.com/minsk-hackerspace/slic3r-configs

Подробнее о конструкции: 
 - http://reprap.org/wiki/Prusa_Mendel_(iteration_2) 
 - http://3dtoday.ru/blogs/leonov-x/the-assembly-of-the-prusa-i2-connect-electronics/

Детальное руководство по решению проблем: https://3dpt.ru/page/faq


Как нарезать модель
=====================

 - Скачать [набор настроек HSPMi2_bundle.ini Slic3r](https://raw.githubusercontent.com/rskdif/slic3r-configs/master/prusa_mendel/bundles/HSPMi2_bundle.ini) для принтера Prusa Mendel i2
 	
	``` bash
 	wget https://raw.githubusercontent.com/rskdif/slic3r-configs/master/prusa_mendel/bundles/HSPMi2_bundle.ini
	```

 - Установить и запустить Slic3r ( http://slic3r.org/download ) 
 
	Я на debian делаю это так:

	``` bash
		sudo apt-get install fuse libgl1 libgl1-mesa-dev libglu1
		wget https://dl.slic3r.org/dev/linux/Slic3r-master-latest.tar.bz2
		tar -xvf Slic3r-master-latest.tar.bz2
		cd ./Slic3r
		./Slic3r --gui
	```

 - Загрузить в Slic3r набор настроек HSPMi2_bundle.ini (File -> Load Config Bundle...)
 - Перезапустить Slic3r
 - Загрузить свою модель (stl файл) в Slic3r (кнопка Add или File -> Open STL)
 - Выбрать настройки 
	```
	Print settings: HSPMi2_02layer_15infill
	      Filament: HSPMi2_PLA_1.75 (или ABS)
	       Printer: HSPMi2_by_Jekhor 
	```

 - Подкоректировать настройки и сгенерировать G-code


TODO:
=====
 - сделать руководство по нарезанию моделей в Cura
 - прикрутить вентилятор для охлаждения области печати для PLA
