#include <iostream>
#include <exception>
#include <stack>
#include <mutex>
#include <queue>
#include <memory>

struct empty_stack : std::exception
{
	std::string msg;
	empty_stack(const std::string& msg) :msg(msg)
	{}
	const char *what() const noexcept
	{
		return msg;
	}
};

template<typename t>
class threadsafe_stack
{
private:
std::stack<t>data;
mutable std::mutex m;

public:

threadsafe_stack() {}

threadsafe_stack(const threadsafe_stack&other)
{
	std::lock_guard<std::mutex>lock(other.m);
	data = other.data;
}
threadsafe_stack&operator=(const threadsafe_stack&) = delete;
void push(t new_value)
{
	std::lock_guard<std::mutex>lock(m);
	data.push(std::move(new_value)); 
}
std::shared_ptr<t> pop()
{
	std::lock_guard<std::mutex>lock(m);
	if (data.empty()) throw empty_stack(); 
	std::shared_ptr<t> const res(
		std::make_shared<t>(std::move(data.top())));
	data.pop();
	return res;
}
void pop(t&value)
{
	std::lock_guard<std::mutex> lock(m);
	if (data.empty()) throw empty_stack("error");
	value = std::move(data.top());
	data.pop();
}
bool empty()const
{
	std::lock_guard<std::mutex> lock(m);
	return data.empty();
}
};
