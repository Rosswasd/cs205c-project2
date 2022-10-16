#include <iostream>
#include <cstring>
#include <stack>
#include <vector>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <bits/stdc++.h>
#include "WTNumber.h"
using namespace std;
struct Point
{
    string v;
    string a;
    Point()
    {
        v = "";
        a = "";
    }
};
vector<Point> varible;

void welcome() //人机交互
{
    cout << "---------------------------------------------------" << endl;
    cout << "------Welcome to use JQJ's simple calculator!------" << endl;
    cout << "-----You can use 'exit' to stop the calculator.----" << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "Now start your calculation:" << endl;
}

void bye()
{
    cout << "---------------------------------------------------" << endl;
    cout << "----------------Thank you for using!---------------" << endl;
    cout << "---------------------------------------------------" << endl;
}

string calc(string &str, string &so, int len)
{
    string o = so;
    bool dot_flag = false;
    double cnt = 0.0;
    int k = 0;
    for (int i = 0; i < len; i++)
    {
        if (str[i] >= 'a' && str[i] < 'z')
        {
            cout << "Sorry, the calculator now doesn't supported mathlib excpet numbers in double!" << endl;
            return "";
        }
        if (str[i] == '.')
        {
            dot_flag = true;
            continue;
        }
        if (dot_flag)
            k++;
        cnt = cnt * 10.0 + (double)(str[i] - 48);
    }
    cnt = cnt / (pow(10.0, (double)k));
    string ans;
    stringstream ss;
    if (o == "sin(")
    {
        ss << setprecision(2) << sin(cnt);
        ans = ss.str();
        return ans;
    }
    if (o == "cos(")
    {
        ss << setprecision(2) << cos(cnt);
        ans = ss.str();
        return ans;
    }
    if (o == "arctan(")
    {
        ss << setprecision(2) << atan(cnt);
        ans = ss.str();
        return ans;
    }
    if (o == "ln(")
    {
        ss << setprecision(2) << log(cnt);
        ans = ss.str();
        return ans;
    }
    if (o == "e(")
    {
        ss << setprecision(2) << exp(cnt);
        ans = ss.str();
        return ans;
    }
    if (o == "sqrt(")
    {
        ss << setprecision(2) << sqrt(cnt);
        ans = ss.str();
        return ans;
    }
    return "";
    //特殊（暂不支持高精度）
}

bool judge_CLO(string &s)
{
    if (s == "-h" || s == "--help")
    {
        cout << "--Help Menu---" << endl;
        cout << "本计算器规则参考网站: https://www.gnu.org/software/bc/manual/html_mono/bc.html" << endl;
        cout << "四则运算已实现" << endl;
        cout << "Mathlib暂不支持非double的对象和变量名" << endl;
        cout << "已支持一些简单bc语句" << endl;
        cout << "-----------------------" << endl;
        return true;
    }
    if (s == "-l" || s == "--mathlib")
    {
        cout << "--Math Library--" << endl;
        cout << "sin,cos,arctan,ln,e,sqrt" << endl;
        cout << "-----------------------" << endl;
        return true;
    }

    if (s == "-v" || s == "--version")
    {
        cout << "1.0.2  更新日期: 2022-10-16" << endl;
        return true;
    }
    return false;
}

bool judge_legal(string &str, int len)
{
    string s = str;
    string basic = "+-*/%";
    string var = "";
    for (int i = 0; i < len; i++)
        if (i != 0 && basic.find(s[i]) != string::npos && basic.find(s[i - 1]) != string::npos)
            return false;
        else
        {
            if (basic.find(s[i]) != string::npos || s[i] == '(' || s[i] == ')')
            {
                bool v_flag = false;
                bool var_flag = false;
                for (int j = 0; j < var.length(); j++)
                    if (var[j] >= 'a' && var[j] <= 'z')
                    {
                        var_flag = true;
                        break;
                    }
                if (var_flag && var[0] >= '0' && var[0] <= '9')
                    return false;
                if (!var_flag)
                {
                    var = "";
                    continue;
                }
                for (int j = 0; j < varible.size(); j++)
                    if (var == varible[j].v)
                    {
                        v_flag = true;
                        break;
                    }
                if (!v_flag)
                    return false;
                var = "";
            }
            else
            {
                var += s[i];
                if (i == len - 1)
                {
                    bool v_flag = false;
                    bool var_flag = false;
                    for (int j = 0; j < var.length(); j++)
                        if (var[j] >= 'a' && var[j] <= 'z')
                        {
                            var_flag = true;
                            break;
                        }
                    if (var_flag && var[0] >= '0' && var[0] <= '9')
                        return false;
                    if (!var_flag)
                    {
                        var = "";
                        continue;
                    }
                    for (int j = 0; j < varible.size(); j++)
                        if (var == varible[j].v)
                        {
                            v_flag = true;
                            break;
                        }
                    if (!v_flag)
                        return false;
                    var = "";
                }
            }
        }
    return true;
}

