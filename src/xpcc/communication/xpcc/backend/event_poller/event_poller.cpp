// coding: utf-8
/* Copyright (c) 2017, Christopher Durand
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include <thread>
#include <memory>
#include <condition_variable>
#include <mutex>

#include "event_poller.hpp"

namespace xpcc
{

class EventPoller::Impl
{
public:
	Impl();

	void
	sendEvent();

	void
	wait();

	template< class Rep, class Period >
	bool
	waitFor(const std::chrono::duration<Rep, Period>& timeout);

	bool
	hasEventOccurred() const;

private:
	bool eventOccurred_;

	std::condition_variable cv_;
	mutable std::mutex mutex_;
};


// ----------------------------------------------------------------------------
EventPoller::EventPoller() : impl_(std::make_shared<EventPoller::Impl>())
{
}

// ----------------------------------------------------------------------------
EventPoller::~EventPoller() = default;

// ----------------------------------------------------------------------------
EventPoller::EventPoller(EventPoller&& other)
	: impl_(std::move(other.impl_))
{
	other.impl_ = std::make_shared<EventPoller::Impl>();
}

// ----------------------------------------------------------------------------
EventPoller&
EventPoller::operator=(EventPoller&& other)
{
	impl_ = std::move(other.impl_);
	other.impl_ = std::make_shared<EventPoller::Impl>();

	return *this;
}

// ----------------------------------------------------------------------------
void
EventPoller::wait()
{
	impl_->wait();
}

// ----------------------------------------------------------------------------
template< class Rep, class Period >
bool
EventPoller::waitFor(const std::chrono::duration<Rep, Period>& timeout)
{
	return impl_->waitFor(timeout);
}

// ----------------------------------------------------------------------------
EventPoller::EventSender
EventPoller::getEventSender() const
{
	return EventPoller::EventSender(impl_);
}

// ----------------------------------------------------------------------------
EventPoller::Impl::Impl() : eventOccurred_(false)
{
}

// ----------------------------------------------------------------------------
void EventPoller::Impl::sendEvent()
{
	{
		std::lock_guard<std::mutex> lock(mutex_);
		eventOccurred_ = true;
	}

	cv_.notify_one();
}

// ----------------------------------------------------------------------------
void EventPoller::Impl::wait()
{
	std::unique_lock<std::mutex> lock(mutex_);

	if(eventOccurred_) {
		eventOccurred_ = false;
		return;
	}

	cv_.wait(lock, [this](){ return eventOccurred_; });
	eventOccurred_ = false;
}

// ----------------------------------------------------------------------------
template< class Rep, class Period >
bool
EventPoller::Impl::waitFor(const std::chrono::duration<Rep, Period>& timeout)
{
	std::unique_lock<std::mutex> lock(mutex_);

	if(eventOccurred_) {
		return true;
	}

	bool event = cv_.wait_for(lock, timeout, [this]() {
		return eventOccurred_;
	});
	eventOccurred_ = false;

	return event;
}

// ----------------------------------------------------------------------------
bool
EventPoller::Impl::hasEventOccurred() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	return eventOccurred_;
}

// ----------------------------------------------------------------------------
EventPoller::EventSender::EventSender() = default;

// ----------------------------------------------------------------------------
EventPoller::EventSender::EventSender(std::weak_ptr<Impl> parent)
	: parent_(parent)
{
}

// ----------------------------------------------------------------------------
void
EventPoller::EventSender::sendEvent()
{
	std::shared_ptr<EventPoller::Impl> ptr = parent_.lock();
	if(ptr) {
		ptr->sendEvent();
	}
}

// ----------------------------------------------------------------------------
bool
EventPoller::EventSender::isValid() const
{
	return not parent_.expired();
}

} // xpcc namespace
