
#include <iostream>
#include <list>
#include <string>
#include <map>
#include <exception>

using namespace std;

// Hier Exception implementieren
class OutOfStockEsception : public exception {
    string _message;
public:
    OutOfStockEsception(const string& product, unsigned int requested, unsigned int available) {
        _message = "Es sind " + to_string(available) + " Artikel vom Typ " + product + " verfügbar. Es können nicht " + to_string(requested) + " Artikel verkauft werden.";
    }

    const char* what()const noexcept override {
        return _message.c_str();
    }
};

// Hier Interface implementieren 
class ISubscriber
{
public:
    virtual void update(const string& message) = 0;
};


// Hier Klassen Customer und GoldCustomer implementieren
class Customer : public ISubscriber {
public:
    int _id;
    static int _id_generator;

    Customer() : _id(++_id_generator) {}
    int get_id() const {
        return _id;
    }
    void update(const string& message)override {
        cout << "Customer " << _id << ": neue Nachricht verfügbar --> " << message << endl;
    }
};

class GoldCustomer : public ISubscriber {
public:
    int _id;
    static int _id_generator;

    GoldCustomer() : _id(++_id_generator) {}

    void update(const string& message) {
        cout << "GoldCustomer " << _id << ": neue Nachricht verfügbar --> " << message << endl;
    }
};


class Store
{
public:
    // Hier Methoden implementieren
    void subscribe(ISubscriber* subscriber) {
        _subscribers.push_back(subscriber);
    }
    void unsubscribe(ISubscriber* subscriber) {
        _subscribers.remove(subscriber);

    }
    void notify_subscribers(const string& message) {
        for (ISubscriber* subscriber : _subscribers) {
            subscriber->update(message);
        }
    }
    void deliver_products(const string& product, int quantity) {
        unsigned int previous_stock = _product_availability[product];
        _product_availability[product] += quantity;

        cout << "Vorrätige Artikel vom Typ " << product << ": " << previous_stock << endl;
        cout << "Ausgelieferte Artikel vom Typ " << product << ": " << quantity << endl;
        cout << "Neuer Bestand: " << _product_availability[product] << endl;

        if (previous_stock == 0) {
            notify_subscribers("Neue Artikel vom Typ " + product + " verfügbar.");
        }
    }
    void sell_products(const string& product, unsigned int quantity) {
        unsigned int previous_stock = _product_availability[product];

        if (previous_stock < quantity) {
            throw OutOfStockEsception(product, quantity, previous_stock);
        }

        _product_availability[product] -= quantity;

        cout << "Vorrätige Artikel vom Typ " << product << ": " << previous_stock << endl;
        cout << "Verkaufte Artikel vom Typ " << product << ": " << quantity << endl;
        cout << "Neuer Bestand: " << _product_availability[product] << endl;

        if (_product_availability[product] == 0) {
            notify_subscribers("Artikel vom Typ " + product + " nicht mehr verfügbar");
        }

    }


private:
    list<ISubscriber*> _subscribers;
    map<string, unsigned int> _product_availability{ {"iPhone", 0}, {"Galaxy", 5} };
};


int Customer::_id_generator = 100;
int GoldCustomer::_id_generator = 0;

void manage_store()
{
    try
    {
        Store* store = new Store;
        ISubscriber* customer_1 = new Customer();
        store->subscribe(customer_1);
        ISubscriber* customer_2 = new GoldCustomer();
        store->subscribe(customer_2);
        ISubscriber* customer_3 = new GoldCustomer();
        store->subscribe(customer_3);
        store->deliver_products("iPhone", 5);
        store->unsubscribe(customer_3);
        store->sell_products("iPhone", 3);
        ISubscriber* customer_4 = new Customer();
        store->subscribe(customer_4);
        store->deliver_products("iPhone", 5);
        store->sell_products("iPhone", 7);
        store->unsubscribe(customer_1);
        ISubscriber* customer_5 = new GoldCustomer();
        store->subscribe(customer_5);
        store->deliver_products("iPhone", 15);
        store->sell_products("Galaxy", 8);
    }
    catch (exception& e)
    {
        cout << e.what() << endl;
    }
    catch (...)
    {
        cout << "Ein unbekannter Fehler ist aufgetreten." << endl;
    }
}

int main(int argc, char* argv[])
{
    manage_store();
    return 0;
}

