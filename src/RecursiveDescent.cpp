/*
* Authors: John Bouchard, Adam Jeniski

    Given a string consisting of parentheses, 
    single digits, and positive and negative signs, 
    convert the string into a mathematical expression to obtain the answer.

    Don't use eval or a similar built-in parser.

    For example, given '-1 + (2 + 3)', you should return 4.
*/



//Grammar:
// S = statement, E = expression, A = assignment, T = term,
// F = factor, V = value, id = id
//    S -> E
//    S -> A
//    A -> id = E
//    E -> ( E )
//    E -> T
//    T -> E + F
//    T -> E - F
//    T -> F
//    F -> T * V
//    F -> T / V
//    F -> V
//    V -> ( E )
//    V -> id
//    V -> const
//    id -> const
    




#include <string>
#include <iostream>
#include <map>
#include <algorithm>

using std::string, std::cout, std::getline, std::cin, std::stoi, std::map;

int statement(string input);
int assignment(string input);
int expression(string input);
int term(string input);
int factor (string input);
int value(string input);
int id(string input);
bool isValidName(string input);

map<string, int> idTable;

int main() {
    string input;
    cout << ">";
    getline(cin, input);
    while (!cin.eof()) {
        input.erase(remove(input.begin(), input.end(), ' '), input.end());
        try {
            int result = statement(input);
            cout << result << "\n>";
        }
        catch (char* e) {
            cout << e << "\n";
        }
        catch (std::invalid_argument &e) {
            cout << e.what() << "\n";
        }
        catch (...) {
            cout << "Error: Invalid input\n";
        }
        getline(cin, input);
    }
}

int statement(string input) {
    string::size_type idx = input.find('=');
    if (idx == string::npos) 
        return expression(input);
    
    return assignment(input);
}

int assignment(string input) {
    string::size_type idx = input.find('=');
    const string substring{ input.substr(0, idx) };
    if (!isValidName(substring)) {
        throw std::invalid_argument("Error, invalid variable name");
    }
    return idTable[substring] = 
        expression(input.substr(idx + 1, input.size() - idx));
}

int expression(string input) {
    if (input[0] == '(' && input[input.size() - 1] == ')') {
        return expression(input.substr(1, input.size() - 2));
    }
    return term(input);
    
}

int term(string input) {
    string::size_type idx = input.size() - 1;
    for(unsigned count = 0; idx < input.size(); --idx) {
        if(input[idx] == '(') {
            --count;
        }
        else if(input[idx] == ')') {
            ++count;
        }
        else if(input[idx] == '+' && count == 0) {
            return expression(input.substr(0, idx)) + factor(input.substr(idx + 1, input.size() - idx - 1));
        }
        else if(input[idx] == '-' && count == 0) {
            return expression(input.substr(0, idx)) - factor(input.substr(idx + 1, input.size() - idx - 1));
        }
    }
    return factor(input);
}

int factor (string input){
    string::size_type idx = input.size() - 1;
    for (unsigned count = 0; idx < input.size(); --idx) {
        //cout << "'" << input[idx] << "', " << count << "\n";
        if (input[idx] == '(')
            --count;
        else if (input[idx] == ')')
            ++count;
        else if (input[idx] == '*' && count == 0) {
            return expression(input.substr(0, idx)) * value(input.substr(idx + 1, input.size() - idx - 1));
        }
        else if (input[idx] == '/' && count == 0) {
            return expression(input.substr(0, idx)) / value(input.substr(idx + 1, input.size() - idx - 1));
        }
    }
    return value(input);
}

int value(string input) {
    if (input[0] == '(' && input[input.size() - 1] == ')') {
        return expression(input.substr(1, input.size() - 2));
    }
    if (idTable.find(input) == idTable.end()) {
        return stoi(input, nullptr, 10);
    }
    return id(input);
}

int id(string input) {
    return idTable[input];
}

bool isValidName(string input) {
    if ((input[0] < 'a' || input[0] > 'z') &&
        (input[0] < 'A' || input[0] > 'Z')) {
        return false;
    }

    for (unsigned i = 1; i < input.length(); ++i) {
        if ((input[i] < 'a' || input[i] > 'z') &&
            (input[i] < 'A' || input[i] > 'Z') &&
            (input[i] < '0' || input[i] > '9') &&
            (input[i] != '_'))
            return false;
    }
    return true;
}