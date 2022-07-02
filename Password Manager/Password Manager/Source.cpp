#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include "Password.h"

using std::string;
using std::cout; using std::cin; using std::endl;
namespace fs = std::filesystem;

void pressEnterContinue() {
    string tmp;
    cout << "Press Enter to Continue";
    cin.ignore();
    getline(cin, tmp);
}

/**
 * function which enumerates and list all objects
 * @tparam T
 * @param ls
 */
template <class T>
void list(const std::vector<T>& ls) {
    for (size_t i = 0; i < ls.size(); i++)
    {
        cout << i << ". " << ls[i] << endl;
    }
}

/**
 * Function which splits a str with a delimiter into a vector
 * @param str - string to be spli
 * @param del - delimiter
 * @return
 */
std::vector<string>& split(const string& str, const char& del)
{
    std::vector<string>* output = new std::vector<string>;
    string temp = "";
    for (int i = 0; i < str.size(); i++) {
        if (str[i] != del) {
            temp += str[i];
        }
        else {
            output->push_back(temp);
            temp = "";
        }
    }

    output->push_back(temp);
    return *output;
}

/**
 * Function which returns a vector of hours, minutes, seconds
 * @return
 */
std::vector<string>& getTime() {
    time_t czas;
    time(&czas);
    string t = ctime(&czas);
    std::vector<string>* a = new std::vector<string>;
    *a = split(t.substr(11, 8), ':');

    return *a;
}

/**
 * Function which calculates a secretkey
 * @param mainPassword - main password of the file
 * @return
 */
int secretKey(const string& mainPassword) {
    int sum = 0;
    for (char c : mainPassword) {
        sum += (int)c;
    }
    return sum % 26;
}

/**
 *
 * @param filePath - path of the file to be encrypted
 * @param passwords - vector of passwords to be encrypted
 * @param timestamp - hours, minutes, seconds
 */
void encrypt(const string filePath, std::vector<Password>& passwords, const std::vector<string>& timestamp)
{
    std::ofstream f;
    f.open(filePath);

    if (!f)
    {
        cout << "File did not open!";
        return;
    }

    string mainPassword;
    cout << "key: ";
    cin >> mainPassword;

    int secretkey = secretKey(mainPassword);

    int line = 1;
    for (int i = 0; i < passwords.size();) {
        if (line == 1) {
            f << timestamp[0];
        }
        else if (line == 2) {
            f << timestamp[1];
        }
        else if (line == 3) {
            f << timestamp[2];
        }
        else {
            for (char c : passwords[i].toString()) {
                if (c == ';') {
                    f << c;
                }
                else {
                    int temp = (c + secretkey);
                    f << (char)temp;
                }
            }
            f << char(secretKey);
            i++;
        }
        f << "\n";
        line++;
    }

    f.close();
}

/**
 *
 * @param filePath - path of the file to be encrypted
 * @param categories - vector of categories
 * * @rerutn passwords - vector of decrypted passwords
*/
std::vector<Password>& decrypt(const string& filePath, std::vector<string>& categories)
{
    std::ifstream f;
    f.open(filePath);

    if (!f)
    {
        cout << "File did not open!";
        exit(1);
    }

    string mainPassword;
    cout << "file password: ";
    cin >> mainPassword;

    int secretkey = secretKey(mainPassword);

    int counter = 1;
    string line;
    std::vector<Password>* passwords = new std::vector<Password>;
    while (getline(f, line)) {
        cout << line << endl;
        if (counter > 3) {
            string tmp;
            for (size_t i = 0; i < line.size() - 1; i++) {
                if (line[i] == ';') {
                    tmp += line[i];
                }
                else {
                    tmp += (char)(line[i] - secretkey);
                }
            }

            std::vector<string> v = split(tmp, ';');

            if (!std::count(categories.begin(), categories.end(), v[2])) {
                categories.push_back(v[2]);
            }

            Password password = Password(v);
            passwords->push_back(password);
        }

        counter++;
    }

    f.close();

    return *passwords;
}

/**
 *
 * @return current executed directory
 */
std::string GetCurrentDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    return std::string(buffer).substr(0, pos);
}

/**
 *
 * @param passwords - vector of passwords
 * @return - candidates of passwords matching criteria
 */
std::vector<Password>& searchPassword(const std::vector<Password>& passwords) {

    string criteria;
    cout << "Criteria: ";
    cin >> criteria;

    std::vector<Password>* candidates = new std::vector<Password>;
    for (Password password : passwords) {
        if (password.getName().find(criteria) != std::string::npos || password.getURL().find(criteria) != std::string::npos || password.getLogin().find(criteria) != std::string::npos) {
            candidates->push_back(password);
        }
    }


    return *candidates;
}

