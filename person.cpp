#include "person.h"

Person::Person() :
    _age (0)
{
    _firstName.clear();
    _lastName.clear();
    std::cout << "Person default\n";
}

Person::Person(const std::string &firstName, const std::string &lastName, const u_int &age) :
    _firstName(firstName), _lastName(lastName), _age(age)
{
    std::cout << "Person\n";
}

Person::~Person()
{
    std::cout <<_age<< "~Person\n";
}

//Person::Person(const Person& person) :
//    _firstName(person._firstName), _lastName(person._lastName), _age(person._age)
//{
//    std::cout << "Person &\n";
//}

//Person &Person::operator =(Person &person)
//{
//    std::cout << "Person = &\n";
//    if(&person == this)
//        return *this;

//    _firstName = person._firstName;
//    _lastName = person._lastName;
//    _age = person._age;

//    return *this;
//}

Person::Person(Person &&person) :
    _firstName(person._firstName), _lastName(person._lastName), _age(person._age)
{
    std::cout << "Person &&\n";
}

Person &Person::operator =(Person &&person)
{
    std::cout << "Person = &&\n";
    if(&person == this)
        return *this;

    _firstName = person._firstName;
    _lastName = person._lastName;
    _age = person._age;

    return *this;
}


