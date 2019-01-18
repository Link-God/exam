#include "pch.h"
#include <iostream>
#include <any>
#include <optional>

class my_any {
public:
	my_any():
		held_{nullptr} {}

	my_any(const my_any& other):
		held_{other.held_} {}

	template <class ValueType>
	my_any(ValueType&& other):
		held_(new holder<ValueType>(other))
	{}
	my_any& operator=(const my_any& other)
	{
		held_ = other.held_;
	}
	const std::type_info& type() const noexcept	
	{
		return held_->type_info();
	}
	struct base_holder
	{
		virtual ~base_holder() {}
		virtual const std::type_info& type_info() const = 0;
	};

	template<typename ValueType> struct holder : base_holder
	{
		holder(const ValueType& t) : t_(t) {}
		const std::type_info& type_info() const
		{
			return typeid(t_);
		}
		ValueType t_;
	};
public:
	base_holder* held_;
};

template<class ValueType>
ValueType any_cast(const my_any& operand) 
{
	if (typeid(ValueType) != operand.type())
		throw std::runtime_error("Bad any cast");
	return static_cast<my_any::holder<ValueType>*> (operand.held_)->t_;
}

template<class ValueType>
ValueType* any_cast(my_any* operand) 
{
	return static_cast<my_any::holder<ValueType>*> operand->t_;
}


int main()
{
	my_any a(2);
	auto b = any_cast<int>(a);
    std::cout << "Hello World!\n"; 
}
