#include "Password.h";

Password::Password(std::vector<string> data) {
    name = data[0];
    password = data[1];
    category = data[2];
    optional = false;

    if (data.size() == 5) {
        URL = data[3];
        login = data[4];
        optional = true;
    }
}

/**
 *
 * @param name
 * @param password
 * @param category
 */
Password::Password(string name, string password, string category) :
    name(name), password(password), category(category), optional(false) {}

/**
 *
 * @param name
 * @param password
 * @param category
 * @param URL
 * @param login
 */
Password::Password(string name, string password, string category, string URL, string login) :
    name(name), password(password), category(category), URL(URL), login(login), optional(true) {}

Password::~Password() {}

/**
 *
 * @param numberOfCharacters
 * @param UpperLowercase
 * @param specialChars
 * @return
 */
string Password::RandomPassword(int numberOfCharacters, bool UpperLowercase, bool specialChars) {

    string password = "";
    for (size_t i = 0; i < numberOfCharacters; i++)
    {
        if (UpperLowercase && specialChars) {
            password += (char)(rand() % ((int)'z' - (int)'!' + 1) + 33);
        }
        else if (UpperLowercase && !specialChars) {
            int n;
            if (rand() % 2 == 0) {
                n = rand() % ((int)'Z' - (int)'A' + 1) + 65;
            }
            else {
                n = rand() % ((int)'z' - (int)'a' + 1) + 97;
            }

            password += (char)(n);
        }
        else if (!UpperLowercase && specialChars) {
            int n;
            if (rand() % 2 == 0) {
                n = rand() % ((int)'@' - (int)'!' + 1) + 33;
            }
            else {
                n = rand() % ((int)'z' - (int)'a' + 1) + 91;
            }

            password += (char)(n);
        }
        else if (!UpperLowercase && !specialChars)
        {
            password += (char)(rand() % ((int)'z' - (int)'a' + 1) + (int)'a');
        }
    }

    std::cout << "Your generated password: " << password << endl;
    return password;
}

string Password::toString() {
    if (optional)
        return name + ';' + password + ';' + category + ';' + URL + ';' + login;
    else
        return name + ';' + password + ';' + category;
}

/**
 *
 * @param str
 * @param p
 * @return
 */
std::ostream& operator<<(std::ostream& str, const Password& p) {
    return str << "Password info:\n"
        << "Name: " + p.getName() << endl
        << "Password: " + p.getPassword() << endl
        << "Category: " + p.getCategory() << endl
        << "URL: " + p.getURL() << endl
        << "Login: " + p.getLogin() << endl;
}


string Password::getName() const { return name; }
/**
 *
 * @param name
 */
void Password::setName(string name) { this->name = name; }

string Password::getPassword() const { return password; }

/**
 *
 * @param password
 */
void Password::setPassword(string password) { this->password = password; }

string Password::getCategory() const { return category; }

/**
 *
 * @param category
 */
void Password::setCategory(string category) { this->category = category; }

string Password::getURL() const { return URL; }

/**
 *
 * @param url
 */
void Password::setURL(string url) { this->URL = url; }

string Password::getLogin() const { return login; }

/**
 *
 * @param login
 */
void Password::setLogin(string login) { this->login = login; }

