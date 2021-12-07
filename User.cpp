#define _CRT_SECURE_NO_WARNINGS
#include <sstream>
#include <fstream>
#include "User.h"

const bool User::checkPassword(const string &password) const {
  return (hsh(password) == this->password);
}

void User::setPassword(const string &password) {
  this->password = hsh(password);
}

User User::parse(const string &line) {
  istringstream istr(line);
  string fields[6];
  string tmp;
  int i = 0;
  while (getline(istr, tmp, ';')) {
    fields[i++] = tmp;
  }
  User user;
  user.username = fields[0];
  sscanf(fields[1].c_str(), "%zu", &user.password);
  user.firstName = fields[2];
  user.lastName = fields[3];
  user.savedCreditCard = fields[4];
  user.accountBalance = atof(fields[5].c_str());
  return user;
}

string User::to_str() {
  return username + ";"
         + to_string(password) + ";"
         + firstName + ";"
         + lastName + ";"
         + savedCreditCard + ";"
         + to_string(accountBalance) + ";";
}

int UserController::hashCode(const string& key) {
  int i, result = 0;
  if (key.length() < 4) {
    for (i = 0; i < (int)key.length(); i++)
      result += key.at(i);
  } else {
    for (i = 0; i < (int)key.length() - 4; i++) {}
    result += key.at(i);
    for (int j = 0; j < 4; ++j)
      result += (int)(key.at(i+j)) * pow(10, j);
  }
  return result;
}

bool UserController::checkUsername(string username) {
  return !(username.size() < 4 || users.containsKey(username));
}

bool UserController::checkPassword(string pass) {
  // TODO: More password checks
  return !(pass.size() < 8);
}

void UserController::addUser(string username, string pass, string fn, string ln) {
  users[username].username = username;
  users[username].setPassword(pass);
  users[username].firstName = fn;
  users[username].lastName = ln;
  users[username].accountBalance = 0;
}

bool UserController::login(string user, string pass) {
  if (users[user].checkPassword(pass)) {
    currentUser = &users[user];
    return true;
  }
  return false;
}

void UserController::loadUsers(string filename) {
  ifstream File(filename);
  while (getline(File, buf)) {
    User user = User::parse(buf);
    users[user.username] = user;
  }
}

void UserController::saveToFile(string filename) {
  ofstream File(filename);
  Queue<string> usernames = users.keys();
  while (!usernames.empty()) {
    File << users[usernames.front()].to_str() << "\n";
    usernames.pop();
  }
  File.close();
}

void UserController::logout() {
  currentUser = nullptr;
}

string UserController::getCurrentUserFullName() const {
  return currentUser->firstName + " " + currentUser->lastName;
}

string UserController::getCurrentUsername() const {
  return currentUser->username;
}

bool UserController::purchase(double price) {
  if (currentUser->accountBalance < price)
    return false;
  currentUser->accountBalance -= price;
  return true;
}

bool UserController::checkCard() {
  return (currentUser->savedCreditCard.size() == 16);
}

void UserController::addCreditCard(string cardNum) {
  currentUser->savedCreditCard = cardNum;
}

void UserController::addBalance(double amount) {
  currentUser->accountBalance += amount;
}

double UserController::getBalance() const {
  return currentUser->accountBalance;
}