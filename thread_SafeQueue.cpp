#include <mutex>
#include <queue>
#include <thread>

template <typename t>
class threadsafe_queue
{
private:
	mutable std::mutex mut;
	std::queue<std::shared_ptr<t> > data_queue;
	std::condition_variable data_cond;
public:
	threadsafe_queue() {}
	
	void wait_and_pop(t& value)
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		value = std::move(*data_queue.front());
		data_queue.pop();
	}
	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return false;
		value = std::move(*data_queue.front());
		data_queue.pop();
		return true;
	}

	std::shared_ptr<t> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		std::shared_ptr<t> res = data_queue.front();
		data_queue.pop();
		return res;
	}
	std::shared_ptr<t> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if(data_queue.empty())
			return std::shared_ptr<t>();
		std::shared_ptr<t> res = data_queue.front();
		data_queue.pop();
		return res;
	}
	void push(t new_value)
	{
		std::shared_ptr<t> data(
			std::make_shared<t>(std::move(new_value)));
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(data);
		data_cond.notify_one();
	}
	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();
	}
};
