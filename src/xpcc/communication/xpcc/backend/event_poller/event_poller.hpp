// coding: utf-8
/* Copyright (c) 2017, Christopher Durand
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef	XPCC__EVENT_POLLER_HPP
#define	XPCC__EVENT_POLLER_HPP

#include <thread>
#include <memory>
#include <condition_variable>
#include <mutex>

namespace xpcc
{

/**
 * @brief	Poll events without busy-waiting on hosted
 *
 * This class provides a thread synchronization mechanism to
 * wait on events occurring in a different thread.
 *
 * The main thread that wants to wait on an event creates an EventPoller
 * instance. Every worker thread is passed an EventSender object that can
 * be obtained through the EventPoller::getEventSender() member function.
 * Whenever an event occurs in the worker thread it has to call
 * EventSender::sendEvent().
 *
 * The main thread can now call EventPoller::wait() or EventPoller::waitFor()
 * which will yield the thread execution until a worker thread calls
 * sendEvent() or the specified timeout expires (in case of 
 * EventPoller::waitFor()). If sendEvent() is called during the
 * execution of the main thread the next call to wait() will not block.
 *
 * Memory management is encapsulated in the EventPoller implementation.
 * EventSender objects are copyable and remain valid after the EventPoller is
 * destructed and EventSender::sendEvent() gets a no-op.
 *
 * The EventSender class is thread-safe.
 *
 * Member functions of EventPoller may only be called from one thread.
 *
 * @ingroup	backend
 *
 * @author	Christopher Durand <christopher.durand@rwth-aachen.de>
 */
class EventPoller
{
	class Impl;

public:
	class EventSender
	{
	public:
		EventSender();

		EventSender(std::weak_ptr<Impl> parent);

		void
		sendEvent();

		bool
		isValid() const;

	private:
		std::weak_ptr<Impl> parent_;
	};

	EventPoller();

	~EventPoller();

	EventPoller(const EventPoller&) = delete;

	EventPoller&
	operator=(EventPoller&) = delete;

	EventPoller(EventPoller&&);

	EventPoller&
	operator=(EventPoller&&);

	void
	wait();

	template< class Rep, class Period >
	bool
	waitFor(const std::chrono::duration<Rep, Period>& timeout);

	bool
	hasEventOccurred() const;

	EventSender
	getEventSender() const;

private:
	std::shared_ptr<Impl> impl_;
};

} // xpcc namespace

#endif // XPCC__EVENT_POLLER_HPP
