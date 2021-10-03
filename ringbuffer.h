#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include <iostream>
#include <memory>


template<typename T>
class RingBuffer
{
public:
    explicit RingBuffer(const size_t &capacity) :
        _buf(std::unique_ptr<T[]>(new T[capacity])),
        _capacity(capacity), _size(0), _empty(true),_head(0), _tail(0)
    {

    }

    /*!
     * \brief push - вставка элемента в буфер
     * \param item - элемент
     */
    void push(T &&item)
    {
        _buf[_tail] = std::move(item);

        _empty = false;
        if (++_size > _capacity) {
            _size = _capacity;
        }

        if (_tail == _head) {
            _head = (_head +1) % _capacity;
        }

        _tail = (_tail + 1) %_capacity;
        std::cout << "pus\n";
    }


    T pop()
    {
        if (!_size) {
            return T();
        }

        auto res = _buf[_head];
        _buf[_head] = T();
        if (--_size < 0) {
            _empty = true;
            _size = 0;
        }

        _head = (_head +1) % _capacity;

        return res;
    }

    /*!
     * \brief emty проверяет, пуст ли контейнер
     * \return true если в контейнер нет элементов, иначе false
     */
    bool emty () { return _empty; }

    /*!
     * \brief size - возвращает количетсво элментов
     * \return возвращает количетсво элментов
     */
    size_t size() const { return _size; }

    void printBuff(){
        for (size_t i = 0; i < _capacity; ++i) {
            std::cout << i << ") "<<_buf[i] << std::endl;
        }

    }

private:
    std::unique_ptr<T[]> _buf;

    size_t _capacity;
    size_t _size;

    bool _empty;

    size_t _head;
    size_t _tail;
};



#endif // RINGBUFFER_H
