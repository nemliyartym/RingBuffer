#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include <iostream>
#include <memory>
#include <utility>
#include <mutex>
#include <shared_mutex>


template<typename T>
class RingBuffer
{
public:
    explicit RingBuffer(const size_t &capacity = 1) :
        _buf(std::unique_ptr<std::pair<bool,T>[]>(std::make_unique<std::pair<bool,T>[]>(capacity))),
        _capacity(capacity), _size(0), _empty(true),_full(false),_head(0), _tail(0)
    {

    }

    RingBuffer (const RingBuffer &buff)
    {

        _capacity = buff._capacity;
        _size = buff._size;

        _empty = buff._empty;
        _full = buff._full;

        _head = buff._head;
        _tail = buff._tail;

        _buf = std::make_unique<std::pair<bool,T>[]>(_capacity);
        for (size_t i = 0; i != _size; ++i) {
            _buf[i].first = buff._buf[i].first;
            _buf[i].second = buff._buf[i].second;
        }

    }

    RingBuffer &operator = (RingBuffer &buff)
    {
        if (&buff == this)
            return *this;

        _capacity = buff._capacity;
        _size = buff._size;

        _empty = buff._empty;
        _full = buff._full;

        _head = buff._head;
        _tail = buff._tail;


        _buf = std::make_unique<std::pair<bool,T>[]>(_capacity);
        for (size_t i = 0; i != _size; ++i) {
            _buf[i].first = buff._buf[i].first;
            _buf[i].second = buff._buf[i].second;
        }

        return *this;
    }

    RingBuffer (RingBuffer &&buff)
    {
        _buf = std::move(buff._buf);
        buff._buf = std::make_unique<std::pair<bool,T>[]>(_capacity);

        _capacity = std::move(buff._capacity);
        _size = std::move(buff._size);
        buff._size = 0;

        _empty = std::move(buff._empty);
        _full = std::move(buff._full);

        _head = std::move(buff._head);
        _tail = std::move(buff._tail);

        buff.clear();
    }

    RingBuffer &operator = (RingBuffer &&buff) {
        if (&buff == this)
            return *this;

        _buf = std::move(buff._buf);
        buff._buf = std::make_unique<std::pair<bool,T>[]>(_capacity);

        _capacity = std::move(buff._capacity);
        _size = std::move(buff._size);
        buff._size = 0;

        _empty = std::move(buff._empty);
        _full = std::move(buff._full);

        _head = std::move(buff._head);
        _tail = std::move(buff._tail);

        buff.clear();
        return *this;
    }

    /*!
     * \brief emplace - создание элемента внутри буффера с полученными аргументами
     * \param args - аругменты
     */
    template <typename... Args>
    void emplace(Args&&... args)
    {
        std::lock_guard<std::shared_timed_mutex> m(_mutex);

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


    /*!
     * \brief push - вставка элемента в буфер
     * \param item - элемент
     */
    void push(T &&item)
    {
        std::lock_guard<std::shared_timed_mutex> m(_mutex);

        emplace(std::move(item));
    }

    void push (const T &item)
    {
        std::lock_guard<std::shared_timed_mutex> m(_mutex);
        _buf[_tail] = std::make_pair(true,item);

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

    /*!
     * \brief pop - получит элемент из буфера
     * \return
     */
    T pop()
    {
        std::lock_guard<std::shared_timed_mutex> m(_mutex);

        if (!_size) {
            return T();
        }

        auto res = _buf[_head].second;
        _buf[_head].first = false;
        if (--_size < 0) {
            _empty = true;
            _size = 0;
        }

        _head = (_head +1) % _capacity;

        return res;
    }

    /*!
     * \brief at - произвольный доступ к элментам буфера по индексу, в слуае выхода за размер буфера генерируется исключение out_of_range
     * \param n - идекс элмента
     * \return ссылку на элемент
     */
    T& at (const size_t n) const
    {
        std::shared_lock<std::shared_timed_mutex> m(_mutex);

        if (n >= _size) {
            throw std::out_of_range("Out of range");
        }
        return _buf[n].second;
    }

    /*!
     * \brief operator [] - произвольный доступ к элментам буфера по индексу
     * \param n - идекс элмента
     * \return ссылку на элемент
     */
    T& operator [] (const size_t n) const
    {
        std::shared_lock<std::shared_timed_mutex> m(_mutex);

        return _buf[n].second;
    }

    /*!
     * \brief setCapacity устанавливает емкость буфера, если новая емкость будет больше чем текущий size буфера, то будут удаленны элменты с головы буфера
     * \param newCapacity новая емкость буфера
     */
    void setCapacity(const size_t newCapacity) {
        std::lock_guard<std::shared_timed_mutex> m(_mutex);

        if (newCapacity == _capacity) return;

        std::unique_ptr<std::pair<bool,T>[]> tmpBuf = std::make_unique<std::pair<bool,T>[]>(newCapacity);

        int offset = _size >= newCapacity ?  _size - newCapacity : 0;

        for (size_t i = 0; i != _size; ++i) {
            tmpBuf[i] = std::move(_buf[ (i + _head + offset) % _capacity]);
        }

        _buf = std::move(tmpBuf);

        _size = offset != 0 ? newCapacity : _size;
        _capacity = newCapacity;
        _tail = _size % newCapacity;
        _head = 0;
    }

    /*!
     * \brief emty проверяет, пуст ли контейнер
     * \return true если в контейнер нет элементов, иначе false
     */
    bool emty () const
    {
        std::shared_lock<std::shared_timed_mutex> m(_mutex);

        return _empty;
    }

    /*!
     * \brief size - возвращает количетсво элментов
     * \return возвращает количетсво элментов
     */
    size_t size() const
    {
        std::shared_lock<std::shared_timed_mutex> m(_mutex);

        return _size;
    }

    void clear () {
        std::lock_guard<std::shared_timed_mutex> m(_mutex);

        for (size_t i = 0; i != _size; ++i){
            _buf[(i + _head) % _capacity].first = false;
        }
        _size = 0;
        _empty = true;
        _full = false;
        _head = 0;
        _tail = 0;
    }

private:
    /*!
     * \brief _buf - массив с данными
     */
    std::unique_ptr<std::pair<bool,T>[]> _buf;

    mutable std::shared_timed_mutex _mutex;


    size_t _capacity;
    size_t _size;

    bool _empty;
    bool _full;

    size_t _head;
    size_t _tail;
};



#endif // RINGBUFFER_H
