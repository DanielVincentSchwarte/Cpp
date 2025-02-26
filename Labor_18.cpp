
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// Klasse für alle Personen, die Medien leihen können aus Aufgabe 10
class Person {
    string _name;
    int _check_out_duration;
public:
    Person(string name, int check_out_duration = 0);
    int get_check_out_duration() const;
    string get_name() const;
    void print() const;
};

Person::Person(string name, int check_out_duration)
    :_name(name)
    , _check_out_duration(check_out_duration)
{
}

int Person::get_check_out_duration() const
{
    return  _check_out_duration;
}

string Person::get_name() const {
    return _name;
}

void Person::print() const
{
    cout << _name;
}

class Lecturer : public Person
{
    int _examiner_id;
public:
    Lecturer(string name, int examiner_id);
    void print() const;
};

Lecturer::Lecturer(string name, int examiner_id)
    : Person(name, 90)
    , _examiner_id(examiner_id)
{
}

void Lecturer::print() const
{
    Person::print();
    cout << ", _examiner_id " << _examiner_id << endl;
}

class Student : public Person
{
    int _matriculation_number;

public:
    Student(string name, int matriculation_number);
    void print() const;
};


Student::Student(string name, int matriculation_number)
    : Person(name, 30)
    , _matriculation_number(matriculation_number)
{
}

void Student::print() const
{
    Person::print();
    cout << ", Matrikelnummer " << _matriculation_number << endl;
}

// Klasse Datum aus Aufgabe 9
class Date {

    int _day, _month, _year;

public:
    Date(int = 0, int = 0, int = 0);
    // ** neu **
    // Konvertierkonstruktor für String neu
    Date(const string& date_str) {
        stringstream ss(date_str);
        char sep;
        ss >> _day >> sep >> _month >> sep >> _year;
    }
    // ** neu **
    // Konvertierkonstruktor für C-String neu
        Date(const char* date_cstr) : Date(string(date_cstr)) {} 
    Date operator+(int days);
    friend ostream& operator<<(ostream&, const Date&);
};


Date::Date(int d, int m, int y)
    : _day(d), _month(m), _year(y)
{
}

// hier Konvertierkonstruktor für String

// hier Konvertierkonstruktor für C-String

Date Date::operator+(int days)
{
    int abs_date = _day + _month * 30 + _year * 360;
    abs_date += days;
    int y = abs_date / 360;
    abs_date = abs_date % 360;
    int m = abs_date / 30;
    abs_date = abs_date % 30;
    int d = abs_date;

    return Date(d, m, y);
}

ostream& operator<<(ostream& os, const Date& d)
{
    os << d._day << "." << d._month << "." << d._year;
    return os;
}


// Klasse für die Medien, die in der Bibliothek ausgeliehen werden
// können
class Medium
{

protected:
    // Titel des Mediums
    const string _title;
    // Verlag, der das Medium herausgibt
    const string _publisher;
    // Jahr, in dem das Medium veröffentlicht wurde
    const int _year_of_publication;
    // ausgeliehen am
    Date _date_of_check_out;
    // ausgeliehen bis
    Date _date_of_return;
    // ausgeliehen von
    Person* _lender;

public:
    // Konstruktor
    Medium(string title = "", string publisher = "", int year_of_publication = 0)
        : _title(title),
        _publisher(publisher),
        _year_of_publication(year_of_publication),
        _lender(nullptr) {}
    virtual ~Medium() {};
    // Titel zurückliefern
    string get_title() const {
        return _title;
    }
    // Ausleiher zurückliefern
    Person* get_lender() const {
        return _lender;
    }
    // das Medium "ausleihen", d.h. Person p, date_of_check_out und date_of_return eintragen
    void check_out(Person& p, Date date_of_check_out, Date date_of_return) {
        _lender = &p;
        _date_of_check_out = date_of_check_out;
        _date_of_return = date_of_return;
    }
    // Medium in der Konsole ausgeben
    virtual void print() const {
        cout << "Titel:         " << _title << endl;
        cout << "Verlag:        " << _publisher << endl;
        cout << "Jahr:          " << _year_of_publication << endl;
        if (_lender) {
            cout << "Ausleiher:     " << _lender->get_name() << " von: " << _date_of_check_out
                << " bis: " << _date_of_return << endl;;
        }
        else {
            cout << "Ausleiher:     -"<<endl;
        }
    }

    virtual Medium* clone() const {
        return new Medium(*this);
    };
};

// hier Konstruktor und Methoden

// Klasse für die Bücher als Spezialisierung von Medium
class Book : public Medium
{
    // Autor(en) des Buches
    string _author;

public:
    // Standardkonstruktor
    Book(string author = "", string title = "", string publisher = "", int year_of_publication = 0)
        : Medium(title, publisher, year_of_publication), _author(author) {}
    // das Buch auf der Konsole ausgeben
    void print() const {
        cout << "Autor:         " << _author << endl;
        Medium::print();
        cout << endl;
        
    }
    Medium* clone() const override {
        return new Book(*this);
    }
};

// hier Konstruktor und Methoden

