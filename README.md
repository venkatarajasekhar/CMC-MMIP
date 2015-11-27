# Mathematical Methods of Image Processing
Репозиторий с заданиями по курсу "Современные методы обработки изображений"

## Bmp I/O
Модуль для чтения и записи файлов в формате ```bmp```.  
Поддерживается чтение изображений без компрессии, имеющие битности 8, 24, 32.

## Задание 1
В рамках данного задания был реализован следующий функционал:  
1. Инверсия значений пикселей изображения  
2. Отражение изображения по вертикали и по горизонтали  
3. Поворот изображений по и против часовой стрелки на 90, 180 и 270 градусов  
4. Фильтры, подсвечивающие контуры: фильтры Превитта, Собеля, Робертса  
5. Медианная фильтрация с квадратным окном произвольного размера  
6. Свёртка с фильтром Гаусса с произвольным выбором параметра — радиуса σ  
7. Вычисление модуля градиента как корень из суммы квадратов свёрток с первой производной фильтра Гаусса по горизонтали и вертикали  
8. Поворот изображения на произвольный угол с использованием билинейной интерполяции для устранения эффекта ступенчатости при повороте изображений с резкими контурами  
9. Фильтр Габора с произвольными параметрами  
10. Обнаружение сосудов на изображениях глазного дна с помощью фильтров Габора  

**Usage:**  
```%programname% (input_image) (output_image) (command) [parameters...]```  

**Список команд:**  
* `invert`      Инверсия пикселей  
* `mirror {x|y}`        Отражение по горизонтали или по вертикали, в зависомсти от указанного параметра  
* `rotate {cw|ccw} (angle)`     Поворот по или против часовой стрелки на заданное количество градусов  
* `prewitt {x|y}`       Фильтр Превитта, обнаруживающий горизонтальные или вертикальные контуры  
* `sobel {x|y}`     Фильтр Собеля, обнаруживающий горизонтальные или вертикальные контуры  
* `roberts {1|2}`      Фильтр Робертса, параметр — выбор диагонали  
* `median (rad)`        Медианная фильтрация, параметр rad — целочисленный *радиус* фильтра  
* `gauss (sigma)`       Фильтр Гаусса, параметр sigma — вещественный параметр фильтра  
* `gradient (sigma)`        Модуль градиента  
* `gabor (sigma) (gamma) (theta) (lambda) (psi)`      Свёртка с фильтром Габора (вещественной частью), углы задаются в градусах  
* `vessels (sigma)`     Обнаружение сосудов заданного масштаба  

Релиз этого задания можно скачать [здесь](https://github.com/igormunkin/MMIP/releases/tag/v0.1)
## Задание 2
В рамках данного задания был реализован следующий функционал:  
1. Увеличение изображений в целое число раз с помощью билинейной интерполяции  
2. Увеличение изображений в целое число раз с помощью бикубической интерполяции  
3. Понижение разрешения изображений в целое число раз  
4. Вычисление метрик сравнения изображений: MSE и PSNR  
5. Вычисление метрик сравнения изображений: SSIM и MSSIM  
6. Увеличение изображений в вещественное число раз с помощью билинейной интерполяции  
7. Увеличение изображений в вещественное число раз с помощью бикубической интерполяции  
8. Понижение разрешения изображений в вещественное число раз  

**Usage:**  
```%programname% (input_image) (output_image) (command) [parameters...]```  
```%programname% (input_image_1) (input_image_2) (command)```  

**Список команд:**  
* `up_bilinear {s}`     Увеличение с помощью билинейной интерполяции в s раз  
* `up_bicubic {s}`       Увеличение с помощью бикубической интерполяции в s раз  
* `downsample {s}`       Уменьшение в s раз  
* `metric {mse|psnr|ssim|mssim}`     Вычисление метрики между двумя входными изображениями, результат выводится числом на экран  

Релиз этого задания можно скачать [здесь](https://github.com/igormunkin/MMIP/releases/tag/v0.2)
