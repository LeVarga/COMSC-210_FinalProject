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
void addBalance();
void seatSelectPrompt(Event*);
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
          Event* ev = eventPicker(eventController.getEventsByLocation(buf));
          seatSelectPrompt(ev);
        } else if (buf == "3") {
          cout << "Sports available:\n";
          for (const auto &sport : eventController.getSports()) {
            cout << sport << "\n";
          }
          cout << "Your choice --> ";
          cin.ignore();
          getline(cin, buf);
          cout << buf << " games:\n";
          Event* ev = eventPicker(eventController.getEventsBySport(buf));
          seatSelectPrompt(ev);
        } else if (buf == "4") {
          cout << "Teams found:\n";
          for (const auto &team : eventController.getAllTeams()) {
            cout << team << "\n";
          }
          cout << "Enter the team you're looking for --> ";
          cin.ignore();
          getline(cin, buf);
          cout << "Games with " << buf << ":\n";
          Event* ev = eventPicker(eventController.getEventsByTeam(buf));
          seatSelectPrompt(ev);
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

void seatSelectPrompt(Event* event) {
  cout << "Choose your seat (A1-F99) or X to go back: ";
  cin >> buf;
  if (buf == "x" || buf == "X") return;
  string seat = buf;
  double cost = event->checkSeat(seat);
  if (cost >= 0) {
    cout << buf << " is available and costs $" << cost << ". Would you like to reserve it? (Y/N) --> ";
    cin >> buf;
    if (buf == "y" || buf == "Y") {
      Ticket* tick = eventController.createTicket(event, seat);
      while(userController.purchaseTicket(tick, cost) == false)
      {
        cout << "You seem to have an insufficient balance in your account. Would you like to add more? (Y/N) --> ";
        cin >> buf;
        if (buf == "y" || buf == "Y")
          addBalance();
        else
          seatSelectPrompt(event);
      }
      cout << "Ticket successfully purchased. Make sure to show the confirmation code at the door.\n";
      return;
    }
    else {
      seatSelectPrompt(event);
    }
  }
  else {
    cout << buf << " is unavailable. Try a different one.\n";
    seatSelectPrompt(event);
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
    cout << "Invalid choice, try again.\n";
    return eventPicker(events);
  }
}

void addBalance()
{
  cout << "Do you have a card registered? (Y/N) --> ";
  cin >> buf;
  if (buf == "n" || buf == "N")
  {
    cout << "Would you like to register one? (Y/N) --> ";
    cin >> buf;
    if (buf == "y" || buf == "Y")
    {
      cout << "Please enter the 16-digit number of your card: ";
      cin >> buf;
      userController.addCreditCard(buf);
      while (!userController.checkCard())
      {
        cout << "Card number not 16-digits, try again: ";
        cin >> buf;
        userController.addCreditCard(buf);
      }

      cout << "Credit card successfully added to account.\n";
    }
    else if (buf == "n" || buf == "N")
      return; 
  }
  if (!userController.checkCard())
  {
    cout << "No valid credit card on file, try again.\n";
    addBalance();
  }
  cout << "How much would you like to add to your balance? Enter: ";
  cin >> buf;
  userController.addBalance(atoi(buf.c_str()));
  cout << "$" << atoi(buf.c_str()) << " successfully added to your account.\nCurrent total: $" << userController.getBalance() << "\n\n";
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
  cin >> password;
  while (!userController.checkPassword(password)) {
    cout << "Password too short, try again: ";
    cin >> password;
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