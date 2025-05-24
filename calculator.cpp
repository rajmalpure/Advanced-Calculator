#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <stdexcept>

class AdvancedCalculator {
private:
    double memory;
    std::vector<double> history;
    bool degrees; // true for degrees, false for radians
    
    // Helper function to check if character is an operator
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
    }
    
    // Get operator precedence
    int getPrecedence(char op) {
        switch(op) {
            case '+':
            case '-': return 1;
            case '*':
            case '/': return 2;
            case '^': return 3;
            default: return 0;
        }
    }
    
    // Convert infix to postfix notation
    std::vector<std::string> infixToPostfix(const std::string& expression) {
        std::vector<std::string> postfix;
        std::stack<char> operators;
        std::string number = "";
        
        for (size_t i = 0; i < expression.length(); i++) {
            char c = expression[i];
            
            if (std::isspace(c)) continue;
            
            if (std::isdigit(c) || c == '.') {
                number += c;
            } else {
                if (!number.empty()) {
                    postfix.push_back(number);
                    number = "";
                }
                
                if (isOperator(c)) {
                    while (!operators.empty() && 
                           getPrecedence(operators.top()) >= getPrecedence(c) &&
                           operators.top() != '(') {
                        postfix.push_back(std::string(1, operators.top()));
                        operators.pop();
                    }
                    operators.push(c);
                } else if (c == '(') {
                    operators.push(c);
                } else if (c == ')') {
                    while (!operators.empty() && operators.top() != '(') {
                        postfix.push_back(std::string(1, operators.top()));
                        operators.pop();
                    }
                    if (!operators.empty()) operators.pop(); // Remove '('
                }
            }
        }
        
        if (!number.empty()) {
            postfix.push_back(number);
        }
        
        while (!operators.empty()) {
            postfix.push_back(std::string(1, operators.top()));
            operators.pop();
        }
        
        return postfix;
    }
    
    // Evaluate postfix expression
    double evaluatePostfix(const std::vector<std::string>& postfix) {
        std::stack<double> operands;
        
        for (const std::string& token : postfix) {
            if (token.length() == 1 && isOperator(token[0])) {
                if (operands.size() < 2) {
                    throw std::runtime_error("Invalid expression");
                }
                
                double b = operands.top(); operands.pop();
                double a = operands.top(); operands.pop();
                
                switch (token[0]) {
                    case '+': operands.push(a + b); break;
                    case '-': operands.push(a - b); break;
                    case '*': operands.push(a * b); break;
                    case '/': 
                        if (b == 0) throw std::runtime_error("Division by zero");
                        operands.push(a / b); 
                        break;
                    case '^': operands.push(std::pow(a, b)); break;
                }
            } else {
                operands.push(std::stod(token));
            }
        }
        
        if (operands.size() != 1) {
            throw std::runtime_error("Invalid expression");
        }
        
        return operands.top();
    }
    
    double toRadians(double degrees) {
        return degrees * M_PI / 180.0;
    }
    
    double toDegrees(double radians) {
        return radians * 180.0 / M_PI;
    }

