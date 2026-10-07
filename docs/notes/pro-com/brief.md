Implementation of a high-throughput DVB-S2 chain on Versal AI engines

WEITHOFFER Stefan - 21 sept. 2026

Ces fiches projets sont à saisir dans Moodle au plus tard le 10 septembre 2024

Encadrant.e 1 : Stefan WEITHOFFER

Encadrant.e 2 : Matthieu ARZEL

Mots clés du projet :

ersal, FPGA, Acceleration, AI engine, Parallel Architectures, Embedded Systems, Forward error correction, Signal processing

TAF(s) concernée(s) et leur campus :

SEH, Brest

Attention: (Si vous estimez que votre projet peut intéresser des élèves d’autres TAF, pensez à prévenir ou à solliciter les référents des projets 3A et/ou les coordinateurs.trices de ces TAF*)

Nombre d’élèves souhaité : 4

Contact référent TAF principal pour ce projet : Catherine DOUILLARD

Catégorie(s) du projet :

Partenaire extérieur : (entreprise, collectivité, association, laboratoire de recherche)
Multi-disciplinaire (au moins 2 TAF concernées)

 
Contexte du projet

The new AMD Versal FPGA family is a heterogeneous hardware acceleration platform that includes:

Programmable processors (ARM)

Programmable logic

AI Engine cores, consisting of clusters of vector processor cores (SIMD) with Very Long Instruction Word (VLIW) parallel instruction execution

These AI Engines can be used not only for AI/ML applications but also for signal processing applications.

TurboConcept specializes in providing hardware implementations of error-correcting code encoders and decoders. The products commercialized by the company cover several coding technologies, including turbo codes, LDPC codes, and polar codes, implemented on various hardware targets such as ASICs and FPGAs.

TurboConcept plans to propose to its customers high-throughput FEC decoder solutions based on the use of AI Engines available in AMD Versal devices. This project is part of that industrial objective.

This project will be a follow-up of an internship (April-August 2026) conducted at TurboConcept and IMTA on the implementation of an LDPC decoder using Versal AI engines. During this internship an LDPC decoder using a single AI engine reaching a throughput of 30 Mbits/s was developed.

Descriptif succinct du projet

The objective of this project will be to develop a high throughput DVB-S2 reception chain that will be implemented on Versal FPGA using a mixture of AI engines and programmble logic.

DVB‑S2 is a highly efficient, flexible satellite transmission standard with very high spectral efficiency and robustness, enabling modern HD/4K broadcasting and broadband satellite services. The reception chain will include the following basic building blocks:

Demapper

Interleaver

LDPC decoder

BCH decoder

These signal processing blocks are fundamental building blocks that can be found in many communication standards (DVB-S2, 5G, CCSDS, etc.).

The objective of the project will be to reach a throughput up to 1Gbits/s.

For each building block the following tasks are foreseen:

Understand the algorithms that needs to be implemented and the provided source code

Modify the existing source code to a fixed-point implementation

Develop a first version of the algorithm in C++ to be implemented on AI engine (not-necessarily a parallel implementation using SIMD instructions)

Measure the obtained throughput

Define a parallel architecture that will use SIMD instructions to increase significantly the throughput on one AI engine.

Measure the obtained throughput and identify the bottlenecks

Optimize the source code to increase the throughput (still using one AI engine)

Define a partitioning between programmable logic and one or several AI engines that will be able to reach the target throughput.

Integrate the block into the full reception chain

An important task will also be to integrate all blocks developed individually into a single reception chain on the FPGA using a mixture of AI engines and programmable logic. The resulting reception chain will be implemented on an FPGA demonstrator.

Livrables identifiés

Detailed written project report

FPGA-based demonstration

Source code of the project

Données d'entrée fournies par les encadrants

Documentation from the previous internship on the Versal platform

Xilinx documentation on the Versal platform

C source code of the full reception chain (using non-parallel C language):

Demapper and interleaver

LDPC decoder

BCH decoder

Full simulation chain including transmitter

Ressources particulières (y compris partenariat)

Emulation platform using Vitis IDE development environment (Linux)

FPGA boards with Versal AIE and associated hardware devices

 
Compétences souhaitées pour la bonne réalisation du projet

(required) Experience with at least one of the following: C, C++

(required) Experience with Forward error correction algorithm with at least one of the following: LDPC, BCH, Reed-Solomon.

(beneficial) Experience with at least one of the following: VHDL, Verilog, SystemVerilog

(beneficial) Experience with Python

(beneficial) Experience with version control (git, svn, etc.)

## Targeted and assessed skills

