#pragma once
#include <vector>
#include <functional>

using std::bind;
using std::function;
using namespace std::placeholders;

template<typename T>
class Event {
public:
	struct Subscriber {
		function<void(const T&)> _bound;
	};

	T& EventObject;
private:
	std::vector<typename Event<T>::Subscriber> subscribers;
public:
	Event() = default;

	typename Event<T>::Subscriber Subscribe(function<void(const T&)>&& func) {
		typename Event<T>::Subscriber out = { func };
		subscribers.emplace_back(out);
		return out;
	}
	typename Event<T>::Subscriber Subscribe(void(*func)(const T&)) {
		return Subscribe(function<void(const T&)>(*func));
	}
	bool Unsubscribe(typename Event<T>::Subscriber&& subscriber) {
		for (auto it = subscribers.begin(); it != subscribers.end(); it++) {
			if ((*it) == subscriber) {
				subscribers.erase(it);
				return true;
			}
		}
		return false;
	}

	void operator()(const T& ev) {
		for (auto it : subscribers) {
			it._bound(ev);
		}
	}
	void operator()() {
		for (auto it : subscribers) {
			it._bound(EventObject);
		}
	}
};