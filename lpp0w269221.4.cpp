#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>

// ==================== 全局变量 ====================
std::map<std::string, int> vars;                          // 变量表
std::map<std::string, std::vector<std::string>> functions; // 函数表
const std::string MEM_FILE = "lpp.mem";                    // 记忆文件

// ==================== 记忆文件操作 ====================
void saveMemory() {
    std::ofstream out(MEM_FILE);
    if (!out) return;
    for (const auto& pair : vars) {
        out << "VAR:" << pair.first << "=" << pair.second << std::endl;
    }
    for (const auto& pair : functions) {
        out << "FUNC:" << pair.first;
        for (const auto& line : pair.second) {
            out << "|" << line;
        }
        out << std::endl;
    }
    out.close();
}

void loadMemory() {
    std::ifstream in(MEM_FILE);
    if (!in) return;
    std::string line;
    while (std::getline(in, line)) {
        if (line.find("VAR:") == 0) {
            std::string content = line.substr(4);
            size_t pos = content.find('=');
            if (pos != std::string::npos) {
                std::string name = content.substr(0, pos);
                int value = std::stoi(content.substr(pos + 1));
                vars[name] = value;
            }
        }
        else if (line.find("FUNC:") == 0) {
            std::string content = line.substr(5);
            std::vector<std::string> parts;
            std::stringstream ss(content);
            std::string part;
            while (std::getline(ss, part, '|')) {
                parts.push_back(part);
            }
            if (!parts.empty()) {
                std::string func_name = parts[0];
                std::vector<std::string> body(parts.begin() + 1, parts.end());
                functions[func_name] = body;
            }
        }
    }
    in.close();
}

void clearMemory() {
    vars.clear();
    functions.clear();
    std::remove(MEM_FILE.c_str());
    std::cout << "记忆已清空" << std::endl;
}

// ==================== 表达式计算 ====================
int evaluate(const std::string& expr) {
    std::vector<std::string> tokens;
    std::string current;

    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        if (std::isdigit(c) || std::isalpha(c)) {
            current += c;
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (!current.empty()) { tokens.push_back(current); current.clear(); }
            tokens.push_back(std::string(1, c));
        } else if (c == ' ') {
            if (!current.empty()) { tokens.push_back(current); current.clear(); }
        }
    }
    if (!current.empty()) tokens.push_back(current);

    for (auto& token : tokens) {
        if (std::isalpha(token[0])) {
            if (vars.find(token) != vars.end()) {
                token = std::to_string(vars[token]);
            } else {
                std::cout << "错误：变量 " << token << " 未定义" << std::endl;
                return 0;
            }
        }
    }

    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "*" || tokens[i] == "/") {
            int a = std::stoi(tokens[i - 1]);
            int b = std::stoi(tokens[i + 1]);
            int result = (tokens[i] == "*") ? (a * b) : (a / b);
            tokens[i - 1] = std::to_string(result);
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            i--;
        }
    }

    int result = std::stoi(tokens[0]);
    for (size_t i = 1; i < tokens.size(); i += 2) {
        int b = std::stoi(tokens[i + 1]);
        if (tokens[i] == "+") result += b;
        else if (tokens[i] == "-") result -= b;
    }
    return result;
}

// ==================== 条件判断 ====================
bool evaluateCondition(const std::string& expr) {
    std::string op;
    size_t op_pos = std::string::npos;

    std::vector<std::string> ops = {">=", "<=", "==", "!=", ">", "<"};
    for (const auto& o : ops) {
        op_pos = expr.find(o);
        if (op_pos != std::string::npos) { op = o; break; }
    }

    if (op_pos == std::string::npos) {
        return evaluate(expr) != 0;
    }

    std::string left_str = expr.substr(0, op_pos);
    std::string right_str = expr.substr(op_pos + op.length());

    int left = evaluate(left_str);
    int right = evaluate(right_str);

    if (op == ">") return left > right;
    if (op == "<") return left < right;
    if (op == ">=") return left >= right;
    if (op == "<=") return left <= right;
    if (op == "==") return left == right;
    if (op == "!=") return left != right;
    return false;
}

// ==================== 前向声明 ====================
void executeBlock(const std::vector<std::string>& block, size_t& index);

// ==================== 执行单条命令 ====================
void executeLine(const std::string& line) {
    if (line.empty()) return;

    if (line == "清空记忆") {
        clearMemory();
        return;
    }

    if (line.find("设 ") == 0) {
        size_t eq_pos = line.find('=');
        if (eq_pos != std::string::npos) {
            std::string name = line.substr(2, eq_pos - 2);
            name.erase(0, name.find_first_not_of(" "));
            name.erase(name.find_last_not_of(" ") + 1);

            std::string value_str = line.substr(eq_pos + 1);
            value_str.erase(0, value_str.find_first_not_of(" "));
            value_str.erase(value_str.find_last_not_of(" ") + 1);

            try {
                int value = evaluate(value_str);
                vars[name] = value;
                saveMemory();
                std::cout << "已设置 " << name << " = " << value << std::endl;
            } catch (...) {
                std::cout << "错误：无法解析值" << std::endl;
            }
        }
        return;
    }

    if (line.find("打印 ") == 0) {
        std::string content = line.substr(3);
        content.erase(0, content.find_first_not_of(" "));
        content.erase(content.find_last_not_of(" ") + 1);
        if (content.length() >= 2 && content.front() == '"' && content.back() == '"') {
            content = content.substr(1, content.length() - 2);
        }
        std::cout << content << std::endl;
        return;
    }

    if (line.length() > 2 && line.back() == ')' && line.find('(') != std::string::npos) {
        std::string func_name = line.substr(0, line.find('('));
        if (functions.find(func_name) != functions.end()) {
            for (const auto& cmd : functions[func_name]) {
                executeLine(cmd);
            }
            return;
        }
    }

    try {
        int result = evaluate(line);
        std::cout << result << std::endl;
    } catch (...) {
        std::cout << "看不懂这个命令：" << line << std::endl;
    }
}

