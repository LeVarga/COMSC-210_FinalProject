#include <string>
#include <fstream>
#include <sstream>

#include "Ticket.h"
#include "Event.h"

using namespace std;

Ticket Ticket::parse(const string &line, EventController& ec) {
  istringstream istr(line);
  string fields[4];
  string tmp;
  int i = 0;
  while (getline(istr, tmp, ';')) {
    fields[i++] = tmp;
  }
  Ticket ticket;
  ticket.confirmation = fields[0];
  ticket.username = fields[1];
  ticket.event = ec.getEventWithID(atoi(fields[2].c_str()));
  ticket.seat = fields[3];
  return ticket;
}

string Ticket::to_str() {
  return confirmation + ";"
         + username + ";"
         + to_string(event->id) + ";"
         + seat + ";";
}

void TicketController::loadTickets(const string &filename, EventController& ec) {
  ifstream File(filename);
  string buf;
  while (getline(File, buf)) {
    tickets[numTickets] = Ticket::parse(buf, ec);
    numTickets++;
  }
}

string TicketController::issueTicket(Event& event, string username, string seat) {
  srand(time(0));
  rand();
  Ticket tempTicket = Ticket();
  const char alphanumBank[] = "1234567890" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz";
  string codeGen = "";
  for (int i = 0; i < 16; i++) {
    codeGen += alphanumBank[rand() % (sizeof(alphanumBank) - 1)];
  }
  tempTicket.confirmation = codeGen;
  tempTicket.seat = seat;
  tempTicket.username = username;
  tempTicket.event = &event;
  tickets[numTickets] = tempTicket;
  numTickets++;
  return codeGen;
}

vector<string> TicketController::getSeatsTaken(const Event* event) {
  vector<string> tmp;
  for (int i = 0; i < numTickets; ++i) {
    if (tickets[i].event == event) tmp.push_back(tickets[i].seat);
  }
  return tmp;
}

Ticket* TicketController::getTicketWithConfirmation(const string& confirmation) {
  for (int i = 0; i < numTickets; ++i) {
    if (tickets[i].confirmation == confirmation) return &tickets[i];
  }
  return nullptr;
}

vector<Ticket*> TicketController::getTicketsByUsername(const string& username) {
  vector<Ticket*> tmp;
  for (int i = 0; i < numTickets; ++i) {
    if (tickets[i].username == username) tmp.push_back(&tickets[i]);
  }
  return tmp;
}

void TicketController::saveToFile(const string &filename) {
  ofstream File(filename);
  for (int i = 0; i < numTickets; ++i) {
    File << tickets[i].to_str() << "\n";
  }
  File.close();
}