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

## 1. DVB-S2 / 数字通信基础
$learn-topic notes docs/notes/pro-com/dvb-s2/Communication-System.md DVB-S2 digital communication bit symbol complex baseband I/Q constellation modulation BPSK QPSK 8PSK 16APSK 32APSK Gray mapping Mapper channel noise AWGN random variable probability probability density conditional probability likelihood hard decision soft decision Demapper LLR FECFRAME MODCOD --requirements 从零开始，按教科书方式系统讲解；不要假设概率论、复数通信表示和数字调制基础已经掌握；任何公式使用前先补足所需前置知识；包含直觉解释、正式定义、公式来源、必要推导、完整数值例子、星座图解释、数据流图、常见误区；重点建立完整DVB-S2收发链的思维模型，不限制篇幅。如需图片，可自制并放入docs\notes\pro-com\assert。如需链接，仅给出官方文档链接，例如ETSI、AMD等官方网站，不引用CSDN、知乎、博客或其他非官方网页

## 2. FEC / 前向纠错基础

$learn-topic notes docs/notes/pro-com/fec/Forward-Error-Correction.md Forward Error Correction redundancy parity information bits parity bits codeword code rate BER FER Hamming distance linear block code GF(2) modulo-2 arithmetic XOR generator matrix parity-check matrix syndrome minimum distance error detection error correction hard decoding soft decoding coding gain --requirements 从零开始，按教科书方式系统讲解；不要假设离散数学、线性代数、有限域或编码理论基础已经掌握；重点解释为什么加入冗余能够检测和纠正错误，以及codeword、parity、code rate、Hamming distance、generator matrix、parity-check matrix和syndrome之间的关系；任何数学符号、矩阵公式、GF(2)运算在第一次使用前必须先解释；包含直觉解释、正式定义、公式来源、必要推导、完整的小型编码和纠错例子、矩阵手算过程、数据流图、常见误区；最终为BCH和LDPC建立扎实基础，不限制篇幅。如需图片，可自制并放入docs\notes\pro-com\assert。如需链接，仅给出官方或权威原始资料链接，标准相关内容优先使用ETSI等官方网站，不引用CSDN、知乎、博客或其他非官方网页
## 3. BCH
$learn-topic notes docs/notes/pro-com/fec/BCH.md BCH codes cyclic codes polynomial representation GF(2) finite fields GF(2^m) primitive polynomial minimal polynomial generator polynomial BCH encoding syndrome error locator polynomial error positions Berlekamp-Massey algorithm Chien search BCH decoding DVB-S2 BCH --requirements 从零开始，按教科书方式系统讲解；不要假设已经掌握有限域、GF(2^m)、多项式运算或循环码；在使用generator polynomial、minimal polynomial、syndrome、error locator polynomial等概念之前，先补足真正需要的前置知识；重点解释BCH为什么能够纠错、编码过程如何产生冗余、接收端如何通过syndrome定位错误；包含直觉解释、正式定义、数学原理、必要公式推导、小规模可手算的完整BCH编码和译码例子、Berlekamp-Massey和Chien search逐步原理、伪代码、算法数据流、与DVB-S2中BCH outer code的关系、常见误区；最终达到能够阅读和分析BCH编码器及解码器C/C++源码的程度，不限制篇幅。如需图片，可自制并放入docs\notes\pro-com\assert。如需链接，仅给出官方标准、官方文档或其他权威原始资料链接，不引用CSDN、知乎、博客或其他非官方网页
## 4. LDPC
$learn-topic notes docs/notes/pro-com/fec/LDPC.md LDPC codeword parity-check matrix sparse matrix HcT=0 Tanner graph variable node check node channel LLR intrinsic information extrinsic information message passing iterative decoding Belief Propagation Sum-Product Algorithm Min-Sum Normalized Min-Sum Offset Min-Sum flooding schedule layered decoding stopping criterion quantization saturation fixed-point DVB-S2 LDPC --requirements 从零开始，按教科书方式系统讲解；不要假设已经掌握图模型、概率消息传递、LLR、矩阵或LDPC编码理论；先从非常小的parity约束和parity-check matrix例子建立直觉，再逐渐进入Tanner graph、Variable Node、Check Node和迭代译码；任何概率公式、LLR公式、VN/CN更新公式在出现前都必须解释所需前置知识及公式来源；详细说明Sum-Product如何得到、为什么可以近似为Min-Sum，以及Normalized Min-Sum和Offset Min-Sum为什么出现；包含正式定义、关键公式推导、小规模可手算LDPC例子、每轮message passing的中间数据、不同schedule、停止条件、量化、定点化、saturation、复杂度、数据存储结构、与DVB-S2 LDPC结构的关系、常见误区；最终达到能够阅读DVB-S2 LDPC decoder源码并为后续并行化和AIE实现建立理论基础的程度，不限制篇幅。如需图片，可自制并放入docs\notes\pro-com\assert。如需链接，仅给出ETSI、AMD或其他官方/权威原始资料链接，不引用CSDN、知乎、博客或其他非官方网页
## 5. Demapper and Interleaver
$learn-topic notes docs/notes/pro-com/dvb-s2/Demapper-and-Interleaver.md Demapper constellation received symbol Euclidean distance AWGN conditional probability density likelihood symbol likelihood bit likelihood exact LLR Max-Log LLR QPSK 8PSK 16APSK 32APSK soft demapping bit reliability DVB-S2 Interleaver Deinterleaver permutation inverse permutation block interleaver modulation bit positions FECFRAME indexing memory layout buffer C C++ implementation --requirements 从零到实现级，按教科书方式系统讲解；不要假设概率密度、高斯分布、条件概率、likelihood、欧氏距离、permutation等基础已经掌握；Demapper部分从y=s+n开始，先解释噪声、随机变量、概率密度和Gaussian分布，再逐步从一维Gaussian推导到I/Q二维AWGN条件概率密度p(y|s)，解释|y-s|²为什么是星座图欧氏距离平方，再进一步推导symbol likelihood、bit likelihood、LLR以及Max-Log近似；必须明确概率与概率密度的区别以及每个公式左侧到底是什么量；Interleaver部分从permutation和inverse permutation开始，解释为什么需要bit interleaving、它与modulation bit positions和LDPC之间的关系；包含12-bit完整手算Interleave/Deinterleave例子、DVB-S2真实64800和16200长度、QPSK/8PSK/APSK相关规则、索引公式推导、forward/inverse mapping、伪代码、C/C++实现、buffer组织、memory access pattern、输入输出验证方法和常见误区；不限制篇幅。如需图片，可自制星座图、矩阵图、数据流图等并统一放入docs\notes\pro-com\assert。如需链接，仅给出ETSI等官方标准或其他官方文档链接，不引用CSDN、知乎、博客或其他非官方网页
## 6. Versal AI Engine
$learn-topic notes docs/notes/pro-com/versal/Versal-AIE.md AMD Versal heterogeneous computing PS PL AI Engine AIE array AIE tile scalar processor vector processor SIMD VLIW local memory memory bank stream buffer window DMA kernel graph ADF PLIO GMIO NoC cascade AIE-to-AIE communication PL-to-AIE communication DDR-to-AIE communication memory hierarchy --requirements 从零开始，按教科书方式系统讲解；不要假设已经掌握异构计算、CPU体系结构、FPGA架构、SIMD、VLIW、DMA或NoC；先从CPU和FPGA的本质区别、Zynq的PS+PL架构讲起，再说明Versal为什么增加AI Engine以及PS、PL、AIE三者各自适合做什么；深入解释AIE tile内部结构、scalar/vector processor、SIMD和VLIW的区别、local memory和memory bank、stream与buffer/window、DMA、kernel、graph、ADF、PLIO、GMIO、cascade、NoC以及数据如何在DDR、PL和AIE之间移动；任何硬件术语在第一次出现时都先解释作用和存在原因；包含架构图、数据流图、memory hierarchy图、简单AIE C++ kernel和graph例子、数据实际流动过程、常见误区；重点服务于后续DVB-S2信号处理与FEC硬件加速，不需要展开与项目无关的Versal子系统，不限制篇幅。如需图片，可自制并放入docs\notes\pro-com\assert。如需链接，仅给出AMD/Xilinx官方文档链接，不引用CSDN、知乎、博客或其他非官方网页；涉及版本相关行为时优先参考当前使用版本的AMD官方文档
## 7. High-Throughput Architecture
$learn-topic notes docs/notes/pro-com/acceleration/High-Throughput-Architecture.md hardware acceleration CPU FPGA AI Engine scalar vector SIMD VLIW pipeline pipelining data parallelism instruction-level parallelism task parallelism fixed-point quantization Q-format rounding truncation overflow saturation dynamic range latency throughput initiation interval bandwidth memory bandwidth compute-bound memory-bound arithmetic intensity data movement sequential access random access buffering double buffering DMA profiling cycle counting bottleneck analysis Roofline model single-AIE multi-AIE PL AIE partitioning pipeline balancing end-to-end throughput DVB-S2 integration --requirements 从零开始，按教科书方式系统讲解；不要假设已经掌握计算机体系结构、流水线、并行计算、fixed-point或性能分析；首先解释为什么专用硬件能够加速以及CPU、FPGA、AIE之间的执行模型差异；详细区分latency、throughput、initiation interval、bandwidth和memory bandwidth，并通过完整数值例子说明它们之间的关系；系统讲解pipeline、SIMD、VLIW、data parallelism、task parallelism和multi-AIE并行分别解决什么问题；系统讲解fixed-point、Q-format、量化、rounding、truncation、overflow、saturation和dynamic range，并解释它们对通信算法的影响；深入解释compute-bound与memory-bound、arithmetic intensity、data movement、连续与随机memory access、buffering、double buffering、DMA、profiling和bottleneck定位；介绍Roofline式性能分析思维，并说明如何通过测量而不是猜测决定优化方向；进一步解释single-AIE、multi-AIE、PL、AIE以及PL+AIE partitioning的选择原则；最终把这些理论连接到完整DVB-S2接收链Demapper→Deinterleaver→LDPC→BCH，讲解如何分析各模块吞吐率、pipeline balancing以及如何设计接近1 Gbit/s的end-to-end architecture；包含公式、公式来源、必要推导、性能估算、完整计算例子、架构图、数据流图、常见误区，不限制篇幅。如需图片，可自制并放入docs\notes\pro-com\assert。如需链接，仅给出AMD或其他官方/权威原始资料链接，不引用CSDN、知乎、博客或其他非官方网页