/**
 *
 * @param passwords - vector of passwords
 * @param listOfParameters - vector of string parameters, by which passwords are supposed to be sorted
 */
void sortPasswords(std::vector<Password>& passwords, const std::vector<string>& listOfParameters) {

    sort(passwords.begin(), passwords.end(),
        [listOfParameters](const Password& lhs, const Password& rhs) {
            for (string parameter : listOfParameters) {
                if (parameter == "Name" && lhs.getName() != rhs.getName()) {
                    return lhs.getName() < rhs.getName();
                }
                else if (parameter == "Category" && lhs.getCategory() != rhs.getCategory()) {
                    return lhs.getCategory() < rhs.getCategory();
                }
                else if (parameter == "URL" && lhs.getURL() != rhs.getURL()) {
                    return lhs.getURL() < rhs.getURL();
                }
                else if (parameter == "Login" && lhs.getLogin() != rhs.getLogin()) {
                    return lhs.getLogin() < rhs.getLogin();
                }
            }

            return false;
        }
    );

    system("CLS");
    cout << "Sorted: \n";
    for (Password p : passwords) {
        cout << p << endl;
    }

    pressEnterContinue();
}

/**
 *
 * @param passwords - vector of passwords
 * @param categories - vector of categories
 */
void addPassword(std::vector<Password>& passwords, std::vector<string>& categories)
{
    Password p;
    string name;
    string password;
    int category;
    string URL = "";
    string login = "";
    cout << "Name: ";
    cin.ignore();
    getline(cin, name);

    int n;
    cout << "Passoword: \n";
    cout << "Would you like to generate a password?\n";
    cout << "1) yes\n";
    cout << "2) no\n";
    cout << "input: ";
    cin >> n;

    if (n == 1) {
        cout << "Number of characters: ";
        cin >> n;
        int a, b;
        cout << "Uppercase and lowercase characters: \n";
        cout << "1) yes\n";
        cout << "2) no\n";
        cout << "input: ";
        cin >> a;
        cout << "Special characters: \n";
        cout << "1) yes\n";
        cout << "2) no\n";
        cout << "input: ";
        cin >> b;
        password = p.RandomPassword(n, a % 2, b % 2);
    }
    else {
        cout << "Enter your password: ";
        cin >> password;
    }

    cout << "Category: \n";
    for (int i = 0; i < categories.size(); i++) {
        cout << i << ". " << categories[i] << endl;
    }
    cout << "input: ";
    cin >> category;
    cout << "URL(optional): ";
    cin.ignore();
    getline(cin, URL);
    cout << "login(optional): ";
    cin.ignore();
    getline(cin, login);

    p = Password(name, password, categories[category], URL, login);

    passwords.push_back(p);
    cout << "Your password has been succesfully created!\n";
}
/**
 *
 * @param passwords - password to be edited
 */
void editPassword(std::vector<Password>& passwords, const std::vector<string>& categories) {

    if (passwords.empty()) {
        cout << "There are no passwords to be edited.";
        return;
    }

    int n;
    cout << "Pick a passwords to be edited:\n";
    for (int i = 0; i < passwords.size(); i++) {
        cout << i << ". " << passwords[i] << endl;
    }
    cout << "input: ";
    cin >> n;

    Password* password = &passwords[n];

    int input;
    cout << "What would you like to change?\n";
    cout << *password;
    cout << "Edit:\n";
    cout << "1) Name\n";
    cout << "2) Password\n";
    cout << "3) Category\n";
    cout << "4) URL\n";
    cout << "5) Login\n";
    cout << "input: ";
    cin >> input;

    cin.ignore();
    string tmp;
    switch (input) {
    case 1:
    {
        cout << "New name: ";
        getline(cin, tmp);
        password->setName(tmp);
        break;
    }
    case 2:
    {
        cout << "New password: ";
        getline(cin, tmp);
        password->setPassword(tmp);
        break;
    }
    case 3:
    {
        cout << "Category:\n";
        list(categories);
        int i;
        cout << "input: ";
        cin >> i;
        password->setCategory(categories[i]);
        break;
    }
    case 4:
    {
        cout << "New URL: ";
        getline(cin, tmp);
        password->setURL(tmp);
        break;
    }
    case 5:
    {
        cout << "New login: ";
        getline(cin, tmp);
        password->setLogin(tmp);
        break;
    }
    }
}

/**
 *
 * @param passwords - vector of passwords
 */
