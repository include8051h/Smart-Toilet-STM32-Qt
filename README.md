📌 项目定位
本项目用于验证智慧公共卫生间系统技术可行性，聚焦本地串口通信架构：
无网络依赖
串口直连，无网络延迟
仅需PC终端 + STM32，无需云服务
数据全程本地传输，无隐私泄露风险



🔗 技术演进：
本项目为智慧公共卫生间系统云平台版的前身，完整演进路径：
本地串口系统 (Smart-Toilet-STM32-Qt) → 云平台远程监控 (Smart-Toilet-STM32-Iot-Cloud-Platform)



🔧 环境要求
| 组件 | 版本 |  
|------|------|  
| STM32开发 | Keil MDK 5.37+ |  
| Qt上位机 | Qt Creator 5.15.2+ (MinGW/MSVC) |  
| 硬件 | STM32F103C8T6开发板 + 传感器套件 |  
| 连接 | USB转TTL模块（波特率9600） |  



⚙️ 编译运行

STM32下位机
1.用Keil打开 STM32code/LED_open.uvprojx
2.连接下载器，编译下载至开发板

Qt上位机
1.用Qt Creator打开 Qtcode/Qt_Stm32_Communi_0.pro
2.编译运行 → 点击"打开串口" → 查看实时数据



📂 项目结构
Smart-Toilet-STM32-Qt/
├── Qtcode/              # Qt上位机工程（PC端）
│   └── Qt_Stm32_Communi_0/  # Qt项目主目录
│       └── Qt_Stm32_Communi_0.pro  # Qt项目工程主文件
├── STM32code/           # STM32下位机工程（嵌入式端）
│   ├── Hardware/        # 硬件驱动层
│   ├── Library/         # 外设库文件
│   ├── Start/           # 启动文件
│   ├── System/          # 系统配置
│   ├──  User/            # 用户应用层
│   └── LED_open.uvprojx   # STM32项目工程主文件
├── docs/                # 项目文档
│   └── 说明文档.docx    # 中文详细说明
└── .gitignore           # Git忽略规则
