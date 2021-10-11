#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include <iostream>
#include <memory>
#include <utility>


template<typename T>
class RingBuffer
{
public:
    explicit RingBuffer(const size_t &capacity) :
        _buf(std::unique_ptr<std::pair<bool,T>[]>(std::make_unique<std::pair<bool,T>[]>(capacity))),
        _capacity(capacity), _size(0), _empty(true),_full(false),_head(0), _tail(0)
    {

    }

    /*!
     * \brief push - вставка элемента в буфер
     * \param item - элемент
     */
    template <typename... Args>
    void emplace(Args&&... args)
    {
        _buf[_tail] = std::make_pair(true,T(std::forward<Args>(args)...));

        _empty = false;
        if (++_size > _capacity) {
            _size = _capacity;
        }

        if (_full) {
            _head = (_head +1) % _capacity;
        }

        _tail = (_tail + 1) %_capacity;

        _full = _head == _tail;
    }

    void push(T &&item)
    {
        _buf[_tail] = std::move(std::make_pair(true,std::move(item)));

        _empty = false;
        if (++_size > _capacity) {
            _size = _capacity;
        }

        if (_full) {
            _head = (_head +1) % _capacity;
        }

        _tail = (_tail + 1) %_capacity;

        _full = _head == _tail;
    }


    T pop()
    {
        if (!_size) {
            return T();
        }

        auto res = std::move(_buf[_head].second);
        _buf[_head].first = false;
        if (--_size < 0) {
            _empty = true;
            _size = 0;
        }

        _head = (_head +1) % _capacity;

        return res;
    }

    void setCapacity(const size_t newCapacity) {

        if (newCapacity == _capacity) return;

        std::unique_ptr<std::pair<bool,T>[]> tmpBuf = std::make_unique<std::pair<bool,T>[]>(newCapacity);

        if (newCapacity >= _size) {

            for (size_t i = 0; i != _size; ++i) {
                tmpBuf[i] = std::move(_buf[ (i + _head) % _capacity]);
            }

            _buf = std::move(tmpBuf);
            _capacity = newCapacity;

            _tail = _size % _capacity;
            _head = 0;

        } else {

            for (size_t i = newCapacity; i != 0; --i) {
                tmpBuf[i] = std::move(_buf[ (i - _tail) % _capacity]);
            }

            _buf = std::move(tmpBuf);
            _capacity = newCapacity;

            _tail = _size % _capacity;
            _head = 0;

        }
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
            std::cout << i << ") ";

            if (_buf[i].first) std::cout << _buf[i].second;
            else std::cout << "-";

            if (i == _head) std::cout << " H";
            if (i == _tail) std::cout << " T";
            std::cout << std::endl;
        }
        std::cout <<"----------" << std::endl;
    }

private:
    /*!
     * \brief _buf - массив с данными
     */
    std::unique_ptr<std::pair<bool,T>[]> _buf;


    size_t _capacity;
    size_t _size;

    bool _empty;
    bool _full;

    size_t _head;
    size_t _tail;
};



#endif // RINGBUFFER_H
