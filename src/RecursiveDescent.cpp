/*
* Authors: John Bouchard, Adam Jeniski

    Given a string consisting of parentheses, 
    single digits, and positive and negative signs, 
    convert the string into a mathematical expression to obtain the answer.

    Don't use eval or a similar built-in parser.

    For example, given '-1 + (2 + 3)', you should return 4.
*/



//Grammar:
//    <Statements>              -> <Statement>
//                               | <Statement><Statements>
//    <Statement>               -> <Expression>;
//                               | <Assignment Statement>;
//    <Assignment Statement>    -> id = <Expression>
//    <Expression>              -> ( <Expression> )
//                               | <Term>
//    <Term>                    -> <Expression> + <Factor>
//                               | <Expression> - <Factor>
//                               | <Factor>
//    <Factor>                  -> <Term> * <Value>
//                               | <Term> / <Value>
//                               | <Value>
//    <Value>                   -> ( <Expression> )
//                               | id
//                               | const
//    id                        -> const
    


// test deploy script to godblot

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

using std::cout, std::string, std::invalid_argument;

void statements(string input);
int statement(string input);
int assignment(string input);
int expression(string input);
int term(string input);
int factor (string input);
int value(string input);
int id(string input);
bool isValidName(string input);

std::map<string, int> idTable;

int main(int argc, char* argv[]) {
    using std::cin, std::getline;

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
    cout << statement(input.substr(0, endOfStatmentIdx + 1)) << "\n";
    statements(input.substr(endOfStatmentIdx + 1));
}

int statement(string input) {
    const auto endOfStatmentIdx = input.find(';');
    if (endOfStatmentIdx == string::npos) {
        throw invalid_argument("Error: invalid statment, statement must end with ';'");
    }
    if (input.find('=') == string::npos) 
        return expression(input.substr(0, endOfStatmentIdx));
    
    return assignment(input.substr(0, endOfStatmentIdx));
}

int assignment(string input) {
    string::size_type idx = input.find('=');
    const string substring{ input.substr(0, idx) };
    if (!isValidName(substring)) {
        throw invalid_argument("Error: invalid variable name");
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