string judge_Mathlib(string &str, int len)
{
    string s = str;
    string lib[6] = {"sin(",
                     "cos(",
                     "arctan(",
                     "ln(",
                     "e(",
                     "sqrt("};
    for (int i = 0; i < 6; i++)
    {
        while (s.find(lib[i]) != string::npos)
        {
            int l = s.find(lib[i]);
            int left = 0, right = 0, r = 0;
            for (int j = l + lib[i].length(); j < len; j++)
            {
                if (str[j] == '(')
                    left++;
                else if (str[j] == ')')
                    right++;
                if (right > left)
                {
                    r = j;
                    break;
                }
            }
            string left_s = "";
            if (l != 0)
                left_s = s.substr(0, l);
            string mid_s = s.substr(l + lib[i].length(), r - l - lib[i].length());
            string right_s = s.substr(r + 1);
            mid_s = judge_Mathlib(mid_s, r - l - lib[i].length());
            if (mid_s == "")
                return "";
            if (judge_legal(mid_s, r - l - lib[i].length()))
                mid_s = calc(mid_s, lib[i], r - l - lib[i].length());
            else
                return "";
            s = left_s + mid_s + right_s;
        }
    }
    return s;
}

string clear(string &str, int len)
{
    string s;
    for (int i = 0; i < len; i++)
    {
        if (str[i] != ' ')
        {
            if (str[i] >= 'A' && str[i] <= 'Z')
                s += (str[i] + 32);
            else
                s += str[i];
        }
    }
    return s;
}

bool judge_br(string str)
{
    int left = 0, right = 0;
    for (int i = 0; i < str.length(); ++i)
    {
        if (str[i] == '(')
            left++;
        else if (str[i] == ')')
            right++;
        if (right > left)
            return false;
        if (str[i] == ')' && i == 0)
            return false;
        if (i != 0 && str[i - 1] == '(' && str[i] == ')')
            return false;
        if (i != 0 && str[i] == '(' && str[i - 1] == ')')
            return false;
    }
    return left == right;
}

int prior(string c)
{
    if (c == "+" || c == "-")
        return 1;
    if (c == "%")
        return 2;
    if (c == "*" || c == "/")
        return 3;
    return 0;
}

bool isOperator(char c)
{
    switch (c)
    {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
        return true;
    default:
        return false;
    }
    return 0;
}

string getPostfix(const string &expr)
{
    string output;
    stack<string> s;
    int i = 0;
    while (i < expr.length())
    {
        string c = "";
        bool c_flag = false;
        while (!isOperator(expr[i]) && expr[i] != '(' && expr[i] != ')')
        {
            c = c + expr[i++];
            c_flag = true;
            if (i >= expr.length())
                break;
        }
        if (!c_flag)
        {
            c = c + expr[i];
            i++;
        }
        if (isOperator(c[0]))
        {
            while (!s.empty() && isOperator(s.top()[0]) && prior(s.top()) >= prior(c))
            {
                output += s.top() + " ";
                s.pop();
            }
            s.push(c);
        }
        else if (c == "(")
        {
            s.push(c);
        }
        else if (c == ")")
        {
            while (s.top() != "(")
            {
                output += s.top() + " ";
                s.pop();
            }
            s.pop();
        }
        else
        {
            output += c + " ";
        }
    }
    while (!s.empty())
    {
        output += s.top() + " ";
        s.pop();
    }
    return output;
}
// 从栈中连续弹出两个操作数
void popTwoNumbers(stack<string> &s, string &first, string &second)
{
    first = s.top();
    if (first[0] >= 'a' && first[0] <= 'z')
    {
        for (int i = 0; i < varible.size(); i++)
            if (varible[i].v == first)
            {
                first = varible[i].a;
                break;
            }
    }
    s.pop();
    second = s.top();
    if (second[0] >= 'a' && second[0] <= 'z')
    {
        for (int i = 0; i < varible.size(); i++)
            if (varible[i].v == second)
            {
                second = varible[i].a;
                break;
            }
    }
    s.pop();
}

// 计算后缀表达式的值
string expCalculate(const string &postfix)
{
    string first, second;
    stack<string> s;
    int i = 0;
    while (i < postfix.length())
    {
        string c = "";
        bool c_flag = false;
        while (!isOperator(postfix[i]) && postfix[i] != ' ')
        {
            c = c + postfix[i++];
            c_flag = true;
            if (i >= postfix.length())
                break;
        }
        if (!c_flag)
        {
            c = c + postfix[i];
            i++;
        }
        if (c == " ")
            continue;
        switch (c[0])
        {
        case '+':
        {
            popTwoNumbers(s, first, second);
            char ch1[first.length() + 1], ch2[second.length() + 1];
            strcpy(ch1, first.c_str());
            strcpy(ch2, second.c_str());
            CWTNumber n1(ch1), n2(ch2);
            s.push((n2 + n1).ToString());
            break;
        }
        case '-':
        {
            popTwoNumbers(s, first, second);
            char ch1[first.length() + 1], ch2[second.length() + 1];
            strcpy(ch1, first.c_str());
            strcpy(ch2, second.c_str());
            CWTNumber n1(ch1), n2(ch2);
            s.push((n2 - n1).ToString());
            break;
        }
        case '*':
        {
            popTwoNumbers(s, first, second);
            char ch1[first.length() + 1], ch2[second.length() + 1];
            strcpy(ch1, first.c_str());
            strcpy(ch2, second.c_str());
            CWTNumber n1(ch1), n2(ch2);
            s.push((n1 * n2).ToString());
            break;
        }
        case '/':
        {
            popTwoNumbers(s, first, second);
            char ch1[first.length() + 1], ch2[second.length() + 1];
            strcpy(ch1, first.c_str());
            strcpy(ch2, second.c_str());
            CWTNumber n1(ch1), n2(ch2);
            s.push((n2 / n1).ToString());
            break;
        }
        default:
        {
            s.push(c);
            break;
        }
        }
    }
    string result = s.top();
    s.pop();
    return result;
}

