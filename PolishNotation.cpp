#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <io.h>
#include <fstream>
using namespace std;

class Node {
public:
    Node* next = nullptr;
    Node* prev = nullptr;
    string data;
};

class Stack {
public:
    void add(string c) {
        if (!head) {
            head = new Node;
            head->next = nullptr;
        }
        else {
            Node* prevHead = head;
            head = new Node;
            head->next = prevHead;
            prevHead->prev = head;
        }
        head->data = c;
        head->prev = nullptr;
        length++;
    }
    void del() {
        Node* next = head->next;
        delete head;
        head = next;
        if (head)
            head->prev = nullptr;
        length--;
    }
    void get() {
        if (!length) {
            cout << "Стэк пуст.\n";
            return;
        }
        cout << "Стэк: ";
        Node* curr = head;
        while (curr->next)
            curr = curr->next;
        while (curr) {
            cout << curr->data << ' ';
            curr = curr->prev;
        }
        cout << "\n\n";
    }
    int length;
    Node* head;
    Stack() {
        length = 0;
        head = nullptr;
    }
};

bool isNumber(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (!isdigit(s[i]) and s[i] != '-')
            return 0;
    }
    return 1;
}

void setTextColor(int textColorIndex) { // Смена цвета текста
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, (0 << 4) + textColorIndex);
}

string* resize(string* arr, size_t oldSize, size_t newSize) {
    string* temp = new string[min(oldSize, newSize)];
    for (int i = 0; i < min(oldSize, newSize); i++)
        temp[i] = arr[i];
    delete[] arr;
    arr = new string[newSize];
    for (int i = 0; i < min(oldSize, newSize); i++)
        arr[i] = temp[i];
    delete[] temp;
    return arr;
}

bool isOperand(char c) {
    switch (c) {
    case '(': case ')': case '+': case '-': case '*': case '/': case ' ': return 0;
    default: return 1;
    }
}

string* tokenizer(string expression, unsigned &counter) {
    size_t arrSize = 100;
    string *exprArr = new string[arrSize];
    string temp = "";
    for (int i = 0; i < expression.length(); i++) {
        if (isOperand(expression[i]) or (i == 0 and expression[i] == '-'))
            temp += expression[i];
        else {
            if (temp != "") {
                exprArr[counter] = temp;
                counter++;
                if (counter == arrSize) {
                    exprArr = resize(exprArr, arrSize, arrSize + 100);
                    arrSize += 100;
                }
                temp = "";
            }
            if (expression[i] != ' ' and expression[i] != '\t') {
                string exp = "";
                exp += expression[i];
                exprArr[counter] = exp;
                counter++;
                if (counter == arrSize) {
                    exprArr = resize(exprArr, arrSize, arrSize + 100);
                    arrSize += 100;
                }
            }
        }
    }
    if (temp != "") {
        exprArr[counter] = temp;
        counter++;
        temp = "";
    }
    exprArr = resize(exprArr, arrSize, counter);
    return exprArr;
}

void creationOutput(int current) {
    setTextColor(15);
    cout << "\tВыберите действие.\n";
    setTextColor(6 - (current == 1 ? 5 : 0));
    cout << "[1] ";
    setTextColor(15);
    cout << "Преобразование выражения." << '\n';
    setTextColor(6 - (current == 2 ? 5 : 0));
    cout << "[2] ";
    setTextColor(15);
    cout << "Проверка выражения." << '\n';
    setTextColor(6 - (current == 3 ? 5 : 0));
    cout << "[3] ";
    setTextColor(15);
    cout << "Подсчет выражения." << '\n';
    setTextColor(4 - (current == 4 ? 3 : 0));
    cout << "[ESC] ";
    setTextColor(15);
    cout << "Выход из программы." << '\n';
}

