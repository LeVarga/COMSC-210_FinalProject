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
        // TODO: Implement the ticket purchase submenu
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
          cout << "Events at " << buf << ": \n";
          for (const auto &event : eventController.getEventsByLocation(buf)) {
            // TODO: Format output properly
            cout << event.sport << ":\t" << event.teams.first << " v. "
                 << event.teams.second << "\t" << event.date << "\n";
          }
        } else if (buf == "3") {
          cout << "Sports available:\n";
          for (const auto &sport : eventController.getSports()) {
            cout << sport << "\n";
          }
          cout << "Your choice --> ";
          cin.ignore();
          getline(cin, buf);
          cout << buf << " games:\n";
          for (const auto &event : eventController.getEventsBySport(buf)) {
            // TODO: Format output properly
            cout << event.teams.first << " v. "
                 << event.teams.second << "\t" << event.date << "\t"
                 << event.location << "\n";
          }
        } else if (buf == "4") {
          cout << "Teams found:\n";
          for (const auto &team : eventController.getAllTeams()) {
            cout << team << "\n";
          }
          cout << "Enter the team you're looking for --> ";
          cin.ignore();
          getline(cin, buf);
          cout << "Games with " << buf << ":\n";
          for (const auto &event : eventController.getEventsByTeam(buf)) {
            // TODO: Format output properly
            cout << event.teams.first << " v. "
                 << event.teams.second << "\t" << event.date << "\t"
                 << event.location << " \t(" << event.sport << ")\n";
          }
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
      loginPrompt();
    } else if (buf == "5") {
      userController.saveToFile("users.txt");
      exit(0);
    } else {
      cout << "Invalid option, try again.";
    }
  }


  return 0;
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