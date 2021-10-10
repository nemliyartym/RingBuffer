#include <QCoreApplication>
#include "ringbuffer.h"
#include "person.h"
#include "vector"

int main(int , char *[])
{

    RingBuffer<int> ring(7);

    ring.push(1);
    ring.pop();
    ring.push(2);
    ring.pop();
    ring.push(3);
    ring.pop();
    ring.push(4);
    ring.pop();
    ring.push(5);
    ring.pop();
    ring.push(6);
    ring.push(7);
    ring.push(8);
    ring.push(9);
    ring.push(10);




    //ring.push(Person("Jon2","Fabia",28));
    ring.printBuff();
    ring.setCapacity(4);
    ring.printBuff();



}


