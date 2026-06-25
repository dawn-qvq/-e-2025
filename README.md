# 项目简介 | Project Overview

本项目为 **2025 全国大学生电子设计竞赛（E题）——简易自行瞄准装置** 的开源实现。

系统采用 **STM32F103C8T6 + MSPM0G3507 双MCU协同架构**，结合 **MaxiCam AI视觉模组、二维云台、自主循迹、PID闭环控制以及自主设计PCB**，实现目标检测、自动跟踪、精准瞄准和小车自主运行。

The project is an open-source implementation of the **2025 National Undergraduate Electronic Design Contest (NUEDC) - Problem E: Autonomous Aiming System**.

It features a **dual-MCU architecture (STM32F103 + MSPM0G3507)** integrated with a **MaxiCam AI vision module, 2-axis gimbal, PID control, autonomous line following, and custom PCB design** to achieve autonomous target detection, tracking, aiming, and robot navigation.

---

# 系统架构 | System Architecture

```
                MaxiCam
                    │ UART
                    ▼
          ┌──────────────────┐
          │ STM32F103C8T6    │
          │------------------│
          │ • 视觉数据解析    │
          │ • PID控制算法     │
          │ • 云台控制        │
          │ • 高层任务调度    │
          └────────┬─────────┘
                   │ UART
                   ▼
          ┌──────────────────┐
          │ MSPM0G3507       │
          │------------------│
          │ • PWM输出         │
          │ • 电机驱动        │
          │ • 编码器采集      │
          │ • 灰度循迹        │
          │ • 底层外设控制    │
          └────────┬─────────┘
                   │
             小车运动执行
```

---

# MaxiCam视觉程序

MaxiCam 负责完成目标识别与坐标提取。

程序采用颜色识别算法检测目标，并计算目标中心坐标（Centroid），随后通过 UART 将目标坐标实时发送给 STM32。

主要流程如下：

```
图像采集
      │
颜色识别
      │
寻找目标区域
      │
计算质心坐标
      │
UART发送(X,Y)
```

STM32 只需接收目标中心坐标即可，无需进行复杂的图像处理，大幅降低主控计算压力，提高整体实时性。

---

# STM32程序

STM32 作为系统主控制器，负责整个视觉闭环控制。

主要功能包括：

- 接收 MaxiCam 输出的目标坐标
- 解析串口数据帧
- 计算目标与图像中心之间的误差
- 双PID控制二维云台
- 控制步进电机及舵机完成目标跟踪
- 与 MSPM0G3507 进行通信

控制流程如下：

```
接收坐标
      │
计算误差
      │
PID计算
      │
云台控制
      │
目标重新回到画面中心
```

---

# MSPM0G3507程序

MSPM0G3507 作为运动控制MCU，负责底层驱动。

主要功能包括：

- PWM输出
- TB6612电机驱动
- 编码器测速
- 八路灰度循迹
- 电机速度控制
- 小车运动控制

STM32 下发运动指令后，由 MSPM0G3507 独立完成底层执行，避免视觉算法占用运动控制资源。

---

# PCB设计

项目采用自主设计PCB，共设计两块通用扩展板：

- **STM32F103扩展板**
  - 云台接口
  - MaxiCam接口
  - 通信接口
  - 调试接口
  - 电源管理

- **MSPM0G3507扩展板**
  - TB6612驱动接口
  - 编码器接口
  - 灰度传感器接口
  - 按键与LED接口

两块扩展板均采用模块化设计，可直接复用于后续电子设计竞赛或其他嵌入式项目，无需重新设计硬件。

---

# 开发环境

- Keil MDK 5
- STM32CubeMX
- CCS / SysConfig（MSPM0）
- 嘉立创EDA Professional
- MaxiCam IDE（如适用）

---

# 硬件平台

- STM32F103C8T6
- MSPM0G3507
- MaxiCam AI Vision Module
- TB6612FNG
- 二维云台（步进电机 + 舵机）
- 八路灰度传感器
- 增量式编码器

---

# 注意事项

1. 根据实际接线修改 UART、PWM、GPIO 等引脚配置。

2. 首次使用请根据机械结构重新整定 PID 参数，以获得最佳跟踪效果。

3. 步进电机、舵机及电机转向可能因安装方式不同，需要修改方向参数。

4. PCB 为自主设计版本，如修改硬件，请同步更新原理图和引脚配置。

5. 本项目仅供学习交流及电子设计竞赛参考，请勿直接用于商业用途。

---

# License

This project is released under the MIT License.