void transformOutput(int current) {
    setTextColor(15);
    cout << "\tКак преобразовать выржение?.\n";
    setTextColor(6 - (current == 1 ? 5 : 0));
    cout << "[1] ";
    setTextColor(15);
    cout << "Обратная польская нотация." << '\n';
    setTextColor(6 - (current == 2 ? 5 : 0));
    cout << "[2] ";
    setTextColor(15);
    cout << "Прямая польская нотация." << '\n';
    setTextColor(4 - (current == 3 ? 3 : 0));
    cout << "[ESC] ";
    setTextColor(15);
    cout << "Выход из программы." << '\n';
}

void checkCountOutput(int current) {
    setTextColor(15);
    cout << "\tКак записано выржение?.\n";
    setTextColor(6 - (current == 1 ? 5 : 0));
    cout << "[1] ";
    setTextColor(15);
    cout << "Обратная польская нотация." << '\n';
    setTextColor(6 - (current == 2 ? 5 : 0));
    cout << "[2] ";
    setTextColor(15);
    cout << "Прямая польская нотация." << '\n';
    setTextColor(6 - (current == 3 ? 5 : 0));
    cout << "[3] ";
    setTextColor(15);
    cout << "Простое выражение." << '\n';
    setTextColor(4 - (current == 4 ? 3 : 0));
    cout << "[ESC] ";
    setTextColor(15);
    cout << "Выход из программы." << '\n';
}

unsigned priority(string c) {
    switch (c[0]) {
    case '(': return 1;
    case '+': return 2;
    case '-': return 2;
    case '*': return 3;
    case '/': return 3;
    default: return 0;
    }
}

void errors(int errNum) {
    switch (errNum) {
    case 1: 
        cout << "Error: Uncorrect order of operands and operators.\n";
        break;
    case 2:
        cout << "Error: No match for ""("" or "")"".\n";
        break;
    case 3:
        cout << "Error: Too many operators or operands.\n";
        break;
    case 4:
        cout << "Error: Unknown error.\n";
        break;
    case 0:
        cout << "Expression is correct!\n";
        break;
    }
}

unsigned stdCheck(string expression) {
    // 0 - OK
    // 1 - Uncorrect order of operands and operators
    // 2 - No match for "(" or ")"
    // 3 - Too many operators or operands
    unsigned lp = 0;
    unsigned rp = 0;
    unsigned operators = 0;
    unsigned operands = 0;
    bool switchOp = 1;   // 0 - operand; 1 - operator
    unsigned counter = 0;
    string temp = "";
    string *exprArr = new string[1000];
    for (int i = 0; i < expression.length(); i++) {
        if (isOperand(expression[i]) or (i == 0 and expression[i] == '-'))
            temp += expression[i];
        else {
            if (temp != "") {
                if (!switchOp)
                {
                    delete[] exprArr;
                    return 1;
                }
                exprArr[counter] = temp;
                counter++;
                temp = "";
                switchOp = 0;
            }
            if (expression[i] != ' ' and expression[i] != '\t') {
                if (expression[i] != '(') {
                    if (switchOp)
                    {
                        delete[] exprArr;
                        return 1;
                    }
                }
                else {
                    if (!switchOp) {
                        exprArr[counter] = "*";
                        counter++;
                    }
                }
                exprArr[counter] = expression[i];
                counter++;
                if (expression[i] != ')')
                    switchOp = 1;
            }
        }
    }
    if (temp != "") {
        if (!switchOp)
            return 1;
        exprArr[counter] = temp;
        counter++;
        temp = "";
        switchOp = 0;
    }
    for (int i = 0; i < counter; i++) {
        if (exprArr[i] == "(")
            lp++;
        else if (exprArr[i] == ")")
            rp++;
        else if (exprArr[i] == "+" or exprArr[i] == "-" or exprArr[i] == "*" or exprArr[i] == "/")
            operators++;
        else
            operands++;
    }
    if (lp != rp)
    {
        delete[] exprArr;
        return 2;
    }
    if (operators + 1 != operands)
    {
        delete[] exprArr;
        return 3;
    }
    delete[] exprArr;
    return 0;
}

