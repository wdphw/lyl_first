# 智能家居各个模块部分

gec6818  stm32 zigbee esp32


## Build the project

进入GEC6818
```
cd build 
cmake -D CMAKE_C_COMPILER=/usr/local/5.4.0/bin/arm-linux-gcc ..
make -j

```


## Run the demo application

```
cd GEC6818
cd bin
./lyl_project
```

## Notes

进入stm32文件夹打开keil烧录到stm32
进入esp32打开arduino烧录到esp32
进入zigbee打开IAR烧录到cc2530