void removePassword(std::vector<Password>& passwords) {

    if (passwords.empty()) {
        cout << "There are no passwords to be removed.";
        return;
    }

    int n;
    list(passwords);
    cout << "Pick a password, which you'd like to remove.\n";
    cout << "input: ";
    cin >> n;

    passwords.erase(passwords.begin() + n);
}

/**
 *
 * @param categories - vector of categories
 */
void addCategory(std::vector<string>& categories) {
    string tmp;
    cout << "What category would you like to add: ";
    cin >> tmp;
    categories.push_back(tmp);
    cout << "\nYour category has been succesfully added!\n";
}

/**
*
* @param categories - vector of categories
*/
void removeCategory(std::vector<string>& categories) {
    if (categories.empty()) {
        cout << "There are no categories to be removed.";
        return;
    }

    int n;
    list(categories);
    cout << "Pick a category, which you'd like to remove.\n";
    cout << "input: ";
    cin >> n;

    categories.erase(categories.begin() + n);
}


int main()
{
    int input = 1;
    while (input != 8) {
        cout << "Password Manager";
        cout << "Choose a source file:\n";
        cout << "1) From the list.\n";
        cout << "2) By inputting an absolute path.\n";
        cout << "input: ";
        cin >> input;

        system("CLS");

        string filePath;
        if (input == 1) {
            cout << "Current directory: \n";
            string path = GetCurrentDirectory();

            std::vector<string> paths;
            int i = 1;
            for (const auto& entry : fs::directory_iterator(path)) {
                string tmp = entry.path().string();
                if (tmp.substr(tmp.length() - 3, tmp.length() - 3) == "txt") {
                    std::cout << i++ << ") " << entry.path() << std::endl;
                    paths.push_back(entry.path().string());
                }
            }

            if (paths.empty()) {
                cout << "There are no .txt files in the directory.\n";
                cout << "Enter the absolute path: ";
                cin >> filePath;
            }


            cout << "input: ";
            cin >> input;

            filePath = paths.at(input - 1);
            cout << filePath << endl;
        }
        else {
            cout << "Enter the absolute path: ";
            cin >> filePath;
        }

        std::vector<string> categories;
        std::vector<Password> passwords = decrypt(filePath, categories);
        std::vector<string> timestamp = getTime();

        system("CLS");

        bool exit = true;
        while (exit) {
            system("CLS");
            cout << "timeStamp: " << timestamp[0] << ":" << timestamp[1] << ":" << timestamp[2] << endl;
            if (!passwords.empty()) {
                cout << "Passwords: " << endl;
                list(passwords);
            }

            cout << "Functionalities: \n";
            cout << "1. Search for passwords.\n";
            cout << "2. Sort passwords.\n";
            cout << "3. Add a password.\n";
            cout << "4. Edit a password.\n";
            cout << "5. Remove a password.\n";
            cout << "6. Add a category.\n";
            cout << "7. Removes a category.\n";
            cout << "8. Save and exit.\n";
            cout << "9. Exit.\n";

            cout << "input:";
            cin >> input;

            system("CLS");
            switch (input) {
            case 1:
            {
                std::vector<Password> candidates = searchPassword(passwords);
                if (candidates.empty())
                    cout << "There are no passwords matching your criteria." << endl;
                else
                    list(candidates);

                break;
            }
            case 2:
            {
                std::vector<string> parameters;

                int n;
                string param;
                cout << "Number of parameters: \n";
                cin >> n;
                cout << "For which parameters would you like to sort your passwords?\n";
                cout << "Name;Category;URL;Login" << endl;
                for (size_t i = 0; i < n; i++)
                {
                    cin >> param;
                    parameters.push_back(param);
                }

                sortPasswords(passwords, parameters);
                break;
            }
            case 3:
            {
                addPassword(passwords, categories);
                break;
            }
            case 4:
            {
                editPassword(passwords, categories);
                break;
            }
            case 5:
            {
                removePassword(passwords);
                break;
            }
            case 6:
            {
                addCategory(categories);
                break;
            }
            case 7:
            {
                removeCategory(categories);
                break;
            }
            case 9: {
                exit = false;
                break;
            }
            default:
            {
                encrypt(filePath, passwords, timestamp);
                exit = false;
                break;
            }
            }
            pressEnterContinue();
        }

        system("CLS");

        cout << "Would you like to continue with a diffrent source file?\n";
        cout << "1) yes\n";
        cout << "2) no\n";
        cout << "input: ";
        cin >> input;

        system("CLS");
        if (input == 2)
            input = 8;
    }
}