Pour le S5 :

Pour le S6B1 :

Rappel des compétences obligatoires pour le S5

CG6 : Conduire un projet innovant, complexe, risqué ou à forts enjeux

CG7 : Animer et gérer une équipe en différents modes de management

Compétences obligatoires pour le S6B1

  CG4 : Critiquer et décider

  CG9 : Communiquer

---

### 项目名称

**基于 Versal AI Engine 的高吞吐率 DVB-S2 链路实现**

**负责人：** Stefan WEITHOFFER　　**日期：**2026 年 9 月 21 日

> 这些项目简介最迟须于 2024 年 9 月 10 日录入 Moodle。

### 项目信息

- **指导教师 1：** Stefan WEITHOFFER
- **指导教师 2：** Matthieu ARZEL
- **项目关键词：** Versal、FPGA、加速、AI Engine、并行架构、嵌入式系统、前向纠错、信号处理
- **涉及的 TAF 及校区：** SEH，Brest
- **期望学生人数：** 4 人
- **主要 TAF 联系人：** Catherine DOUILLARD
- **项目类别：**
  - 外部合作伙伴（企业、地方政府、协会或研究实验室）
  - 多学科项目（至少涉及两个 TAF）

> **提示：** 如果认为项目可能吸引其他 TAF 的学生，请通知或邀请 3A 项目负责人及相关 TAF 的协调人参与。

## 项目背景

新一代 AMD Versal FPGA 是一种异构硬件加速平台，包含：

- 可编程处理器（ARM）；
- 可编程逻辑；
- AI Engine 核心：由向量处理器核心集群组成，支持 SIMD 以及超长指令字（VLIW）并行执行。

AI Engine 不仅可用于 AI/ML 应用，也适用于信号处理应用。

TurboConcept 专注于提供纠错码编码器和解码器的硬件实现。公司产品涵盖 Turbo 码、LDPC 码和 Polar 码等多种编码技术，并支持 ASIC、FPGA 等硬件平台。

TurboConcept 计划利用 AMD Versal 器件中的 AI Engine，为客户提供高吞吐率 FEC 解码器方案。本项目属于这一产业目标的一部分。

本项目将延续 2026 年 4 月至 8 月在 TurboConcept 和 IMTA 开展的实习项目。该实习实现了一个基于 Versal AI Engine 的 LDPC 解码器：使用单个 AI Engine 时吞吐率达到 30 Mbit/s。

## 项目简介

本项目旨在开发一条高吞吐率 DVB-S2 接收链路，并在 Versal FPGA 上结合 AI Engine 与可编程逻辑实现。

DVB-S2 是一种高效、灵活的卫星传输标准，具有很高的频谱效率和良好的鲁棒性，可用于现代高清/4K 广播及宽带卫星服务。接收链路将包括以下基本模块：

1. 解映射器（Demapper）
2. 交织器（Interleaver）
3. LDPC 解码器
4. BCH 解码器

这些信号处理模块是 DVB-S2、5G、CCSDS 等多种通信标准中的基础模块。

项目目标是实现最高 **1 Gbit/s** 的吞吐率。

### 各模块的主要任务

1. 理解待实现算法及所提供的源代码。
2. 将现有源代码修改为定点实现。
3. 使用 C++ 开发可部署到 AI Engine 的算法初版（不要求一开始就使用 SIMD 并行指令）。
4. 测量吞吐率。
5. 设计使用 SIMD 指令的并行架构，以显著提升单个 AI Engine 的吞吐率。
6. 测量吞吐率并识别瓶颈。
7. 优化源代码，提高单个 AI Engine 的吞吐率。
8. 在可编程逻辑与一个或多个 AI Engine 之间进行划分，以达到目标吞吐率。
9. 将模块集成到完整接收链路中。

此外，还需要将各模块集成到 FPGA 上由 AI Engine 与可编程逻辑共同实现的单一接收链路中，并在 FPGA 演示平台上完成部署。

## 预期交付物

- 详细的项目书面报告；
- 基于 FPGA 的演示系统；
- 项目源代码。

## 指导教师提供的输入数据

- Versal 平台前期实习项目的相关文档；
- Versal 平台的 Xilinx 文档；
- 使用非并行 C 语言编写的完整接收链路 C 源代码：
  - 解映射器和交织器；
  - LDPC 解码器；
  - BCH 解码器；
- 包含发射机的完整仿真链路。

## 专用资源（包括合作条件）

- 使用 Vitis IDE 开发环境（Linux）的仿真平台；
- 配备 Versal AIE 及相关硬件设备的 FPGA 开发板。

