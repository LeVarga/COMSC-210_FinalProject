#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include "HashTable.h"
#include "Queue.h"
#include "DynamicArray.h"
#include <cstdlib>
#include <stdio.h>

using namespace std;

string buf;

struct Event {
    string name;
    string date;
    string location;
    pair<string, string> teams;

    static Event parse(const string &line) {
      istringstream istr(line);
      string fields[5];
      string tmp;
      int i = 0;
      while (getline(istr, tmp, ';')) {
        fields[i++] = tmp;
      }
      Event event;
      event.name = fields[0];
      event.date = fields[1];
      event.location = fields[2];
      event.teams.first = fields[3];
      event.teams.second = fields[4];
      return event;
    }
};

DynamicArray<Event> loadEvents(const string filename, int& numEvents) {
  string buf;
  ifstream File(filename);
  DynamicArray<Event> events;
  while (getline(File, buf)) {
    events[numEvents] = Event().parse(buf);
    numEvents++;
  }
  return events;
}

struct Ticket {
    string confirmation;
    string seat;
    Event event;
};

struct User {
private:
  hash<string> hash;
  size_t password;
public:
  string username;
  string firstName;
  string lastName;
  string savedCreditCard;
  DynamicArray<Ticket*> purchasedTickets;

  const bool checkPassword(const string &password) const {
    return (hash(password) == this->password);
  }

  void setPassword(const string &password) {
    this->password = hash(password);
  }

  static User parse(const string &line) {
    istringstream istr(line);
    string fields[5];
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
    // TODO: add ticket data
    return user;
  }

  static string to_str(const User& user) {
    return user.username + ";"
         + to_string(user.password) + ";"
         + user.firstName + ";"
         + user.lastName + ";"
         + user.savedCreditCard + ";"; // TODO: add ticket data
  }
};

int hashCode(const string& key) {
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

class UserController {
    HashTable<string, User, 100> users = HashTable<string, User, 100>(hashCode);
    User* currentUser;

    void loginPrompt() {
      string username;
      cout << "Enter your username or X to go back: ";
      cin >> username;
      if (username == "X" || username == "x") return;
      cout << "Enter your password: ";
      cin >> buf;
      if (users[username].checkPassword(buf)) {
        cout << "Login successful!\n";
        currentUser = &users[username];
        return;
      }
      cout << "Incorrect username or password, try again.\n";
      loginPrompt();
    }
    void signupPrompt() {
      string username;
      cout << "Username (4+ characters): ";
      cin >> username;
      while (username.size() < 4 || users.containsKey(username)) {
        cout << "Username is too short or already taken, try again: ";
        cin >> username;
      }
      cout << "Password (8+ characters): ";
      cin >> buf;
      while (buf.size() < 8) {
        cout << "Password too short, try again: ";
        cin >> buf;
      }
      users[username].username = username;
      users[username].setPassword(buf);
      cout << "First name: ";
      cin >> users[username].firstName;
      cout << "Last name: ";
      cin >> users[username].lastName;
    }

public:
    void loadUsers(string filename) {
      ifstream File(filename);
      while (getline(File, buf)) {
        User user = User().parse(buf);
        users[user.username] = user;
      }
    }
    void loginOrSignupPrompt() {
      while (!currentUser) {
        cout << "Enter [1] to log in\n"
             << "      [2] to sign up\n"
             << "      [3] to quit\n"
             << "Choice --> ";
        cin >> buf;
        if (buf == "1") {
          loginPrompt();
        } else if (buf == "2") {
          signupPrompt();
        } else if (buf == "3"){
          saveToFile("users.txt");
          exit(0);
        } else {
          cout << "Invalid option, try again.\n";
        }
      }
    }
    void saveToFile(string filename) {
      ofstream File(filename);
      Queue<string> usernames = users.keys();
      while (!usernames.empty()) {
        File << User().to_str(users[usernames.front()]) << "\n";
        usernames.pop();
      }
      File.close();
    }
    void logout() {
      currentUser = nullptr;
    }
    string getCurrentUserFullName() {
      return currentUser->firstName + " " + currentUser->lastName;
    }
};

UserController userController;

int main() {
  int numEvents = 0;
  // load data from files
  userController.loadUsers("users.txt");
  auto events = loadEvents("events.txt", numEvents);
  // initial login prompt
  userController.loginOrSignupPrompt();

  // main menu
  while (true) {
    cout << "Logged in as " << userController.getCurrentUserFullName() << "\n"
         << "Enter [1] to find events\n"
         << "      [2] to purchase merchandise\n"
         << "      [3] to view your reservations\n"
         << "      [4] to log out\n"
         << "      [5] to quit\n"
         << "Choice --> ";
    cin >> buf;
    if (buf == "1") {
      while (true) {
        cout << "Enter [1] to view events by date"
             << "      [2] to view events by location\n"
             << "      [3] to view events by sport\n"
             << "      [4] to view events by teams\n"
             << "      [5] to go back...\n"
             << "Choice --> \n";
        cin >> buf;
        // TODO: Implement the lists and ticket purchase submenu
        if (buf == "1") {
          cout << "NOT IMPLEMENTED\n";
        } else if (buf == "2") {
          cout << "NOT IMPLEMENTED\n";
        } else if (buf == "3") {
          cout << "NOT IMPLEMENTED\n";
        } else if (buf == "4") {
          cout << "NOT IMPLEMENTED\n";
        } else if (buf == "5") {
          break;
        } else {
          cout << "Invalid option, try again.";
        }
      }
    } else if (buf == "2") {
      cout << "NOT IMPLEMENTED\n"; // TODO: Implement merch purchase menu
    } else if (buf == "3") {
      cout << "NOT IMPLEMENTED\n"; // TODO: Display user's tickets here
    } else if (buf == "4") {
      userController.logout();
      userController.loginOrSignupPrompt();
    } else if (buf == "5") {
      userController.saveToFile("users.txt");
      exit(0);
    } else {
      cout << "Invalid option, try again.";
    }
  }

  return 0;
}