string operation(string n1, string n2, string op) {
    int num1 = stoi(n1);
    int num2 = stoi(n2);
    char oper = op[0];
    cout << "Вычисляем выражение \"" << n1 + ' ' + op + ' ' + n2 << "\", удаляем значения из стэка и добавляем результат в стэк.\n";
    switch (oper) {
    case '*':
        return to_string(num1 * num2);
    case '/':
        return to_string(num1 / num2);
    case '+':
        return to_string(num1 + num2);
    case '-':
        return to_string(num1 - num2);
    }
}

unsigned rpnCheck(string expression) {
    Stack stack;
    unsigned counter = 0;
    string* rpn = tokenizer(expression, counter);
    for (int i = 0; i < counter; i++) {
        if (isOperand(rpn[i][0]))
            stack.add(rpn[i]);
        else {
            if (stack.length > 1)
                stack.del();
            else
                return 4;
        }
    }
    if (stack.length != 1)
        return 4;
    return 0;
}

unsigned dpnCheck(string expression) {
    Stack stack;
    unsigned counter = 0;
    string* rpn = tokenizer(expression, counter);
    for (int i = counter - 1; i >= 0; i--) {
        if (isOperand(rpn[i][0]))
            stack.add(rpn[i]);
        else {
            if (stack.length > 1)
                stack.del();
            else
                return 4;
        }
    }
    if (stack.length != 1)
        return 4;
    return 0;
}

void showArr(string* arr, unsigned length) {
    cout << "Выражение: ";
    for (int i = 0; i < length; i++)
        cout << arr[i] << ' ';
    cout << '\n';
}

string stdToRpn(string expression, bool show) {
    unsigned counter = 0;
    string* exprArr = tokenizer(expression, counter);
    unsigned arrCounter = 0;
    string revPolNot[100];
    Stack stack;
    for (int i = 0; i < counter; i++) {
        if (!(exprArr[i].length() == 1 and !isOperand(exprArr[i][0]))) {
            revPolNot[arrCounter] = exprArr[i];
            arrCounter++;
            if (show) {
                cout << '\n' << exprArr[i] << " - число/переменная, добавляем в выражение.\n";
                showArr(revPolNot, arrCounter);
            }
        }
        else {
            if (exprArr[i] == "("){
                stack.add(exprArr[i]);
                if (show) {
                    cout << '\n' << exprArr[i] << " - левая скобка, добавляем в стэк.\n";
                    stack.get();
                }
            }
            else if (exprArr[i] == ")") {
                while (stack.head->data != "(") {
                    revPolNot[arrCounter] = stack.head->data;
                    arrCounter++;
                    stack.del();
                }
                stack.del();
                if (show){
                    cout << '\n' << exprArr[i] << " - правая скобка, удаляем из стэка всё до левой скобки включительно.\n";
                    stack.get();
                    showArr(revPolNot, arrCounter);
                }
            }
            else {
                if (stack.length == 0 or priority(stack.head->data) < priority(exprArr[i])){
                    stack.add(exprArr[i]);
                    if (show){
                        cout << '\n' << exprArr[i] << " - оператор, приоритет которого выше оператора в стэке, добавляем в стэк.\n";
                        stack.get();
                    }
                }
                else
                {
                    while (stack.length > 0 and priority(stack.head->data) >= priority(exprArr[i])) {
                        revPolNot[arrCounter] = stack.head->data;
                        arrCounter++;
                        stack.del();
                    }
                    stack.add(exprArr[i]);
                    if (show) {
                        cout << '\n' << exprArr[i] << " - оператор, приоритет которого ниже оператора в стэке, удаляем операторы, добавляем в стэк текущий оператор.\n";
                        stack.get();
                        showArr(revPolNot, arrCounter);
                    }
                }
            }
        }
    }
    if (show) {
        if (stack.length > 0)
            cout << "\nУдаляем из стека все операторы.\n";
    }
    while (stack.head) {
        revPolNot[arrCounter] = stack.head->data;
        arrCounter++;
        stack.del();
    }
    if (show) {
        stack.get();
    }
    string answer = "";
    for (int i = 0; i < arrCounter; i++)
        answer = answer + revPolNot[i] + ' ';
    return answer;
}