bool judge_statement(string &str, int len)
{
    if (str.find("=") != string::npos)
    {
        int pos = str.find("=");
        if (pos == len - 1 || pos == 0)
            return 0;
        string v_left = str.substr(0, pos);
        string v_right = str.substr(pos + 1);
        if (v_left == "sin" || v_left == "cos" || v_left == "arctan" || v_left == "ln" || v_left == "e")
            return 0;
        if (v_right.find("=") != string::npos)
            return 0;
        if (v_left[0] < 'a' || v_left[0] > 'z')
            return 0;
        int i;
        for (i = 1; i < v_left.length(); i++)
            if ((v_left[i] < 'a' || v_left[i] > 'z') && (v_left[i] < '0' || v_left[i] > '9') && (v_left[i] != '_'))
                return 0;
        v_right = judge_Mathlib(v_right, v_right.length());
        if (v_right == "")
            return 0;
        if (!judge_legal(v_right, v_right.length()))
            return 0;
        string postfix = getPostfix(v_right);
        string result = expCalculate(postfix);
        for (i = 0; i < varible.size(); i++)
            if (varible[i].v == v_left)
            {
                varible[i].a = result;
                break;
            }
        if (i == varible.size())
        {
            Point temp;
            temp.a = result;
            temp.v = v_left;
            varible.push_back(temp);
        }
        return 1;
    }
    return 0;
}

int main()
{
    welcome();
    bool exit_flag = false;
    bool exp_flag = false;
    while (!exit_flag)
    {
        string read = "";
        getline(cin, read); //暂不支持进制输入
        read = clear(read, read.length());
        if (read == "")
            continue;

        if (read == "exit") //程序终止判断
        {
            cout << "Are you sure? Print [Y] to exit, others will return." << endl;
            read = "";
            getline(cin, read);
            read = clear(read, read.length());
            if (read == "y") // solution：strcasecmp的使用
            {
                exit_flag = true;
                continue;
            }
        }

        if (judge_CLO(read)) //判断Command Line Options
            continue;

        if (exp_flag) //解决注释问题
        {
            if (read.find("*/") != string::npos)
            {
                if (read.find("*/") + 2 < read.length())
                    read = read.substr(read.find("*/") + 2);
                if (read == "*/")
                    read = "";
                exp_flag = false;
            }
            else
                continue;
        }
        if (read.find("/*") == 0)
        {
            exp_flag = true;
            continue;
        }
        if (read.find("//") == 0 || read.find("#") == 0)
            continue;
        if (read.find("/*") != string::npos)
        {
            read = read.substr(0, read.find("/*") - 1);
            exp_flag = true;
        }
        if (read.find("//") != string::npos)
            read = read.substr(0, read.find("//") - 1);
        if (read.find("#") != string::npos)
            read = read.substr(0, read.find("#") - 1);

        if (!judge_br(read)) //判断括号完整
        {
            cout << "The last command is not executed!" << endl;
            continue;
        }

        if (judge_statement(read, read.length())) //判断statement
            continue;
        if (read[0] == '=' && read.length() > 1)
            read = read.substr(1);
        else if (read[0] == '=')
            continue;
        if (read[read.length() - 1] == '=' && read.length() > 1)
            read = read.substr(0, read.length() - 1);
        else if (read[read.length() - 1] == '=')
            continue;
        if (read.find("=") != string::npos)
        {
            cout << "The last command is not executed!" << endl;
            continue;
        }

        read = judge_Mathlib(read, read.length()); //消除Mathlib
        if (read == "")
            continue;

        if (judge_legal(read, read.length())) //判断式子合法
        {
            if (read[0] == '-')
                read = "0" + read; //转后缀式前的负号预处理
            for (int i = 1; i < read.length(); i++)
                if (read[i] == '-' && read[i - 1] == '(')
                    read = read.substr(0, i) + "0" + read.substr(i);
            // cout<<"已整理的式子："<<read<<endl;

            string postfix = getPostfix(read);
            // cout<<"后缀式："<<postfix<<endl;
            string result = expCalculate(postfix);
            cout << result << endl;
            continue;
        }

        cout << "The last command is not executed!" << endl;
    }
    bye();
}