// Klasse für die DVDs als Spezialisierung von Medium
class DVD : public Medium
{
    // Abspieldauer der DVD
    const int _c_play_time;

public:
    // Standardkonstruktor
    DVD(string title = "", string publisher = "", int year_of_publication = 0, int play_time = 0)
        : Medium(title, publisher, year_of_publication), _c_play_time(play_time) {}

    // die DVD auf der Konsole ausgeben
    void print() const {
        Medium::print();
        cout << "Dauer:         " << _c_play_time << endl;
        cout << endl;
    }
    Medium* clone() const override {
        return new DVD(*this);
    }
};

// hier Konstruktor und Methoden

// Die Klasse Library verweist auf alle Medien (Bücher und DVDs),
// die von Personen (Studierende und Dozenten) ausgeliehen werden
// können.
class Library
{
    // maximale Anzahl der Medien im Katalog
    const unsigned int _c_maximal_number_of_media;
    // Der Medienkatalog
    vector<Medium*> media;

public:
    // Standardkonstruktor
    Library(int maximal_number_of_media = 1000)
        : _c_maximal_number_of_media (maximal_number_of_media) {}

    // Kopie einer DVD oder eines Buches in den Katalog der Bibliothek eintragen
    // Die maximale Anzahl an Medien darf nicht überschritten werden
    void procure_medium(Medium& m){
        if (media.size() < _c_maximal_number_of_media) {
            media.push_back(m.clone());
        }
    }
    // alle Medien auf der Konsole ausgeben,
    // die im Titel das Suchwort enthalten
    void search_medium(string search_word) {
        cout << endl;
        cout << "Suche nach \"" << search_word << "\" Ergebnis:" << endl;
        cout << "---------------------------------------" << endl << endl;;
        for (size_t i = 0; i < media.size(); ++i) {
            if (media[i]->get_title().find(search_word) != string::npos) {
                cout << "Medium " << i << ":" << endl;
                media[i]->print();
                
            }
        }
        cout << "---------------------------------------" << endl;;
    }

    // im Medium mit der Nummer number, die Person p als Ausleiher
    // eintragen mit von-Datum d und bis-Datum d+p.ausleihdauer
    void check_out_medium(int number, Person& p, Date d) {
        if (number >= 0 && number < static_cast<int>(media.size())) {
            Date return_date = d + p.get_check_out_duration();
            media[number]->check_out(p, d, return_date);
        }
    }
    // alle Medien in der Konsole ausgeben
    void print() const{
        cout << endl;
        cout << "Bibliothekskatalog:" << endl;
        cout << "---------------------------------------" << endl << endl;
        for (size_t i = 0; i < media.size(); ++i) {
            cout << "Medium " << i << ":" << endl;
            media[i]->print();
            
        }
        cout << "---------------------------------------" << endl;
    }
};

// hier Konstruktor und Methoden

int main(int argc, char* argv[])
{
    // Umlaute etc. in der Konsole zulassen
    setlocale(LC_ALL, "");
    // Bibliothek mit 100 Plätzen initialisieren
    Library library(100);
    // Bücher und DVDs erstellen
    Book b1("U. Breymann", "C++ - Eine Einführung", "Hanser", 2016);
    Book b2("U. Breymann", "Der C++ Programmierer", "Hanser", 2017);
    Book b3("Rainer Grimm", "C++20, Get the details", "Leanpub", 2021);
    Book b4("Bartłomiej Filipek", "C++17 in detail", "Leanbub", 2018);
    Book b5("Timothy Gallwey", "The Inner Game of Tennis", "Pan", 2015);
    Book b6("Brad Gilbert", "Winning Ugly – Mentale Kriegsführung im Tennis", "riva", 2021);
    DVD d1("The IT Crowd", "2entertain", 2016, 600);
    DVD d2("Mr. Robot", "Universal Pictures UK", 2020, 2106);
    DVD d3("Chuck", "Warner Bros (Universal Pictures)", 2013, 3774);
    // Ein Buch und eine DVD ausgeben
    b1.print();
    d1.print();
    // Kopien der Bücher und DVDs (Medien) in die Bibliothek
    // einfügen
    library.procure_medium(b1);
    library.procure_medium(b2);
    library.procure_medium(b3);
    library.procure_medium(b4);
    library.procure_medium(b5);
    library.procure_medium(b6);
    library.procure_medium(d1);
    library.procure_medium(d2);
    library.procure_medium(d3);
    library.procure_medium(d3);
    // Bestand der Bibliothek ausgeben
    library.print();
    // Personen anlegen
    Student p1("Peter", 12345678);
    Student p2("Lisa", 87654321);
    Lecturer p3("Prof Miller", 4711);
    // Suchen im Bibliotheksbestand durchführen
    library.search_medium("C++");
    library.search_medium("Tennis");
    library.search_medium("The");
    // Medien ausleihen
    library.check_out_medium(1, p1, "4.5.2023");
    library.check_out_medium(7, p2, "25.05.2023");
    library.check_out_medium(4, p3, "12.10.2023");
    library.check_out_medium(8, p3, "14.03.2023");
    // Bestand der Bibliothek ausgeben
    library.print();
}

