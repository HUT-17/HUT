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
## 仪器控制
汇川-工控机 安装后可通过GPIB、USB、以太网/LAN或串行总线RS232/RS458与各类仪器进行通信。

![Image text](https://www.ni.com/images/coreblock/instrumentcontrol_sm.jpg)

3轴伺服系统

### 连接和设置硬件
安装好软件和驱动程序后，即可开始连接和设置硬件。
![Image text]（https://image.baidu.com/search/detail?ct=503316480&z=&tn=baiduimagedetail&ipn=d&word=3%E8%BD%B4%E4%BC%BA%E6%9C%8D%E7%B3%BB%E7%BB%9F&step_word=&ie=utf-8&in=&cl=2&lm=-1&st=-1&hd=undefined&latest=undefined&copyright=undefined&cs=2582837982,3965805716&os=1117127670,2077733793&simid=3487378337,210711305&pn=12&rn=1&di=94270&ln=1307&fr=&fmq=1618476944259_R&ic=0&s=undefined&se=&sme=&tab=0&width=&height=&face=undefined&is=0,0&istype=2&ist=&jit=&bdtype=0&spn=0&pi=0&gsm=0&objurl=https%3A%2F%2Fgimg2.baidu.com%2Fimage_search%2Fsrc%3Dhttp%253A%252F%252F5b0988e595225.cdn.sohucs.com%252Fimages%252F20180930%252F88b6f74f486d4a49831411734d4d7cfe.jpeg%26refer%3Dhttp%253A%252F%252F5b0988e595225.cdn.sohucs.com%26app%3D2002%26size%3Df9999%2C10000%26q%3Da80%26n%3D0%26g%3D0n%26fmt%3Djpeg%3Fsec%3D1621068944%26t%3D20852ef9698f172b265d928d701ebd04&rpstart=0&rpnum=0&adpicid=0&force=undefined）
