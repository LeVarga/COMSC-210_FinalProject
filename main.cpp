#include <sstream>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "Event.h"
#include "User.h"
#include "Ticket.h"
#include "Merch.h"
#include "Concession.h"

using namespace std;

string buf;

UserController userController;
EventController eventController;
TicketController ticketController;
MerchController merchController;
ConcessionsController concessionsController;

void loginPrompt();
void loginOrSignupPrompt();
void signupPrompt();
void addBalance();
void seatSelectPrompt(Event*);
bool purchaseMerch();
void buildMonths(string*);
void vendorPicker(string);
void concessionItemPicker(Vendor);
void purchaseConcession(Item);
Event* eventPicker(vector<Event*>, string);
Merch* merchPicker(vector<Merch*>);
Merch* addMerch(Merch*);


int main() {
  // load data from files
  userController.loadUsers("users.txt");
  eventController.loadEvents("events.txt");
  ticketController.loadTickets("tickets.txt", eventController);
  merchController.loadMerch("merch.txt");
  concessionsController.loadConcessions("concessions.txt");

  // initial login prompt
  loginOrSignupPrompt();

  string months[12];
  buildMonths(months);


  // main menu
  while (true) {
    cout << "Logged in as " << userController.getCurrentUserFullName() << "\n"
         << "Enter [1] to find events and purchase tickets\n"
         << "      [2] to purchase merchandise\n"
         << "      [3] to view your reservations and buy concessions\n"
         << "      [4] to log out\n"
         << "      [5] to quit\n"
         << "Choice --> ";
    getline(cin, buf);
    if (buf == "1") {
      while (true) {
        cout << "Enter [1] to view events by date\n"
             << "      [2] to view events by location\n"
             << "      [3] to view events by sport\n"
             << "      [4] to view events by team\n"
             << "      [5] to go back...\n"
             << "Choice --> ";
        getline(cin, buf);
        if (buf == "1") {
          cout << "Months available: \n";
          for (const auto &dates : eventController.getDates()) {
            cout << months[dates - 1] << "\n";
          }
          cout << "Your choice --> ";
          getline(cin, buf);
          cout << "Events in " << buf << ":\n";
          Event* ev = eventPicker(eventController.getEventsByDate(buf), "");
          if (ev)
            seatSelectPrompt(ev);
        } else if (buf == "2") {
          cout << "Locations available: \n";
          for (const auto &location : eventController.getLocations()) {
            cout << location << "\n";
          }
          cout << "Your choice --> ";
          getline(cin, buf);
          cout << "Events in " << buf << ":\n";
          Event* ev = eventPicker(eventController.getEventsByLocation(buf), "location");
          if (ev)
            seatSelectPrompt(ev);
        } else if (buf == "3") {
          cout << "Sports available:\n";
          for (const auto &sport : eventController.getSports()) {
            cout << sport << "\n";
          }
          cout << "Your choice --> ";
          getline(cin, buf);
          cout << buf << " games:\n";
          Event* ev = eventPicker(eventController.getEventsBySport(buf), "sport");
          if(ev)
            seatSelectPrompt(ev);
        } else if (buf == "4") {
          cout << "Teams found:\n";
          for (const auto &team : eventController.getAllTeams()) {
            cout << team << "\n";
          }
          cout << "Enter the team you're looking for --> ";
          getline(cin, buf);
          cout << "Games with " << buf << ":\n";
          Event* ev = eventPicker(eventController.getEventsByTeam(buf), "");
          if (ev)
            seatSelectPrompt(ev);
        } else if (buf == "5") {
          break;
        } else {
          cout << "Invalid option, try again.\n";
        }
      }
    } else if (buf == "2") {
      while (true) {
        cout << "Enter [1] to view merch by team\n"
             << "      [2] to go back...\n"
             << "Choice --> ";
        getline(cin, buf);
        if (buf == "1") {
          while (true) {
            cout << "\nTeams found:\n";
            for (const auto& team : eventController.getAllTeams()) {
              cout << team << "\n";
            }
            cout << "Enter the team you're looking for, or enter C to check out, or enter X to cancel --> ";
            getline(cin, buf);
            if (buf == "x" || buf == "X") break;
            else if (!userController.cartIsEmpty() && (buf == "c" || buf == "C")) {
              if (!purchaseMerch())
                continue;
              else
                break;
            }
            else if (userController.cartIsEmpty() && (buf == "c" || buf == "C")) {
              cout << "Cannot check out with an empty cart. Try again.\n";
              continue;
            }
            cout << "Merchandise for " << buf << ":\n";
            Merch* mrc = merchPicker(merchController.getMerchByTeam(buf));
            userController.addToCart(addMerch(mrc));
          }
          userController.clearCart();
        }
        else if(buf == "2")
          break;
        else {
          cout << "Invalid option, try again.\n";
        }
      }
    } else if (buf == "3") {
      cout << "Your tickets: \n";
      for (const auto& tick : ticketController.getTicketsByUsername(userController.getCurrentUsername())) {
        const Event* event = tick->event;
        cout << event->sport << ":\t"
             << event->teams.first << " v. " << event->teams.second << "\t"
             << event->location << "\t" << event->date
             << "\tSeat " << tick->seat << "\tConfirmation: "
             << tick->confirmation << "\n";
      }
      cout << "Enter a ticket confirmation number to see vendors and buy concessions, or X to go back --> ";
      getline(cin, buf);
      if (buf != "X" || buf != "x") {
        auto ticketSelected = ticketController.getTicketWithConfirmation(buf);
        while (!ticketSelected) {
          cout << "Could not find a ticket with that confirmation, try again --> ";
          getline(cin, buf);
          ticketSelected = ticketController.getTicketWithConfirmation(buf);
        }
        vendorPicker(ticketSelected->event->location);
      }
    } else if (buf == "4") {
      userController.logout();
      loginOrSignupPrompt();
    } else if (buf == "5") {
      userController.saveToFile("users.txt");
      ticketController.saveToFile("tickets.txt");
      exit(0);
    } else {
      cout << "Invalid option, try again.\n";
    }
  }
}

