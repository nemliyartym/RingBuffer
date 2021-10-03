#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <memory>

class Person
{
public:
    Person();
    Person(const std::string &firstName, const std::string &lastName, const u_int &age);

    ~Person();

    Person (const Person &person) = delete;
    Person &operator =(Person &person) = delete;

    Person (Person &&person);
    Person &operator = (Person &&person);

    friend std::ostream& operator<<(std::ostream &out, const Person &person)
    {
        return out << person._firstName << " " << person._lastName << " " << person._age << std::endl;
    }

private:
    std::string _firstName;
    std::string _lastName;
    u_int _age;

};


#endif // PERSON_H
