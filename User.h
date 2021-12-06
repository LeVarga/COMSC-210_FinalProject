#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <math.h>

#include "DynamicArray.h"
#include "Event.h"
#include "HashTable.h"

using namespace std;

struct User {
private:
    hash <string> hsh;
    size_t password;
public:
    string username;
    string firstName;
    string lastName;
    string savedCreditCard;
    double accountBalance;
    vector<Ticket*> purchasedTickets;

    const bool checkPassword(const string&) const;
    void setPassword(const string&);
    string to_str();
    static User parse(const string&);
};

class UserController {
    static int hashCode(const string&);
    string buf;
    HashTable<string, User, 100> users = HashTable<string, User, 100>(hashCode);
    User *currentUser;
public:
    bool login(string, string);
    void loadUsers(string);
    bool checkUsername(string);
    bool checkPassword(string);
    void addUser(string, string, string, string);
    void saveToFile(string);
    void logout();
    string getCurrentUserFullName() const;
    bool purchaseTicket(Ticket*, double);
    bool checkCard();
    void addCreditCard(string);
    void addBalance(double);
    double getBalance() const;
};


#endif //USER_H
