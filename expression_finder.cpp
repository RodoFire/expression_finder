#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <valarray>
#include <iomanip>
#include <sstream>
#include <queue>
#include <stack>

using namespace std;

/**@brief based on Shunting Yard algorithm.
 * <p> implemented algorithm by
*  <a href="https://github.com/ozgekaracam/Shunting-Yard/blob/main/shunting_yard.cpp">GitHub: ozgekaracam/Shunting-Yard</a>
*/
class Token {
public:
    enum class Type {
        Unknown,
        Number,
        LeftP,
        RightP,
        Operator
    };

    Type type;
    int precedence;
    string elementStr;
};

deque<Token> GetTokens(string expressionLine) {
    double doubleNr;
    string inputElement;
    Token token;
    deque<Token> tokenStack;

    istringstream iss(expressionLine);
    while (iss) {
        iss >> inputElement;
        if (iss) {
            token.elementStr = inputElement;
            token.precedence = -1;
            try {
                doubleNr = stod(inputElement);
                token.type = Token::Type::Number;
            } catch (const invalid_argument &) {
                token.type = Token::Type::Unknown;
            }
            if (token.type == Token::Type::Unknown) {
                if (token.elementStr == "(") {
                    token.type = Token::Type::LeftP;
                } else if (token.elementStr == ")") {
                    token.type = Token::Type::RightP;
                } else {
                    token.type = Token::Type::Operator;
                    if (token.elementStr == "*" || token.elementStr == "/" || token.elementStr == "%")
                        token.precedence = 3;
                    else token.precedence = 2; // + and -
                }
            }
            tokenStack.push_back(token);
        } else break;
    }
    return tokenStack;
}

string FixPrecision(string x) {
    ostringstream strout;

    double doubleNr = stod(x);
    strout << setprecision(10) << doubleNr;
    string str = strout.str();
    size_t end = str.find_last_not_of('.') + 1;
    str.erase(end);
    return str;
}

queue<Token> ShuntingYard(deque<Token> tokenStack) {
    queue<Token> outputQueue;
    stack<Token> operatorStack;

    while (!tokenStack.empty()) {
        Token token = tokenStack.front();
        tokenStack.pop_front();
        if (token.type == Token::Type::Number) {
            token.elementStr = FixPrecision(token.elementStr);
            outputQueue.push(token);
        } else if (token.type == Token::Type::Operator) {
            const Token &o1 = token;
            while (!operatorStack.empty()) {
                Token o2 = operatorStack.top();
                if (o2.type != Token::Type::LeftP && (o1.precedence <= o2.precedence)) {
                    operatorStack.pop();
                    outputQueue.push(o2);
                } else break;
            }
            operatorStack.push(o1);
        } else if (token.type == Token::Type::LeftP) {
            operatorStack.push(token);
        } else if (token.type == Token::Type::RightP) {
            while (!operatorStack.empty() && operatorStack.top().type != Token::Type::LeftP) {
                outputQueue.push(operatorStack.top());
                operatorStack.pop();
            }
            if (operatorStack.top().type == Token::Type::LeftP) {
                operatorStack.pop();
            }
        } else break;
    }
    while (!operatorStack.empty()) {
        if (operatorStack.top().type != Token::Type::LeftP) {
            outputQueue.push(operatorStack.top());
            operatorStack.pop();
        } else break;
    }
    return outputQueue;
}

double CalculateResult(queue<Token> outputQueue) {
    vector<double> operandsV;

    while (!outputQueue.empty()) {
        Token token = outputQueue.front();
        outputQueue.pop();
        if (token.type == Token::Type::Number) {
            operandsV.push_back(stod(token.elementStr));
        }
        if (token.type == Token::Type::Operator) {
            double operandR = operandsV.back();
            operandsV.pop_back();
            double operandL = operandsV.back();
            operandsV.pop_back();
            if (token.elementStr == "*") {
                operandsV.push_back(operandL * operandR);
            }
            if (token.elementStr == "/") {
                operandsV.push_back(operandL / operandR);
            }
            if (token.elementStr == "%") {
                operandsV.push_back((double) ((int) operandL % (int) operandR));
            }
            if (token.elementStr == "+") {
                operandsV.push_back(operandL + operandR);
            }
            if (token.elementStr == "-") {
                operandsV.push_back(operandL - operandR);
            }
        }
    }
    return operandsV[0];
}

