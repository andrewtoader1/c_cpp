
#ifndef _ABSTRACT_QUEUE_H_
#define _ABSTRACT_QUEUE_H_

// type T must be orderable (support operator<)
template <typename T>
class AbstractQueue
{
public:

  // return true if the priority queue is empty
  virtual bool isEmpty() = 0;

  // add item to the back of the queue
  virtual void enqueue(const T& item) = 0;

  // add item to the front of the queue
  virtual void enqueueFront(const T& item) = 0;

  // remove item from the front of the queue
  virtual void dequeue() = 0;

  // remove item from the back of the queue
  virtual void dequeueBack() = 0;

  // get the value of the item at the front of the queue
  virtual T peekFront() = 0;

  // get the value of the item at the back of the queue
  virtual T peekBack() = 0;

  // display the contents of the queue
  virtual void display() = 0;
};

#endif // _ABSTRACT_QUEUE_H_
