#include <iterator>
#include <iterator>
#include <unordered_map>

template <typename Iterator, typename T>
std::pair<bool, std::pair<Iterator, Iterator>> find_two_elements_with_sum(Iterator first, Iterator last, T c)
{
	std::unordered_map<T,Iterator> my_map;
	std::pair<bool, std::pair<Iterator, Iterator>> p;
	Iterator one = last;
	Iterator two = last;
	p = std::make_pair<bool, std::pair<Iterator, Iterator>>(false, std::make_pair<Iterator, Iterator>(one, two));
	for (auto i = first; i != last; ++i)
	{
		auto f = my_map.find(*i);
		if (f != my_map.end())
		{
			p.first = true;
			one = i;
			two = f->second;
			p.second = std::make_pair<Iterator, Iterator>(one, two);
			return p;
		}
		else
		{
			my_map.insert(std::make_pair<T, Iterator>(c - (*i), i));
		}
	}
	return p;
}

template <typename Iterator1, typename Iterator2>
Iterator2 merge(Iterator1 first1, Iterator1 last1, Iterator1 first2, Iterator1 last2, Iterator2 output)
{
	f1 = first1;
	f2 = first2;
	while (f1 != last1 && f2 != last2)
	{
		if (*f1 < *f2)
		{
			*output = *f1;
			++f1;
		}
		else
		{
			*output = *f2;
			++f2;
		}
		++output;
	}
	while (f1 != last1)
	{
		*output = *f1;
		++f1;
		++output;
	}
	while (f2 != last2)
	{
		*output = *f2;
		++f2;
		++output;
	}
	return output;
}
