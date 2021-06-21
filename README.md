# SpeechEnhancement

## 介绍
语音增强模型 C++ libtorch部署  
模型源码见https://github.com/Nitin4525/SpeechEnhancement

## 预训练模型
百度网盘 链接: https://pan.baidu.com/s/1lVNOucGRDAYNlAczE4MO_g 提取码: 8bpg  

## 使用说明
### 下载libtorch  
官方地址：https://pytorch.org/get-started/locally/  
如果要使用libtorch调用已有模型，则下载时注意版本应与使用的Python中Pytorch版本一致（提供的预训练模型基于pytorch1.8.1）。下载后直接进行解压，得到libtorch文件夹。  
官方提供的编译完成的包仅支持x86或amd64，如果需要在嵌入式平台(ARM等)使用，需要下载源码自行编译。
### 编译CMake  
逐步执行，会在build文件夹下生成名为SpeechEnhancement的可执行文件
```
git clone https://gitee.com/Nitin525/speech-enhancement.git
cd speech-enhancement
mkdir build  
cd build/  
cmake ..  # 进行这一步之前需要修改CMakeLists.txt中的CMAKE_PREFIX_PATH，与libtorch的位置一致
make -j 4   # 进行这一步之前需要修改main.cpp中模型和wav文件的路径
```
### 运行
`/SpeechEnhancement`

## 对比测试
本程序在本人已有的设备上进行了测试，测试结果如下。  

| 计算硬件          | 内存    |  处理速度   |
| --------         | -----: |  :----:   |
| AMD R7-4800H     | 16G    |  0.218    |
| Intel i7-9700K   | 32G    |  0.886    |
| Broadcom BCM2711 |  8G    |  5.714    |  
处理速度=模型前向推理用时/文件总时长；实际程序运行占用内存为1.5GB。
