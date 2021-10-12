#include <QCoreApplication>
#include "ringbuffer.h"
#include "person.h"
#include "vector"

int main(int , char *[])
{
    RingBuffer<int> ring(6);

    ring.push(1);
    ring.push(2);
    ring.push(3);
    ring.push(4);

    ring.printBuff();

    ring.setCapacity(4);
    ring.setCapacity(2);

    ring.printBuff();

}


