#include <iostream>
#include <string>
#include <vector>
#include <array>

using namespace std;

class Meal {
protected:
    string _name;
public:
    Meal(const string& name) : _name(name) {}
    virtual ~Meal() = default;

    virtual void add_topping(const string& topping) = 0;
    virtual void prepare() const = 0;

};

class Pizza : public Meal {
protected:
    vector<string> toppings;
public:
    Pizza(const string& name) : Meal(name) {};

    const vector<string>& get_toppings() const {
        return toppings;
    }

    void add_topping(const string& topping) override {
        toppings.push_back(topping);
    }

    void prepare() const override {
        cout << "Pizza " << _name << ". Pizzaboden, belegt mit:" << endl;
        for (const auto& topping : toppings) {
            cout << "- " << topping << endl;
        }
    }
};


class Burger : public Meal {
protected:
    vector<string> toppings;
public:
    Burger(const string& name) : Meal(name) {};

    const vector<string>& get_toppings() const {
        return toppings;
    }

    void add_topping(const string& topping) override {
        toppings.push_back(topping);
    }

    void prepare() const override {
        cout << _name << ". Brötchen mit:" << endl;
        for (const auto& topping : toppings) {
            cout << "- " << topping << endl;
        }
    }
};



int main(int argc, char* argv[])
{
    Pizza* mista = new Pizza("Mista");
    mista->add_topping("Salami");
    mista->add_topping("Pilzen");
    Meal* hawaii = new Pizza("Hawaii");
    hawaii->add_topping("Schinken");
    hawaii->add_topping("Ananas");
    Burger* hamburger = new Burger("Hamburger");
    hamburger->add_topping("Hackfleisch");
    Meal* cheesburger = new Burger("Cheesburger");
    cheesburger->add_topping("Hackfleisch");
    cheesburger->add_topping("Käse");
    array<Meal*, 4> menu = { mista, hawaii, hamburger, cheesburger };
    for (Meal* g : menu)
    {
        g->prepare(); //*[1]
    }
    return 0;
}

