/*
* Authors: John Bouchard, Adam Jeniski

    Given a string consisting of parentheses, 
    single digits, and positive and negative signs, 
    convert the string into a mathematical expression to obtain the answer.

    Don't use eval or a similar built-in parser.

    For example, given '-1 + (2 + 3);', you should return 4.

Grammar:
    <Statements>            -> <Statement>
                             | <Statement><Statements>
    <Statement>               -> <Expression>;
                              | <Assignment Statement>;
    <Assignment Statement>   -> id = <Expression>
    <Expression>             -> ( <Expression> )
                              | <Term>
    <Term>                   -> <Expression> + <Factor>
                              | <Expression> - <Factor>
                              | <Factor>
    <Factor>                 -> <Term> * <Value>
                              | <Term> / <Value>
                              | <Value>
    <Value>                  -> ( <Expression> )
                              | id
                              | const
    id                       -> const
    
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

#include "Value.h"
#include "Type.h"

using std::cout, std::string, std::invalid_argument;

void statements(string input);
Value statement(string input);
Value assignment(string input);
Value expression(string input);
Value term(string input);
Value factor (string input);
Value value(string input);
Value id(string input);
bool isValidName(string input);

std::map<string, Value> idTable;

int main(int argc, char* argv[]) {
    using std::cin, std::getline;

    free(nullptr);

    if(argc == 2) {
        // Get input
        const char* FILE_PATH = argv[1];
        std::ifstream srcFile;
        srcFile.open(FILE_PATH);
        string input, line;
        if(srcFile.is_open()) {
            while(getline(srcFile, line)){
                input += line;
            }
        } else {
            cout << "Unable to read file";
        }

        // Remove spaces from input
        input.erase(remove(input.begin(), input.end(), ' '), input.end());

        // Parse input
        try {
                statements(input);
            }
            catch (char* e) {
                cout << e;
            }
            catch (invalid_argument &e) {
                cout << e.what();
            }
            catch (...) {
                cout << "Error: Invalid input\n";
            }
    }
    else {
        string input;
        cout << ">";
        getline(cin, input);
        while (!cin.eof()) {
            input.erase(remove(input.begin(), input.end(), ' '), input.end());
            try {
                statements(input);
                cout << '>';
            }
            catch (char* e) {
                cout << e << "\n>";
            }
            catch (invalid_argument &e) {
                cout << e.what() << "\n>";
            }
            catch (...) {
                cout << "Error: Invalid input\n>";
            }
            getline(cin, input);
        }
    }
}

void statements(string input) {
    if(input.empty())
        return;
    const auto endOfStatmentIdx = input.find(';');
    if (endOfStatmentIdx == string::npos) {
        throw invalid_argument("Error: invalid statments, each statement must end with ';'");
    }
    cout << statement(input.substr(0, endOfStatmentIdx + 1)).toString() << '\n';
    statements(input.substr(endOfStatmentIdx + 1));
}

Value statement(string input) {
    const auto endOfStatmentIdx = input.find(';');
    if (endOfStatmentIdx == string::npos) {
        throw invalid_argument("Error: invalid statment, statement must end with ';'");
    }
    if (input.find('=') == string::npos) 
        return expression(input.substr(0, endOfStatmentIdx));
    
    return assignment(input.substr(0, endOfStatmentIdx));
}

Value assignment(string input) {
    string::size_type idx = input.find('=');
    const string substring{ input.substr(0, idx) };
    if (!isValidName(substring)) {
        throw invalid_argument("Error: invalid variable name");
    }
    return idTable[substring] = 
        expression(input.substr(idx + 1, input.size() - idx));
}

Value expression(string input) {
    if (input[0] == '(' && input[input.size() - 1] == ')') {
        return expression(input.substr(1, input.size() - 2));
    }
    return term(input);
}

Value term(string input) {
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
            break;
        }
        else if(input[idx] == '-' && count == 0) {
            return expression(input.substr(0, idx)) - factor(input.substr(idx + 1, input.size() - idx - 1));
            break;
        }
    }
    return factor(input);
}

Value factor (string input){
    string::size_type idx = input.size() - 1;
    for (unsigned count = 0; idx < input.size(); --idx) {
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

Value value(string input) {
    if (input[0] == '(' && input[input.size() - 1] == ')') {
        return expression(input.substr(1, input.size() - 2));
    }
    if (idTable.find(input) == idTable.end()) {
        return stoi(input, nullptr, 10);
    }
    return id(input);
}

Value id(string input) {
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
