# 关于《视觉引导涂胶机构的设计与实现》的说明
本毕业设计作品荣获-湖南工业大学-17级自动化毕业设计作品“特等奖”，其中团队成员包括：罗  琛 何智勇 温东胜 苏  鹏 

项目介绍：本组拟设计一款适用于工厂生产的视觉引导涂胶机，具体功能有：能正确的识别物体的各种形状，并在界面上描述识别到的图形；在已识别的物体上可以画出轨迹；按照画出的轨迹，控制三轴来完成点胶。

## 关于Labview视觉软件平台的搭建
### 事前准备
使用DAQ软件前，必须先安装应用程序开发环境和驱动程序。必须安装LabVIEW和NI-DAQmx驱动程序。如无随产品发运的原始安装光盘，请访问ni.com/downloads/products下载最新LabVIEW试用版和NI-DAQmx。
如正在更新现有系统，NI建议先对所有应用程序、项目及其他相关文件进行备份。

预计安装用时：8小时
### 选择平台
选择合适的环境平台，windows10、MAC、Linux。

![Image text](https://www.ni.com/images/coreblock/daq_family.gif)
### 安装LabVIEW

安装驱动程序前必须首先安装LabVIEW。请插入LabVIEW开发平台DVD，按照屏幕提示进行安装。如无随LabVIEW发运的原始光装光盘，可在线下载LabVIEW最新版本。
安装LabVIEW过程中如遇到问题，请参阅LabVIEW安装指南或LabVIEW安装疑难解答。
### 安装NI-DAQmx

安装LabVIEW及合适的模块、工具包后，请安装NI-DAQmx驱动程序。安装新NI硬件设备前必须先安装NI-DAQmx驱动程序，否则Windows无法检测到硬件。NI-DAQmx驱动程序的安装途径有三种：

NI Device Drivers DVD:如通过LabVIEW开发平台DVD安装设备驱动，程序将自动提示用户插入NI设备驱动程序DVD以完成安装。
NI-DAQmx DVD:所有NI DAQ设备均随附DVD光盘，包含了设备所需的驱动程序。
在线下载:去驱动程序和更新页下载最新NI-DAQmx驱动程序。
为确保所有依赖软件均安装，NI建议安装软件时选择典型安装选项。如弹出选择NI软件安装路径的提示，请选择与LabVIEW相同的根目录。



### 安装DAQ
DAQ设备用于测量各种来自传感器的电气和物理信号。NI DAQ硬件系列产品丰富，有即插即用式USB设备，也有多通道数的PXI系统。
![Image text](https://www.ni.com/images/coreblock/crio_embedded.gif)
### 安装CompactRIO
安装和配置NI CompactRIO，然后将系统配置为使用动态主机配置协议(DHCP)或静态IP地址。
### 安装VDM
这个是NI视觉所有的图像处理函数库，包提供在LabVIEW编程环境下使用多种图像处理以及识别的函数及功能，本文的圆形拟合即基于该工具包。
### 安装VAS
用于捕获图像的一系列驱动程序。工业相机，调用的是DLL，需要使用IMAQdx、IMAQ等驱动。安装后，在VBAI、VDM中的视觉助手（Vision Assistant)、MAX中都是可以直接从相机中采集图像。
### 安装VBAI
用于自动检测的视觉生成器。NI Vision Developments Module（视觉开发包VDM）开发的一款功能比较强大的应用程序，需要安装。
## 仪器控制
汇川-工控机 安装后可通过GPIB、USB、以太网/LAN或串行总线RS232/RS458与各类仪器进行通信。

![Image text](https://www.ni.com/images/coreblock/instrumentcontrol_sm.jpg)


### 连接和设置硬件
安装好软件和驱动程序后，即可开始连接和设置硬件。



