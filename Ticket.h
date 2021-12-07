#include "Event.h"

using namespace std;

#ifndef TICKET_H
#define TICKET_H
struct Ticket {
    string confirmation;
    string seat;
    int eventID;
    string username;
    static Ticket parse(const string&);
    string to_str();
};

class TicketController {
    int numTickets = 0;
    DynamicArray<Ticket> tickets;
public:
    void loadTickets(const string&);
    vector<string> getSeatsTaken(const Event&);
    vector<Ticket*> getTicketsByUsername(const string&);
    string issueTicket(const Event&, const string, const string);
    void saveToFile(const string&);
};
#endif //TICKET_H
