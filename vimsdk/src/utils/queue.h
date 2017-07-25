#ifndef QUEUE_H_
#define QUEUE_H_
#include <mutex>
#include <memory>

 /*
 * 消费者队列，通过头尾两个锁实现，插入时不会阻塞node主线程
 */
class Queue
{
public:
	Queue() :head_(new node), tail_(head_.get()){}
	
	Queue(const Queue& other) = delete;
	Queue& operator=(const Queue& other) = delete;
	void *tryPop()
	{
		std::unique_ptr<node> oldHead = popHead();
		return oldHead ? oldHead->data : nullptr;
	}
	void push(void *newValue)
	{
		std::unique_ptr<node> p(new node);
		node* const newTail = p.get();
		std::lock_guard<std::mutex> tailLock(tailMutex_);
		tail_->data = newValue;
		tail_->next = std::move(p);
		tail_ = newTail;
	}

private:
	struct node
	{
		void *data = nullptr;
		std::unique_ptr<node> next;
	};
	
	node* getTail()
	{
		std::lock_guard<std::mutex> tailLock(tailMutex_);
		return tail_;
	}
	std::unique_ptr<node> popHead()
	{
		std::lock_guard<std::mutex> headLock(headMutex_);
		if (head_.get() == getTail())
		{
			return nullptr;
		}
		std::unique_ptr<node> oldHead = std::move(head_);
		head_ = std::move(oldHead->next);
		return oldHead;
	}

	std::mutex headMutex_;
	std::unique_ptr<node> head_;
	node* tail_;
	std::mutex tailMutex_;
};

#endif