char *convert_to_operators_array(const string &input) {
    set<char> operators;
    for (char c: input) {
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
            operators.insert(c);
        }
    }
    char *result = new char[input.length() + 1];
    int index = 0;
    for (char c: input) {
        result[index++] = c;
    }
    result[input.length()] = '\0';
    return result;
}

vector<int> convert_to_int_array(string input) {
    vector<int> output;
    string lastValue;

    for (int i = 0; i < input.length(); i++) {
        const char c = input.at(i);

        if (c == ' ') {
            if (!lastValue.empty()) {
                output.push_back(stoi(lastValue));
                lastValue = "";
            }
        }
        if (c >= '0' && c <= '9') {
            lastValue += c;
        }
        if (i == input.length() - 1) {
            output.push_back(stoi(lastValue));
        }
    }
    return output;
}

/**
 * main method that will find every possible equation arrangement
 * @param numbers numbers used for the equation
 * @param operators the possible operators types
 * @param expectedResult the wanted value
 * @param round will print values close to the expectedResult
 * @param allResult define if the program should continue to run to find all possible solutions or stop after finding the first
 */
void manage_equation(vector<int> numbers,
                     const char *operators,
                     const int expectedResult,
                     const int round,
                     const bool allResult) {
    ranges::sort(numbers);
    cout << "finding started, \n\t- numbers used: [";
    for (const int number: numbers) {
        cout << " " << number;
    }
    cout << "], \n\t- operators used: [";

    int operatorsSize = 0;
    for (; operators[operatorsSize] != '\0'; operatorsSize++) {
        cout << " " << operators[operatorsSize];
    }
    cout << "], \n\t- expected result: " << expectedResult << "\n\t- round: " << round << "\n\t- allResult: " <<
            allResult << "\n";


    do {
        const int opCount = numbers.size() - 1;
        const int maxOpComb = pow(4, opCount);

        for (int i = 0; i < maxOpComb; i++) {
            stringstream expression;
            int temp = i;

            for (size_t j = 0; j < numbers.size(); j++) {
                expression << numbers[j];
                if (j < opCount) {
                    expression << " " << operators[temp % operatorsSize] << " ";
                    temp /= operatorsSize;
                }
            }

            string exprStr = expression.str();
            const deque<Token> tokens = GetTokens(exprStr);
            const queue<Token> rpn = ShuntingYard(tokens);
            const double result = CalculateResult(rpn);

            if (result == expectedResult) {
                cout << "\n" << " +-----------------------------------------------------|" << "\n";
                cout << " | Found value: " << exprStr << " = " << result << "  |" << "\n";
                cout << " +-----------------------------------------------------|" << "\n" << endl;
                if (!allResult)
                    return;
            }
            if (result < static_cast<float>(expectedResult) * (1 + static_cast<float>(round) / 100) &&
                result > static_cast<float>(expectedResult) * (1 - static_cast<float>(round) / 100)) {
                cout << "    | Close value: " << exprStr << " = " << result << " (" << abs(result) /
                        expectedResult * 100 << "%)" << endl;
            }
        }
    } while (ranges::next_permutation(numbers).found);
}

int main() {
    char exit = ' ';
    do {
        bool allResult = false;
        int expectedResult;
        int round;
        string inputNumbers;
        string operators;

        cout << "Enter the values to be used: " << endl;
        getline(cin, inputNumbers);
        const vector<int> numbers = convert_to_int_array(inputNumbers);

        cout << "Enter the wanted result: " << endl;
        cin >> expectedResult;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter the wanted operators (+ | - | * | / | %): " << endl;
        getline(cin, operators);
        const char *operatorsArray = convert_to_operators_array(operators);


        cout << "Print similar results? (0: no, or enter percentage tolerance, e.g. 5 for +-5%)" << endl;
        cin >> round;

        char stop;
        cout << "Stop after finding one result: (y/n)" << endl;
        cin >> stop;

        allResult = !(stop == 'Y' || stop == 'y');

        manage_equation(numbers, operatorsArray, expectedResult, round, allResult);

        cout << "do you want to quit the program? (y -> quit, else, it restart)" << endl;
        cin >> exit;

        delete[] operatorsArray;
    } while (exit != 'y' && exit != 'Y');

    return 0;
}
