设 a = 10
设 b = 20
设 姓名 = "李华"
3. 数学运算
支持标准加减乘除，甚至可以直接在交互界面当计算器用。
lpp
1 + 1
a * b
4. 条件判断（如果...那么...否则）
支持基于条件的逻辑分支。
lpp
如果 a > 5 那么    
打印 "a 大于 5"
否则    
打印 "a 不大于 5"
5. 循环执行（重复执行）
用于重复执行某段代码。
lpp
设 i = 0
重复执行 5 次    
打印 i    
设 i = i + 1
6. 记忆管理
由于 L++ 自带持久化记忆，你甚至不需要重启程序。
lpp
清空记忆  // 清空当前保存的所有变量记忆
exit      // 退出 L++ 交互界面
🛠️ 从源码编译 (开发者)
如果你熟悉 C++，想要自己从源码编译 L++，请确保你的电脑安装了 g++ (MinGW)。
在源码目录下执行以下命令（将 你的源码文件名.cpp 替换为实际的源码文件）：
1. 编译资源文件 (生成图标)：
bash
windres icon.rc -O coff -o icon.res
2. 编译主程序 (包含静态链接和图标)：
bash
g++ "你的源码文件名.cpp" "icon.res" -o "你的输出程序名.exe" -static -mconsole
(注意：-static 保证静态链接，-mconsole 强制生成控制台程序入口)
📄 开源许可证
本项目采用 MIT License 许可证。
你可以自由地使用、修改、分发本项目，甚至用于商业用途，只需保留原作者信息即可。
💬 反馈与交流
如果你在使用过程中遇到 Bug，或者有新的语法想法，欢迎在 Issues 区提出！
「**遇到问题？去 [Issues](https://github.com/lixytag/Lpp/issues) 反馈**」
也欢迎通过以下邮箱直接联系我：
📧 lizi369gz@126.com
Here is the English translation of your README.md, written in a professional, open-source style while keeping the original structure and the friendly, respectful tone you asked for.

L++ (L Plus Plus)
In the L++ interactive interface, you can directly type the following commands and press Enter:

1. Print Output
Use the 打印 (Print) keyword to output text or variable contents.

lpp
打印 "元宵节大烧甜66666"
2. Variable Definition and Assignment
Use the 设 (Set) keyword to define variables. Basic data types are supported.

lpp
设 a = 10
设 b = 20
设 姓名 = "李华"
3. Mathematical Operations
Standard addition, subtraction, multiplication, and division are supported. You can even use the interactive interface directly as a calculator.

lpp
1 + 1
a * b
4. Conditional Statements (If...Then...Else)
Logical branching based on conditions is supported.

lpp
如果 a > 5 那么    
打印 "a 大于 5"
否则    
打印 "a 不大于 5"
5. Loops (Repeat Execution)
Used to execute a block of code repeatedly.

lpp
设 i = 0
重复执行 5 次    
打印 i    
设 i = i + 1
6. Memory Management
Since L++ features built-in persistent memory, you don't even need to restart the program.

lpp
清空记忆  // Clear all currently saved variable memory
exit      // Exit the L++ interactive interface
🛠️ Compiling from Source (For Developers)

If you are familiar with C++ and want to compile L++ from source yourself, please ensure that g++ (MinGW) is installed on your computer.

Run the following commands in the source code directory (replace 你的源码文件名.cpp with your actual source file name):

1. Compile the resource file (to generate the icon):

bash
windres icon.rc -O coff -o icon.res
2. Compile the main program (with static linking and icon):

bash
g++ "你的源码文件名.cpp" "icon.res" -o "你的输出程序名.exe" -static -mconsole
(Note: -static ensures static linking, and -mconsole forces the generation of a console application entry point.)

📄 Open Source License

This project is licensed under the MIT License.

You are free to use, modify, and distribute this project, even for commercial purposes, as long as you retain the original author's information.

💬 Feedback and Communication

If you encounter any bugs during use, or have new ideas for syntax, feel free to submit them in the Issues section!

"Encountered a problem? Go to Issues to report it."

You can also contact me directly via the following email:

📧 lizi369gz@126.com

A quick note on the translation:

I kept the Chinese keywords (打印, 设, 如果, etc.) inside the code blocks exactly as they are, since they are part of the L++ language syntax.

I used standard GitHub Markdown formatting (like bash for the compilation commands) so it renders nicely on your repository page.

I slightly polished the phrasing in the "Compiling" section to match how English-speaking developers usually write documentation.

You can copy this directly into your README.md or README_EN.md file. Let me know if you'd like any adjustments!