string stdToDpn(string expression, bool show) {
    unsigned counter = 0;
    string* exprArr = tokenizer(expression, counter);
    unsigned arrCounter = 0;
    string dirPolNot[100];
    Stack stack;
    for (int i = counter - 1; i >= 0; i--) {
        if (!(exprArr[i].length() == 1 and !isOperand(exprArr[i][0]))) {
            dirPolNot[arrCounter] = exprArr[i];
            arrCounter++;
            if (show) {
                cout << '\n' << exprArr[i] << " - число/переменная, добавляем в выражение.\n";
                showArr(dirPolNot, arrCounter);
            }
        }
        else {
            if (exprArr[i] == ")"){
                stack.add(exprArr[i]);
                if (show) {
                    cout << '\n' << exprArr[i] << " - правая скобка, добавляем в стэк.\n";
                    stack.get();
                }
            }
            else if (exprArr[i] == "(") {
                while (stack.head->data != ")") {
                    dirPolNot[arrCounter] = stack.head->data;
                    arrCounter++;
                    stack.del();
                }
                stack.del();
                if (show) {
                    cout << '\n' << exprArr[i] << " - левая скобка, удаляем из стэка всё до правой скобки включительно.\n";
                    stack.get();
                    showArr(dirPolNot, arrCounter);
                }
            }
            else {
                if (stack.length == 0 or priority(stack.head->data) < priority(exprArr[i])){
                    stack.add(exprArr[i]);
                    if (show) {
                        cout << '\n' << exprArr[i] << " - оператор, приоритет которого выше оператора в стэке, добавляем в стэк.\n";
                        stack.get();
                    }
                }
                else
                {
                    while (stack.length > 0 and priority(stack.head->data) >= priority(exprArr[i])) {
                        dirPolNot[arrCounter] = stack.head->data;
                        arrCounter++;
                        stack.del();
                    }
                    stack.add(exprArr[i]);
                    if (show) {
                        cout << '\n' << exprArr[i] << " - оператор, приоритет которого ниже оператора в стэке, удаляем операторы, добавляем в стэк текущий оператор.\n";
                        stack.get();
                        showArr(dirPolNot, arrCounter);
                    }
                }
            }
        }
    }
    if (show and stack.length > 0)
        cout << "\nУдаляем из стека все операторы.\n";
    while (stack.head) {
        dirPolNot[arrCounter] = stack.head->data;
        arrCounter++;
        stack.del();
    }
    if (show) {
        stack.get();
    }
    string answer = "";
    for (int i = 0; i < arrCounter; i++)
        answer = dirPolNot[i] + ' ' + answer;
    return answer;
}

string calcRpn(string expression) {
    Stack stack;
    unsigned counter = 0;
    string* rpn = tokenizer(expression, counter);
    for (int i = 0; i < counter; i++) {
        if (!(rpn[i].length() == 1 and !isOperand(rpn[i][0]))) {
            if (!isNumber(rpn[i])) {
                cout << "Введите значение " << rpn[i] << ": ";
                cin >> rpn[i];
            }
        }
    }
    for (int i = 0; i < counter; i++) {
        if (!(rpn[i].length() == 1 and !isOperand(rpn[i][0]))) {
            stack.add(rpn[i]);
            cout << '\n' << rpn[i] << " - число, добавляем в стэк.\n";
            stack.get();
        }
        else {
            string value1 = stack.head->next->data;
            string value2 = stack.head->data;
            stack.del();
            stack.head->data = operation(value1, value2, rpn[i]);
            stack.get();
        }
    }
    string answer = stack.head->data;
    stack.del();
    return answer;
}

