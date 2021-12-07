#include <string>
#include <fstream>
#include <sstream>

#include "Ticket.h"
#include "Event.h"

using namespace std;

Ticket Ticket::parse(const string &line) {
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
  ticket.eventID = atoi(fields[2].c_str());
  ticket.seat = fields[3];
  return ticket;
}

string Ticket::to_str() {
  return confirmation + ";"
         + username + ";"
         + to_string(eventID) + ";"
         + seat + ";";
}

void TicketController::loadTickets(const string &filename) {
  ifstream File(filename);
  string buf;
  while (getline(File, buf)) {
    tickets[numTickets] = Ticket().parse(buf);
    numTickets++;
  }
}

string TicketController::issueTicket(const Event& event, string username, string seat) {
  srand(time(0));
  rand();
  Ticket tempTicket = Ticket();
  const char alphanumBank[] = "1234567890" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz";
  string codeGen = "";
  for (int i = 0; i < 16; i++)
  {
    codeGen += alphanumBank[rand() % (sizeof(alphanumBank) - 1)];
  }
  tempTicket.confirmation = codeGen;
  tempTicket.seat = seat;
  tempTicket.username = username;
  tempTicket.eventID = event.id;
  tickets[numTickets] = tempTicket;
  numTickets++;
  return codeGen;
}

vector<string> TicketController::getSeatsTaken(const Event& event) {
  vector<string> tmp;
  for (int i = 0; i < numTickets; ++i) {
    if (tickets[i].eventID == event.id) tmp.push_back(tickets[i].seat);
  }
  return tmp;
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