#include <fstream>
#include <sstream>

#include "Event.h"

Event Event::parse(const string &line) {
  istringstream istr(line);
  string fields[5];
  string tmp;
  int i = 0;
  while (getline(istr, tmp, ';')) {
    fields[i++] = tmp;
  }
  Event event;
  event.sport = fields[0];
  event.date = fields[1];
  event.location = fields[2];
  event.teams.first = fields[3];
  event.teams.second = fields[4];
  return event;
}

void EventController::loadEvents(const string &filename) {
  ifstream File(filename);
  string buf;
  while (getline(File, buf)) {
    events[numEvents] = Event().parse(buf);
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

vector<Event> EventController::getEventsByLocation(const string& location) {
  vector<Event> eventsAtLocation;
  for (int i = 0; i < numEvents; ++i) {
    if (events[i].location == location) {
      eventsAtLocation.push_back(events[i]);
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

vector<Event> EventController::getEventsBySport(const string& sport) {
  vector<Event> eventsBySport;
  for (int i = 0; i < numEvents; ++i) {
    if (events[i].sport == sport) {
      eventsBySport.push_back(events[i]);
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

vector<Event> EventController::getEventsByTeam(const string& team) {
  vector<Event> eventsWithTeam;
  for (int i = 0; i < numEvents; ++i) {
    if (events[i].teams.first == team || events[i].teams.first == team) {
      eventsWithTeam.push_back(events[i]);
    }
  }
  return eventsWithTeam;
}