string calcDpn(string expression) {
    Stack stack;
    unsigned counter = 0;
    string* dpn = tokenizer(expression, counter);
    for (int i = 0; i < counter; i++) {
        if (!(dpn[i].length() == 1 and !isOperand(dpn[i][0]))) {
            if (!isNumber(dpn[i])) {
                cout << "Введите значение " << dpn[i] << ": ";
                cin >> dpn[i];
            }
        }
    }
    for (int i = counter - 1; i >= 0; i--) {
        if (!(dpn[i].length() == 1 and !isOperand(dpn[i][0]))) {
            stack.add(dpn[i]);
            cout << '\n' << dpn[i] << " - число, добавляем в стэк.\n";
            stack.get();
        }
        else {
            string value1 = stack.head->data;
            string value2 = stack.head->next->data;
            stack.del();
            stack.head->data = operation(value1, value2, dpn[i]);
            stack.get();
        }
    }
    string answer = stack.head->data;
    stack.del();
    return answer;
}

int output(void outputType(int)) {
    int currentNum = 1;
    unsigned button;
    outputType(currentNum);
    while (TRUE) {
        button = _getch();
        button = (button == 224) ? _getch() : button;
        if (button == 80 || button == 77) {           // Управление стрелками движения вниз
            system("cls");
            currentNum += 1;
            currentNum = currentNum == 5 ? 1 : currentNum;
            outputType(currentNum);
        }
        else if (button == 72 || button == 75) {      // Управление стрелками движения вверх
            system("cls");
            currentNum -= 1;
            currentNum = currentNum == 0 ? 4 : currentNum;
            outputType(currentNum);

        }
        else if (button >= 49 && button <= 51) {      // Управление цифрами
            system("cls");
            currentNum = button - 48;
            outputType(currentNum);
            system("cls");
            return currentNum;
        }
        else if (button == 13) {   // Нажат enter
            if (currentNum != 4)   // Нажат не enter+[esc]
            {
                system("cls");
                return currentNum;
            }
            else                   // Нажат enter+[esc]
            {
                system("cls");
                if (outputType == creationOutput)
                    exit(0);
                return 0;
            }
        }
        else if (button == 27)  // Нажат esc
        {
            system("cls");
            if (outputType == creationOutput)
                exit(0);
            return 0;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "");
    int currentNum;
    while (TRUE) {
        currentNum = output(creationOutput);
        cout << "\tВведите выражение: ";
        string expression = "";
        if (_access("pw3.txt", 0) == -1) {
            char c = cin.get();
            while (c != '\n') {
                expression += c;
                c = cin.get();
            }
        }
        else {
            ifstream file;
            file.open("pw3.txt");
            getline(file, expression);
        }
        system("cls");

        if (expression == "") {
            cout << "Ошибка! Неверный ввод.\n";
            exit(-1);
        }

        switch (currentNum) {
        case 1:
            currentNum = output(transformOutput);
            switch (currentNum) {
            case 1:
                cout << stdToRpn(expression, 1) << '\n';
                break;
            case 2:
                cout << stdToDpn(expression, 1) << '\n';
                break;
            }
            system("pause");
            break;
        case 2:
            currentNum = output(checkCountOutput);
            switch (currentNum) {
            case 1:
                errors(rpnCheck(expression));
                break;
            case 2:
                errors(dpnCheck(expression));
                break;
            case 3:
                errors(stdCheck(expression));
                break;
            }
            system("pause");
            break;
        case 3:
            currentNum = output(checkCountOutput);
            switch (currentNum) {
            case 1:
                if (!rpnCheck(expression))
                    cout << expression << " = " << calcRpn(expression) << '\n';
                else
                    errors(rpnCheck(expression));
                break;
            case 2:
                if (!dpnCheck(expression))
                    cout << expression << " = " << calcDpn(expression) << '\n';
                else
                    errors(dpnCheck(expression));
                break;
            case 3:
                string expressionRpn = stdToRpn(expression, 0);
                if (!stdCheck(expression)){
                    cout << "Выражение в обратной польской нотации: " << expressionRpn << "\n\n";
                    cout << expression << " = " << calcRpn(expressionRpn) << '\n';
                }
                else
                    errors(stdCheck(expression));
                break;
            }
            system("pause");
            break;
        }
        system("cls");
    }
}