void seatSelectPrompt(Event* event) {
  cout << "Choose your seat (A1-F99) or X to go back: ";
  getline(cin, buf);
  if (buf == "x" || buf == "X") return;
  string seat = buf;
  double cost = event->checkSeat(seat, ticketController.getSeatsTaken(event));
  if (cost >= 0) {
    cout << buf << " is available and costs $" << cost << ". Would you like to reserve it? (Y/N) --> ";
    getline(cin, buf);
    if (buf == "y" || buf == "Y") {
      while(!userController.purchase(cost)) {
        cout << "You seem to have an insufficient balance in your account. Would you like to add more? (Y/N) --> ";
        getline(cin, buf);
        if (buf == "y" || buf == "Y")
          addBalance();
        else
          seatSelectPrompt(event);
      }
      string conf = ticketController.issueTicket(*event, userController.getCurrentUsername(), seat);
      cout << "Ticket successfully purchased. Make sure to show the " <<
      "confirmation code " << conf << " at the door.\n";
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

void purchaseConcession(Item item) {
  cout << "That'll be $" << item.cost << ". Would you like to continue? (Y/N) --> ";
  getline(cin, buf);
  if (buf == "Y" || buf == "y") {
    while (!userController.purchase(item.cost)) {
      cout << "You seem to have an insufficient balance in your account. Would you like to add more? (Y/N) --> ";
      getline(cin, buf);
      if (buf == "y" || buf == "Y")
        addBalance();
      else if (buf == "n" || buf == "N") {
        return;
      }
      else {
        cout << "Invalid input. Try again.\n";
        continue;
      }
    }
    cout << "Successfully purchased " << item.name << ". Your confirmation code is "
         << concessionsController.concessionConfCode() << "\n";
  } else if (buf != "n" && buf != "N") {
    cout << "Invalid input, try again.\n";
    purchaseConcession(item);
  }
}

void concessionItemPicker(Vendor vendor) {
  cout << "Items available at " << vendor.name << ":\n";
  for (int i = 0; i < (int)vendor.items.size(); ++i) {
    cout << "[" << i + 1 << "] " << vendor.items[i].name
         << " – $" << vendor.items[i].cost << "\n";
  }
  cout << "Choose an item to purchase or enter X to go back --> ";
  getline(cin, buf);
  if (buf == "x" || buf == "X") return;
  else if (atoi(buf.c_str()) <= (int)vendor.items.size() && atoi(buf.c_str()) > 0) {
    purchaseConcession(vendor.items[atoi(buf.c_str()) - 1]);
  } else {
    cout << "Invalid choice, try again.\n";
    concessionItemPicker(vendor);
  }
}

void vendorPicker(string location) {
  cout << "Vendor's available at " << location << ":\n";
  auto vendors = concessionsController.getVendors();
  for (int i = 0; i < (int)vendors.size(); ++i) {
    cout << "[" << i + 1 << "] " << vendors[i].name << "\t(Location: "
         << vendors[i].location << ")\n";
  }

  cout << "Choose a vendor to purchase items or enter X to go back --> ";
  getline(cin, buf);
  if (buf == "x" || buf == "X") return;
  else if (atoi(buf.c_str()) <= (int)vendors.size() && atoi(buf.c_str()) > 0) {
    concessionItemPicker(vendors[atoi(buf.c_str()) - 1]);
  } else {
    cout << "Invalid choice, try again.\n";
    vendorPicker(location);
  }
}

bool purchaseMerch() {
  double total = 0;
  for (Queue<Merch*> cart = userController.currentCart(); !cart.empty(); cart.pop()) {
    cout << cart.front()->team << " " << cart.front()->type << ": $" << cart.front()->cost << "\n";
    total += cart.front()->cost;
  }
  cout << "Total cost: $" << total << ". Confirm checkout? (Y/N) --> ";
  getline(cin, buf);
  if (buf == "y" || buf == "Y") {
    bool check = true;
    while (!userController.purchase(total)) {
      cout << "You seem to have an insufficient balance in your account. Would you like to add more? (Y/N) --> ";
      getline(cin, buf);
      if (buf == "y" || buf == "Y")
        addBalance();
      else if (buf == "n" || buf == "N") {
        check = false;
        break;
      }
      else {
        cout << "Invalid input. Try again.\n";
        purchaseMerch();
      }
    }
    while (check) {
      cout << "Would you like to have the merchandise shipped to your address? (Y/N) --> ";
      getline(cin, buf);
      if (buf == "y" || buf == "Y") {
        cout << "Please enter your shipping address: ";
        getline(cin, buf);
        cout << "Your merchandise will arrive at " << buf << " 5-10 business days from now. \nShipping code: " << merchController.merchConfCode()
          << "\nMerch successfully purchased.\n\n";
        return true;
      }
      else if (buf == "n" || buf == "N") {
        string conf = merchController.merchConfCode();
        cout << "Merch successfully purchased. Make sure to show the " <<
          "confirmation code " << conf << " at the merch booth.\n\n";
        return true;
      }
      else
        cout << "Invalid input. Try again.\n";
    }
  }
  else if (buf == "n" || buf == "N")
    return false;
  else {
    cout << "Invalid input. Try again.\n";
    purchaseMerch();
  }
  return false;
}

Merch* addMerch(Merch* merch) {
  if (merch == nullptr) {
    return nullptr;
  }
  cout << "Please confirm: you are about to add a " << merch->type << " from " << merch->team << " for " << merch->cost << " to your cart\n";
  cout << "Confirm? (Y/N) --> ";
  getline(cin, buf);
  if (buf == "y" || buf == "Y") {
    cout << "Successfully added to cart.\n";
    return merch;
  }
  else if (buf == "n" || buf == "N") {
    
    cout << "Purchase cancelled.\n";
    return nullptr;
  }
  else {
    cout << "Invalid input. Try again.\n";
    addMerch(merch);
  }
}

Event* eventPicker(vector<Event*> events, string excludedField) {
  for (int i = 0; i < (int)events.size(); ++i) {
    cout << left << setw(5) << "[" + to_string(i + 1) + "] ";
    if (excludedField != "sport"){
      cout << left << setw(20) << events[i]->sport;
    }
    cout << right << setw(25) << events[i]->teams.first << " vs. " << setw(25) << left << events[i]->teams.second;
    if (excludedField != "location") {
      cout << left << setw(15) << events[i]->location;
    }
    cout << left << setw(10) << events[i]->date << "\n";
  }
  cout << "Enter the number of an event to buy a ticket, or X to go back: ";
  getline(cin, buf);
  if (buf == "x" || buf == "X") return nullptr;
  if (atoi(buf.c_str()) <= (int)events.size() && atoi(buf.c_str()) > 0) {
    return events[atoi(buf.c_str()) - 1];
  } else {
    cout << "Invalid choice, try again.\n";
    return eventPicker(events, excludedField);
  }
}

Merch* merchPicker(vector<Merch*> merch) {
  if ((int)merch.size() > 0) {
    for (int i = 0; i < (int)merch.size(); i++) {
      cout << "[" << i + 1 << "] " << merch[i]->type << ": $"
        << merch[i]->cost << "\n";
    }
    cout << "Enter the number of the merch you'd like to buy, or X to go back: ";
    getline(cin, buf);
    if (buf == "x" || buf == "X") {
      return nullptr;
    }
    if (atoi(buf.c_str()) <= (int)merch.size() && atoi(buf.c_str()) > 0) {
      return merch[atoi(buf.c_str()) - 1];
    }
    else {
      cout << "Invalid choice, try again.\n";
      return merchPicker(merch);
    }
  }
  else {
    cout << "This team has no available merchandise.\n";
    return nullptr;
  }
}

void addBalance() {
  cout << "Do you have a card registered? (Y/N) --> ";
  getline(cin, buf);
  if (buf == "n" || buf == "N") {
    cout << "Would you like to register one? (Y/N) --> ";
    getline(cin, buf);
    if (buf == "y" || buf == "Y") {
      cout << "Please enter the 16-digit number of your card: ";
      getline(cin, buf);
      userController.addCreditCard(buf);
      while (!userController.checkCard()) {
        cout << "Card number not 16-digits, try again: ";
        getline(cin, buf);
        userController.addCreditCard(buf);
      }

      cout << "Credit card successfully added to account.\n";
    }
    else if (buf == "n" || buf == "N")
      return; 
  }
  if (!userController.checkCard()) {
    cout << "No valid credit card on file, try again.\n";
    addBalance();
  }
  cout << "How much would you like to add to your balance? Enter: ";
  getline(cin, buf);
  userController.addBalance(atoi(buf.c_str()));
  cout << "$" << atoi(buf.c_str()) << " successfully added to your account.\nCurrent total: $" << userController.getBalance() << "\n\n";
}

void signupPrompt() {
  string username, password, fn, ln;
  cout << "Username (4+ characters): ";
  getline(cin, username);
  while (!userController.checkUsername(username)) {
    cout << "Username is too short or already taken, try again: ";
    getline(cin, username);
  }
  cout << "Password (8+ characters): ";
  getline(cin, password);
  while (!userController.checkPassword(password)) {
    cout << "Password too short, try again: ";
    getline(cin, password);
  }
  cout << "First name: ";
  getline(cin, fn);
  cout << "Last name: ";
  getline(cin, ln);
  userController.addUser(username, password, fn, ln);
  userController.login(username, password);
}

void loginPrompt() {
  string username;
  cout << "Enter your username or X to go back: ";
  getline(cin, username);
  if (username == "X" || username == "x") loginOrSignupPrompt();
  else {
    cout << "Enter your password: ";
    getline(cin, buf);
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
  getline(cin, buf);
  if (buf == "1") {
    loginPrompt();
  } else if (buf == "2") {
    signupPrompt();
  } else if (buf == "3") {
    userController.saveToFile("users.txt");
    ticketController.saveToFile("tickets.txt");
    exit(0);
  } else {
    cout << "Invalid option, try again.\n";
    loginOrSignupPrompt();
  }
}

void buildMonths(string* months) {
  months[0] = "January";
  months[1] = "February";
  months[2] = "March";
  months[3] = "April";
  months[4] = "May";
  months[5] = "June";
  months[6] = "July";
  months[7] = "August";
  months[8] = "September";
  months[9] = "October";
  months[10] = "November";
  months[11] = "December";
}
