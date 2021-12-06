#include <sstream>
#include <iostream>
#include <cstdlib>
#include "Event.h"
#include "User.h"

using namespace std;

string buf;

UserController userController;
EventController eventController;

void loginPrompt();
void loginOrSignupPrompt();
void signupPrompt();
Event* eventPicker(vector<Event> events);

int main() {
  // load data from files
  userController.loadUsers("users.txt");
  eventController.loadEvents("events.txt");

  // initial login prompt
  loginOrSignupPrompt();

  // main menu
  while (true) {
    cout << "Logged in as " << userController.getCurrentUserFullName() << "\n"
         << "Enter [1] to find events\n"
         << "      [2] to purchase merchandise [NOT IMPLEMENTED]\n"
         << "      [3] to view your reservations [NOT IMPLEMENTED]\n"
         << "      [4] to log out\n"
         << "      [5] to quit\n"
         << "Choice --> ";
    cin >> buf;
    if (buf == "1") {
      while (true) {
        cout << "Enter [1] to view events by date [NOT IMPLEMENTED]\n"
             << "      [2] to view events by location\n"
             << "      [3] to view events by sport\n"
             << "      [4] to view events by team\n"
             << "      [5] to go back...\n"
             << "Choice -->";
        cin >> buf;
        if (buf == "1") {
          cout << "NOT IMPLEMENTED\n";
        } else if (buf == "2") {
          cout << "Locations available: \n";
          for (const auto &location : eventController.getLocations()) {
            cout << location << "\n";
          }
          cout << "Your choice --> ";
          cin.ignore();
          getline(cin, buf);
          cout << "Events in " << buf << ":\n";
          eventPicker(eventController.getEventsByLocation(buf));
        } else if (buf == "3") {
          cout << "Sports available:\n";
          for (const auto &sport : eventController.getSports()) {
            cout << sport << "\n";
          }
          cout << "Your choice --> ";
          cin.ignore();
          getline(cin, buf);
          cout << buf << " games:\n";
          eventPicker(eventController.getEventsBySport(buf));
        } else if (buf == "4") {
          cout << "Teams found:\n";
          for (const auto &team : eventController.getAllTeams()) {
            cout << team << "\n";
          }
          cout << "Enter the team you're looking for --> ";
          cin.ignore();
          getline(cin, buf);
          cout << "Games with " << buf << ":\n";
          eventPicker(eventController.getEventsByTeam(buf));
        } else if (buf == "5") {
          break;
        } else {
          cout << "Invalid option, try again.\n";
        }
      }
    } else if (buf == "2") {
      cout << "NOT IMPLEMENTED\n"; // TODO: Implement merch purchase menu
    } else if (buf == "3") {
      cout << "NOT IMPLEMENTED\n"; // TODO: Display user's tickets here
    } else if (buf == "4") {
      userController.logout();
      loginPrompt();
    } else if (buf == "5") {
      userController.saveToFile("users.txt");
      exit(0);
    } else {
      cout << "Invalid option, try again.\n";
    }
  }
}

Event* eventPicker(vector<Event> events) {
  for (int i = 0; i < events.size(); ++i) {
    // TODO: Format output better
    cout << "[" << i + 1 << "] " << events[i].sport << ":\t"
         << events[i].teams.first << " v. " << events[i].teams.second << "\t"
         << events[i].location << "\t" << events[i].date << "\n";
  }
  cout << "Enter the number of an event to buy a ticket, or X to go back: ";
  cin >> buf;
  if (buf == "x" || buf == "X") return nullptr;
  if (atoi(buf.c_str()) <= events.size() && atoi(buf.c_str()) > 0) {
    return &events[atoi(buf.c_str()) - 1];
  } else {
    cout << "Invalid choice, try again.";
    return eventPicker(events);
  }
}

void signupPrompt() {
  string username, password, fn, ln;
  cout << "Username (4+ characters): ";
  cin >> username;
  while (!userController.checkUsername(username)) {
    cout << "Username is too short or already taken, try again: ";
    cin >> username;
  }
  cout << "Password (8+ characters): ";
  cin >> buf;
  while (!userController.checkPassword(buf)) {
    cout << "Password too short, try again: ";
    cin >> buf;
  }
  cout << "First name: ";
  cin >> fn;
  cout << "Last name: ";
  cin >> ln;
  userController.addUser(username, password, fn, ln);
  userController.login(username, password);
}

void loginPrompt() {
  string username;
  cout << "Enter your username or X to go back: ";
  cin >> username;
  if (username == "X" || username == "x") loginOrSignupPrompt();
  else {
    cout << "Enter your password: ";
    cin >> buf;
    if (userController.login(username, buf)) {
      cout << "Login successful!\n";
    } else {
      cout << "Incorrect username or password, try again.\n";
      loginPrompt();
    }
  }
}

void loginOrSignupPrompt() {
  cout << "Enter [1] to log in\n"
       << "      [2] to sign up\n"
       << "      [3] to quit\n"
       << "Choice --> ";
  cin >> buf;
  if (buf == "1") {
    loginPrompt();
  } else if (buf == "2") {
    signupPrompt();
  } else if (buf == "3") {
    userController.saveToFile("users.txt");
    exit(0);
  } else {
    cout << "Invalid option, try again.\n";
    loginOrSignupPrompt();
  }
}