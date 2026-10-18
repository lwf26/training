// ============================================================
// 调试记录 —— password_checker.cpp
//
// 本次一共修复三处逻辑错误，并消除一处编译警告：
//
// 1. 数字判断错误（原第 19 行）
//    原代码：bool digit_ok = contains_between(password, 0, 9);
//    问题：传给 char 形参的是整数 0 和 9，被当成 ASCII 码 0 和 9
//          （控制字符），而不是字符 '0' 和 '9'（ASCII 48 和 57）。
//          因此任何数字字符都落在区间之外，digit_ok 恒为 false，
//          所有密码都被判为 INVALID。
//    修复：改为 contains_between(password, '0', '9')。
//
// 2. 长度判断错误（原第 16 行）
//    原代码：bool length_ok = password.size() <= 10;
//    问题：题目要求“长度至少 10 个字符”，即 >= 10；原条件写反成
//          “不超过 10”，导致 11 位以上的长密码反被判为不合格，
//          而 1 位的密码却被认为合格。
//    修复：改为 password.size() >= 10。
//
// 3. 范围边界错误（原第 6 行）
//    原代码：if (lower < ch && ch < upper)
//    问题：contains_between 的作用是判断字符是否落在闭区间
//          [lower, upper] 内，但两处都用严格小于，把区间两端排除在外。
//          于是 'A'、'Z'、'a'、'z'、'0'、'9' 这六个边界字符都判不到，
//          例如密码里只有 'Z' 时会被判为“没有大写字母”。
//    修复：改为 if (lower <= ch && ch <= upper)。
//
// 4. 编译警告（原第 22 行）
//    原代码：int checked = 0;
//    问题：定义后从未使用，-Wall -Wextra 下报 unused variable。
//    修复：删除该行。
//
// 自设计测试用例（输入 -> 预期输出）：
//   Chen Zhao Xy77889900      -> VALID    长度恰好 10，含大写/小写/数字，不含名姓
//   Chen Zhao Xy7788990       -> INVALID  长度只有 9
//   Chen Zhao Xy778899001     -> VALID    长度 11
//   Chen Zhao XY77889900      -> INVALID  没有小写字母
//   Chen Zhao xy77889900      -> INVALID  没有大写字母
//   Chen Zhao Xyzzzzzzzz      -> INVALID  没有数字
//   Chen Zhao Xy7788Chen00    -> INVALID  含有名 Chen（区分大小写）
//   Chen Zhao Xy7788Zhao00    -> INVALID  含有姓 Zhao（区分大小写）
//   Chen Zhao Zz0000000000    -> VALID    全用边界字符 Z / z / 0
//   Chen Zhao Xy7788chen00    -> VALID    小写 chen 不等于 Chen，不算含名
// ============================================================

#include <iostream>
#include <string>

bool contains_between(const std::string& text, char lower, char upper) {
    for (char ch : text) {
        if (lower <= ch && ch <= upper) {
            return true;
        }
    }
    return false;
}

bool check_password(const std::string& first_name,
                    const std::string& last_name,
                    const std::string& password) {
    bool length_ok = password.size() >= 10;
    bool upper_ok = contains_between(password, 'A', 'Z');
    bool lower_ok = contains_between(password, 'a', 'z');
    bool digit_ok = contains_between(password, '0', '9');
    bool name_ok = password.find(first_name) == std::string::npos &&
                   password.find(last_name) == std::string::npos;
    return length_ok && upper_ok && lower_ok && digit_ok && name_ok;
}

int main() {
    std::string first_name, last_name, password;
    if (!(std::cin >> first_name >> last_name >> password)) {
        std::cerr << "请输入：名 姓 密码\n";
        return 1;
    }
    std::cout << (check_password(first_name, last_name, password)
                      ? "VALID" : "INVALID") << '\n';
    return 0;
}