// ==================== 执行代码块 ====================
void executeBlock(const std::vector<std::string>& block, size_t& index) {
    while (index < block.size()) {
        std::string line = block[index];

        if (line == "}" || line.find("否则") == 0) {
            return;
        }

        // 重复执行
        if (line.find("重复 ") == 0) {
            std::string times_str = line.substr(3);
            int times = evaluate(times_str);
            index++;
            std::vector<std::string> body;
            int brace_count = 1;
            while (index < block.size()) {
                if (block[index].find('{') != std::string::npos) brace_count++;
                if (block[index] == "}") {
                    brace_count--;
                    if (brace_count == 0) break;
                }
                if (brace_count >= 1) body.push_back(block[index]);
                index++;
            }
            for (int i = 0; i < times; i++) {
                size_t body_index = 0;
                executeBlock(body, body_index);
            }
            index++;
            continue;
        }

        // 如果那么 / 否则
        if (line.find("如果 ") == 0) {
            std::string cond = line.substr(3, line.find('{') - 3);
            cond.erase(0, cond.find_first_not_of(" "));
            cond.erase(cond.find_last_not_of(" ") + 1);

            index++;
            std::vector<std::string> if_body;
            int brace_count = 1;
            while (index < block.size()) {
                if (block[index].find('{') != std::string::npos) brace_count++;
                if (block[index] == "}") {
                    brace_count--;
                    if (brace_count == 0) break;
                }
                if (brace_count >= 1) if_body.push_back(block[index]);
                index++;
            }
            index++;

            std::vector<std::string> else_body;
            if (index < block.size() && block[index].find("否则") == 0) {
                index++;
                brace_count = 1;
                while (index < block.size()) {
                    if (block[index].find('{') != std::string::npos) brace_count++;
                    if (block[index] == "}") {
                        brace_count--;
                        if (brace_count == 0) break;
                    }
                    if (brace_count >= 1) else_body.push_back(block[index]);
                    index++;
                }
                index++;
            }

            if (evaluateCondition(cond)) {
                size_t if_index = 0;
                executeBlock(if_body, if_index);
            } else if (!else_body.empty()) {
                size_t else_index = 0;
                executeBlock(else_body, else_index);
            }
            continue;
        }

        executeLine(line);
        index++;
    }
}

// ==================== 主程序 ====================
int main() {
    loadMemory();
    if (!vars.empty() || !functions.empty()) {
        std::cout << "已加载上次的记忆（变量 " << vars.size() << " 个，函数 " << functions.size() << " 个）" << std::endl;
    }

    std::string line;

    while (true) {
        std::cout << "L++ > ";
        std::getline(std::cin, line);

        if (line.empty()) continue;
        if (line == "exit") { std::cout << "再见！" << std::endl; break; }

        // 函数定义
        if (line.find("函数 ") == 0) {
            size_t brace_pos = line.find('{');
            if (brace_pos == std::string::npos) continue;
            std::string func_name = line.substr(3, brace_pos - 3);
            func_name.erase(0, func_name.find_first_not_of(" "));
            func_name.erase(func_name.find_last_not_of(" ") + 1);
            size_t paren_pos = func_name.find('(');
            if (paren_pos != std::string::npos) func_name = func_name.substr(0, paren_pos);

            std::vector<std::string> body;
            std::string body_line;
            int brace_count = 1;
            while (std::getline(std::cin, body_line)) {
                if (body_line.find('{') != std::string::npos) brace_count++;
                if (body_line == "}") {
                    brace_count--;
                    if (brace_count == 0) break;
                }
                body.push_back(body_line);
            }
            functions[func_name] = body;
            saveMemory();
            std::cout << "函数 " << func_name << " 已定义" << std::endl;
            continue;
        }

        // 顶层多行代码块
        std::vector<std::string> block;
        block.push_back(line);
        if (line.back() == '{') {
            std::string body_line;
            int brace_count = 1;
            while (std::getline(std::cin, body_line)) {
                if (body_line.find('{') != std::string::npos) brace_count++;
                if (body_line == "}") {
                    brace_count--;
                    if (brace_count == 0) break;
                }
                block.push_back(body_line);
            }
        }

        size_t index = 0;
        executeBlock(block, index);
    }

    return 0;
}