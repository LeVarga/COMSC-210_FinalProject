#include <fstream>
#include <sstream>
#include <math.h>

#include "Event.h"

double Event::checkSeat(string seat, vector<string> seatsTaken) {
  double seatPrice = baseTicketPrice;
  if (!seat.empty()) {
    char letter = seat[0];
    seat.erase(seat.begin());
    int number = atoi(seat.c_str());
    if (number < 1) return -1;

    // check availability
    for (const auto& s : seatsTaken) {
      if (s == seat) return -1;
    }

    // seat pricing logic
    switch (letter) {
    case 'A': seatPrice *= 5; break;
    case 'B': seatPrice *= 2.5; break;
    case 'C': seatPrice *= 2; break;
    case 'D': seatPrice *= 1.5; break;
    case 'E': seatPrice *= 1.15; break;
    case 'F': break;
    default: return -1;
    }
    seatPrice *= pow(99 - number + 1, 0.1);
    return seatPrice;
  }
  return -1;
}

Event Event::parse(const string &line) {
  istringstream istr(line);
  string fields[6];
  string tmp;
  int i = 0;
  while (getline(istr, tmp, ';')) {
    fields[i++] = tmp;
  }
  Event event;
  event.id = atoi(fields[0].c_str());
  event.sport = fields[1];
  event.date = fields[2];
  event.location = fields[3];
  event.teams.first = fields[4];
  event.teams.second = fields[5];
  return event;
}

void EventController::loadEvents(const string &filename) {
  ifstream File(filename);
  string buf;
  while (getline(File, buf)) {
    events[numEvents] = Event::parse(buf);
    numEvents++;
  }
}

set<string> EventController::getLocations() {
  set<string> locations;
  for (int i = 0; i < numEvents; ++i) {
    locations.insert(events[i].location);
  }
  return locations;
}

vector<Event*> EventController::getEventsByLocation(const string& location) {
  vector<Event*> eventsAtLocation;
  for (int i = 0; i < numEvents; ++i) {
    if (events[i].location == location) {
      eventsAtLocation.push_back(&events[i]);
    }
  }
  return eventsAtLocation;
}

set<string> EventController::getSports() {
  set<string> sports;
  for (int i = 0; i < numEvents; ++i) {
    sports.insert(events[i].sport);
  }
  return sports;
}

vector<Event*> EventController::getEventsBySport(const string& sport) {
  vector<Event*> eventsBySport;
  for (int i = 0; i < numEvents; ++i) {
    if (events[i].sport == sport) {
      eventsBySport.push_back(&events[i]);
    }
  }
  return eventsBySport;
}

set<string> EventController::getAllTeams() {
  set<string> teams;
  for (int i = 0; i < numEvents; ++i) {
    teams.insert(events[i].teams.first);
    teams.insert(events[i].teams.second);
  }
  return teams;
}

vector<Event*> EventController::getEventsByTeam(const string& team) {
  vector<Event*> eventsWithTeam;
  for (int i = 0; i < numEvents; ++i) {
    if (events[i].teams.first == team || events[i].teams.first == team) {
      eventsWithTeam.push_back(&events[i]);
    }
  }
  return eventsWithTeam;
}

Event* EventController::getEventWithID(const int &id) {
  for (int i = 0; i < numEvents; ++i) {
    if (events[i].id == id) return &events[i];
  }
  return nullptr;
}