public:
    AdvancedCalculator() : memory(0), degrees(true) {}
    
    // Basic arithmetic operations
    double add(double a, double b) { return a + b; }
    double subtract(double a, double b) { return a - b; }
    double multiply(double a, double b) { return a * b; }
    double divide(double a, double b) {
        if (b == 0) throw std::runtime_error("Division by zero");
        return a / b;
    }
    double power(double base, double exponent) { return std::pow(base, exponent); }
    double squareRoot(double x) {
        if (x < 0) throw std::runtime_error("Square root of negative number");
        return std::sqrt(x);
    }
    
    // Trigonometric functions
    double sine(double x) {
        return degrees ? std::sin(toRadians(x)) : std::sin(x);
    }
    double cosine(double x) {
        return degrees ? std::cos(toRadians(x)) : std::cos(x);
    }
    double tangent(double x) {
        return degrees ? std::tan(toRadians(x)) : std::tan(x);
    }
    double arcsine(double x) {
        if (x < -1 || x > 1) throw std::runtime_error("Invalid input for arcsine");
        double result = std::asin(x);
        return degrees ? toDegrees(result) : result;
    }
    double arccosine(double x) {
        if (x < -1 || x > 1) throw std::runtime_error("Invalid input for arccosine");
        double result = std::acos(x);
        return degrees ? toDegrees(result) : result;
    }
    double arctangent(double x) {
        double result = std::atan(x);
        return degrees ? toDegrees(result) : result;
    }
    
    // Logarithmic functions
    double naturalLog(double x) {
        if (x <= 0) throw std::runtime_error("Logarithm of non-positive number");
        return std::log(x);
    }
    double log10(double x) {
        if (x <= 0) throw std::runtime_error("Logarithm of non-positive number");
        return std::log10(x);
    }
    double exponential(double x) { return std::exp(x); }
    
    // Other mathematical functions
    double factorial(int n) {
        if (n < 0) throw std::runtime_error("Factorial of negative number");
        if (n > 20) throw std::runtime_error("Factorial too large");
        double result = 1;
        for (int i = 2; i <= n; i++) {
            result *= i;
        }
        return result;
    }
    double absolute(double x) { return std::abs(x); }
    double ceiling(double x) { return std::ceil(x); }
    double floor(double x) { return std::floor(x); }
    
    // Memory operations
    void memoryStore(double value) { memory = value; }
    double memoryRecall() { return memory; }
    void memoryClear() { memory = 0; }
    void memoryAdd(double value) { memory += value; }
    void memorySubtract(double value) { memory -= value; }
    
    // History operations
    void addToHistory(double value) { 
        history.push_back(value); 
        if (history.size() > 20) { // Keep only last 20 results
            history.erase(history.begin());
        }
    }
    void showHistory() {
        std::cout << "\n=== CALCULATION HISTORY ===\n";
        if (history.empty()) {
            std::cout << "No calculations in history.\n";
        } else {
            for (size_t i = 0; i < history.size(); i++) {
                std::cout << i + 1 << ". " << std::fixed << std::setprecision(6) 
                         << history[i] << std::endl;
            }
        }
        std::cout << "===========================\n\n";
    }
    void clearHistory() { 
        history.clear(); 
        std::cout << "History cleared.\n";
    }
    
    // Settings
    void toggleAngleMode() {
        degrees = !degrees;
        std::cout << "Angle mode set to: " << (degrees ? "Degrees" : "Radians") << std::endl;
    }
    
    // Expression evaluation
    double evaluateExpression(const std::string& expression) {
        try {
            std::vector<std::string> postfix = infixToPostfix(expression);
            return evaluatePostfix(postfix);
        } catch (const std::exception& e) {
            throw std::runtime_error("Error evaluating expression: " + std::string(e.what()));
        }
    }
    
    // Main menu display
    void showMenu() {
        std::cout << "\n================= ADVANCED CALCULATOR =================\n";
        std::cout << "Current mode: " << (degrees ? "Degrees" : "Radians") << std::endl;
        std::cout << "Memory: " << std::fixed << std::setprecision(6) << memory << std::endl;
        std::cout << "======================================================\n";
        std::cout << "1.  Basic Operations (+, -, *, /, ^)\n";
        std::cout << "2.  Square Root\n";
        std::cout << "3.  Trigonometric Functions (sin, cos, tan)\n";
        std::cout << "4.  Inverse Trigonometric Functions (asin, acos, atan)\n";
        std::cout << "5.  Logarithmic Functions (ln, log10)\n";
        std::cout << "6.  Exponential Function (e^x)\n";
        std::cout << "7.  Factorial\n";
        std::cout << "8.  Absolute Value\n";
        std::cout << "9.  Ceiling/Floor\n";
        std::cout << "10. Memory Operations (MS, MR, MC, M+, M-)\n";
        std::cout << "11. Expression Evaluation\n";
        std::cout << "12. History\n";
        std::cout << "13. Toggle Angle Mode (Degrees/Radians)\n";
        std::cout << "14. Clear Screen\n";
        std::cout << "0.  Exit\n";
        std::cout << "======================================================\n";
        std::cout << "Enter your choice: ";
    }
    
    void run() {
        int choice;
        double a, b, result;
        std::string expression;
        
        std::cout << "Welcome to the Advanced Calculator!\n";
        
        while (true) {
            showMenu();
            
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input! Please enter a number.\n";
                continue;
            }
            
            try {
                switch (choice) {
                    case 0:
                        std::cout << "Thank you for using the Advanced Calculator!\n";
                        return;
                        
                    case 1:
                        std::cout << "Enter first number: ";
                        std::cin >> a;
                        std::cout << "Enter operator (+, -, *, /, ^): ";
                        char op;
                        std::cin >> op;
                        std::cout << "Enter second number: ";
                        std::cin >> b;
                        
                        switch (op) {
                            case '+': result = add(a, b); break;
                            case '-': result = subtract(a, b); break;
                            case '*': result = multiply(a, b); break;
                            case '/': result = divide(a, b); break;
                            case '^': result = power(a, b); break;
                            default: 
                                std::cout << "Invalid operator!\n";
                                continue;
                        }
                        std::cout << "Result: " << std::fixed << std::setprecision(6) << result << std::endl;
                        addToHistory(result);
                        break;
                        
                    case 2:
                        std::cout << "Enter number: ";
                        std::cin >> a;
                        result = squareRoot(a);
                        std::cout << "√" << a << " = " << std::fixed << std::setprecision(6) << result << std::endl;
                        addToHistory(result);
                        break;
                        
                    case 3:
                        std::cout << "Enter angle: ";
                        std::cin >> a;
                        std::cout << "sin(" << a << ") = " << std::fixed << std::setprecision(6) << sine(a) << std::endl;
                        std::cout << "cos(" << a << ") = " << std::fixed << std::setprecision(6) << cosine(a) << std::endl;
                        std::cout << "tan(" << a << ") = " << std::fixed << std::setprecision(6) << tangent(a) << std::endl;
                        break;
                        
                    case 4:
                        std::cout << "Enter value (-1 to 1): ";
                        std::cin >> a;
                        std::cout << "asin(" << a << ") = " << std::fixed << std::setprecision(6) << arcsine(a) << std::endl;
                        std::cout << "acos(" << a << ") = " << std::fixed << std::setprecision(6) << arccosine(a) << std::endl;
                        std::cout << "atan(" << a << ") = " << std::fixed << std::setprecision(6) << arctangent(a) << std::endl;
                        break;
                        
                    case 5:
                        std::cout << "Enter positive number: ";
                        std::cin >> a;
                        std::cout << "ln(" << a << ") = " << std::fixed << std::setprecision(6) << naturalLog(a) << std::endl;
                        std::cout << "log10(" << a << ") = " << std::fixed << std::setprecision(6) << log10(a) << std::endl;
                        break;
                        
                    case 6:
                        std::cout << "Enter exponent: ";
                        std::cin >> a;
                        result = exponential(a);
                        std::cout << "e^" << a << " = " << std::fixed << std::setprecision(6) << result << std::endl;
                        addToHistory(result);
                        break;
                        
                    case 7:
                        int n;
                        std::cout << "Enter non-negative integer: ";
                        std::cin >> n;
                        result = factorial(n);
                        std::cout << n << "! = " << std::fixed << std::setprecision(0) << result << std::endl;
                        addToHistory(result);
                        break;
                        
                    case 8:
                        std::cout << "Enter number: ";
                        std::cin >> a;
                        result = absolute(a);
                        std::cout << "|" << a << "| = " << std::fixed << std::setprecision(6) << result << std::endl;
                        addToHistory(result);
                        break;
                        
                    case 9:
                        std::cout << "Enter number: ";
                        std::cin >> a;
                        std::cout << "ceiling(" << a << ") = " << ceiling(a) << std::endl;
                        std::cout << "floor(" << a << ") = " << floor(a) << std::endl;
                        break;
                        
                    case 10: {
                        std::cout << "Memory Operations:\n";
                        std::cout << "1. Store (MS)\n2. Recall (MR)\n3. Clear (MC)\n4. Add (M+)\n5. Subtract (M-)\n";
                        std::cout << "Choose: ";
                        int memChoice;
                        std::cin >> memChoice;
                        
                        switch (memChoice) {
                            case 1:
                                std::cout << "Enter value to store: ";
                                std::cin >> a;
                                memoryStore(a);
                                std::cout << "Value stored in memory.\n";
                                break;
                            case 2:
                                std::cout << "Memory value: " << std::fixed << std::setprecision(6) << memoryRecall() << std::endl;
                                break;
                            case 3:
                                memoryClear();
                                std::cout << "Memory cleared.\n";
                                break;
                            case 4:
                                std::cout << "Enter value to add: ";
                                std::cin >> a;
                                memoryAdd(a);
                                std::cout << "Value added to memory. New value: " << std::fixed << std::setprecision(6) << memoryRecall() << std::endl;
                                break;
                            case 5:
                                std::cout << "Enter value to subtract: ";
                                std::cin >> a;
                                memorySubtract(a);
                                std::cout << "Value subtracted from memory. New value: " << std::fixed << std::setprecision(6) << memoryRecall() << std::endl;
                                break;
                            default:
                                std::cout << "Invalid choice!\n";
                        }
                        break;
                    }
                    
                    case 11:
                        std::cin.ignore(); // Clear input buffer
                        std::cout << "Enter expression (use +, -, *, /, ^, parentheses): ";
                        std::getline(std::cin, expression);
                        result = evaluateExpression(expression);
                        std::cout << "Result: " << std::fixed << std::setprecision(6) << result << std::endl;
                        addToHistory(result);
                        break;
                        
                    case 12: {
                        std::cout << "History Options:\n1. Show History\n2. Clear History\nChoose: ";
                        int histChoice;
                        std::cin >> histChoice;
                        if (histChoice == 1) {
                            showHistory();
                        } else if (histChoice == 2) {
                            clearHistory();
                        } else {
                            std::cout << "Invalid choice!\n";
                        }
                        break;
                    }
                    
                    case 13:
                        toggleAngleMode();
                        break;
                        
                    case 14:
                        #ifdef _WIN32
                            system("cls");
                        #else
                            system("clear");
                        #endif
                        break;
                        
                    default:
                        std::cout << "Invalid choice! Please try again.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            
            if (choice != 0 && choice != 14) {
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
            }
        }
    }
};

int main() {
    AdvancedCalculator calculator;
    calculator.run();
    return 0;
}