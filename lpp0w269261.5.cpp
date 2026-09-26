#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>

// ==================== 全局变量 ====================
std::map<std::string, std::string> vars; // 升级：变量表现在存字符串
const std::string MEM_FILE = "lpp.mem";

// ==================== 记忆文件操作 ====================
void saveMemory() {
    std::ofstream out(MEM_FILE);
    if (!out) return;
    for (const auto& pair : vars) {
        out << pair.first << "=" << pair.second << std::endl;
    }
    out.close();
}

void loadMemory() {
    std::ifstream in(MEM_FILE);
    if (!in) return;
    std::string line;
    while (std::getline(in, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string name = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            vars[name] = value;
        }
    }
    in.close();
}

void clearMemory() {
    vars.clear();
    std::remove(MEM_FILE.c_str());
    std::cout << "记忆已清空" << std::endl;
}

// ==================== 辅助函数：去除首尾空格 ====================
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

// ==================== 表达式计算 ====================
// 支持数字、变量、加减乘除
int evaluate(const std::string& expr) {
    std::vector<std::string> tokens;
    std::string current;

    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        if (std::isdigit(c)) {
            current += c;
        } else if (std::isalpha(c) || (unsigned char)c >= 0x80) { // 支持中文变量名
            current += c;
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (!current.empty()) { tokens.push_back(current); current.clear(); }
            tokens.push_back(std::string(1, c));
        } else if (c == ' ') {
            if (!current.empty()) { tokens.push_back(current); current.clear(); }
        }
    }
    if (!current.empty()) tokens.push_back(current);

    // 把变量名替换成它的值
    for (auto& token : tokens) {
        if (!token.empty() && (std::isalpha(token[0]) || (unsigned char)token[0] >= 0x80)) {
            if (vars.find(token) != vars.end()) {
                token = vars[token];
            } else {
                std::cout << "错误：变量 " << token << " 未定义" << std::endl;
                return 0;
            }
        }
    }

    // 先算乘除
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "*" || tokens[i] == "/") {
            int a = std::stoi(tokens[i - 1]);
            int b = std::stoi(tokens[i + 1]);
            if (tokens[i] == "/" && b == 0) { std::cout << "错误：除数不能为0" << std::endl; return 0; }
            int result = (tokens[i] == "*") ? (a * b) : (a / b);
            tokens[i - 1] = std::to_string(result);
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            i--;
        }
    }

    // 再算加减
    if (tokens.empty()) return 0;
    int result = std::stoi(tokens[0]);
    for (size_t i = 1; i < tokens.size(); i += 2) {
        int b = std::stoi(tokens[i + 1]);
        if (tokens[i] == "+") result += b;
        else if (tokens[i] == "-") result -= b;
    }
    return result;
}

