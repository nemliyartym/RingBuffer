#include <QCoreApplication>
#include "ringbuffer.h"
#include "person.h"


int main(int , char *[])
{
    RingBuffer<Person> ring(1);

    ring.push(Person("Jon","Fabia",25));

//    ring.push(Person("Jon2","Fabia2",26));
//    ring.push(Person("Jon3","Fabia3",27));

    //ring.pop();

    std::cout << "END\n";
}