// ==================== 主程序 ====================
int main() {
    loadMemory();
    if (!vars.empty()) {
        std::cout << "已加载上次的记忆（共 " << vars.size() << " 个变量）" << std::endl;
    }

    std::string line;
    bool inIfBlock = false;
    bool inElseBlock = false;
    bool skipBlock = false;
    bool conditionMet = false;
    int repeatCount = 0;
    std::string repeatBlock = "";

    while (true) {
        if (repeatCount == 0) std::cout << "L++ > ";
        std::getline(std::cin, line);
        if (line.empty()) continue;

        // 处理重复执行块
        if (repeatCount > 0) {
            repeatBlock += line + "\n";
            if (line.find("结束重复") != std::string::npos) {
                for (int i = 0; i < repeatCount; i++) {
                    std::istringstream iss(repeatBlock);
                    std::string cmd;
                    while (std::getline(iss, cmd)) {
                        if (cmd.find("结束重复") != std::string::npos) break;
                        // 递归执行块内代码（简化版）
                        if (cmd.find("打印 ") == 0) {
                            std::string content = trim(cmd.substr(7));
                            if (content.length() >= 2 && content.front() == '"' && content.back() == '"')
                                content = content.substr(1, content.length() - 2);
                            std::cout << content << std::endl;
                        } else if (cmd.find("设 ") == 0) {
                            size_t eq_pos = cmd.find('=');
                            if (eq_pos != std::string::npos) {
                                std::string name = trim(cmd.substr(2, eq_pos - 2));
                                std::string value_str = trim(cmd.substr(eq_pos + 1));
                                vars[name] = std::to_string(evaluate(value_str));
                            }
                        }
                    }
                }
                repeatCount = 0;
                repeatBlock = "";
                std::cout << "重复执行完成" << std::endl;
            }
            continue;
        }

        // 退出
        if (line == "exit") { std::cout << "再见！" << std::endl; break; }
        if (line == "清空记忆") { clearMemory(); continue; }

        // 处理如果...那么
        if (line.find("如果 ") == 0 && line.find("那么") != std::string::npos) {
            size_t then_pos = line.find("那么");
            std::string cond_str = line.substr(3, then_pos - 3);
            size_t op_pos = std::string::npos;
            std::string op = "";
            if ((op_pos = cond_str.find(">=")) != std::string::npos) op = ">=";
            else if ((op_pos = cond_str.find("<=")) != std::string::npos) op = "<=";
            else if ((op_pos = cond_str.find("==")) != std::string::npos) op = "==";
            else if ((op_pos = cond_str.find(">")) != std::string::npos) op = ">";
            else if ((op_pos = cond_str.find("<")) != std::string::npos) op = "<";

            if (op_pos != std::string::npos) {
                int left = evaluate(trim(cond_str.substr(0, op_pos)));
                int right = evaluate(trim(cond_str.substr(op_pos + op.length())));
                conditionMet = false;
                if (op == ">") conditionMet = (left > right);
                else if (op == "<") conditionMet = (left < right);
                else if (op == ">=") conditionMet = (left >= right);
                else if (op == "<=") conditionMet = (left <= right);
                else if (op == "==") conditionMet = (left == right);
                
                inIfBlock = true;
                skipBlock = !conditionMet;
            }
            continue;
        }

        if (line.find("否则") != std::string::npos && inIfBlock) {
            inElseBlock = true;
            skipBlock = conditionMet;
            continue;
        }

        if (line.find("结束如果") != std::string::npos) {
            inIfBlock = false; inElseBlock = false; skipBlock = false;
            continue;
        }

        // 如果条件不满足，跳过块内内容（简化的块处理）
        if ((inIfBlock || inElseBlock) && skipBlock) continue;

        // 处理重复执行 N 次
        if (line.find("重复执行 ") == 0 && line.find("次") != std::string::npos) {
            std::string num_str = trim(line.substr(5, line.find("次") - 5));
            repeatCount = evaluate(num_str);
            repeatBlock = "";
            if (repeatCount > 0) std::cout << "进入重复块，请继续输入代码，以 结束重复 结尾：" << std::endl;
            continue;
        }

        // 变量声明：设 a = 1
        if (line.find("设 ") == 0) {
            size_t eq_pos = line.find('=');
            if (eq_pos != std::string::npos) {
                std::string name = trim(line.substr(2, eq_pos - 2));
                std::string value_str = trim(line.substr(eq_pos + 1));
                // 尝试作为表达式计算，如果失败就存字符串
                try {
                    int value = evaluate(value_str);
                    vars[name] = std::to_string(value);
                    saveMemory();
                    std::cout << "已设置 " << name << " = " << value << std::endl;
                } catch (...) {
                    // 去引号当字符串存
                    if (value_str.length() >= 2 && value_str.front() == '"' && value_str.back() == '"')
                        value_str = value_str.substr(1, value_str.length() - 2);
                    vars[name] = value_str;
                    saveMemory();
                    std::cout << "已设置 " << name << " = \"" << value_str << "\"" << std::endl;
                }
            }
            continue;
        }

        // 打印：打印 "内容"
        if (line.find("打印 ") == 0) {
            std::string content = trim(line.substr(7)); // 修复：正确跳过"打印 "的7个字节
            // 如果是变量名，打印变量的值
            if (vars.find(content) != vars.end()) {
                std::cout << vars[content] << std::endl;
            } else if (content.length() >= 2 && content.front() == '"' && content.back() == '"') {
                std::cout << content.substr(1, content.length() - 2) << std::endl;
            } else {
                std::cout << content << std::endl;
            }
            continue;
        }

        // 表达式计算
        try {
            int result = evaluate(line);
            std::cout << result << std::endl;
        } catch (...) {
            std::cout << "看不懂这个命令：" << line << std::endl;
        }
    }
    return 0;
}