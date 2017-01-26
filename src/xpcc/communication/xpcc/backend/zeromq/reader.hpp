// coding: utf-8
/* Copyright (c) 2016, Sascha Schade
 * Copyright (c) 2017, Christopher Durand
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef	XPCC__ZEROMQ_READER_HPP
#define	XPCC__ZEROMQ_READER_HPP

#include <thread>
#include <mutex>
#include <atomic>
#include <deque>

#include <zmqpp/zmqpp.hpp>

#include "../header.hpp"
#include "../event_poller.hpp"

#include <xpcc/debug/logger.hpp>
#undef XPCC_LOG_LEVEL
#define	XPCC_LOG_LEVEL xpcc::log::ERROR

namespace xpcc
{

/**
 * @brief	Reads packets from a zmqpp socket in a background thread
 *
 * @ingroup	backend
 *
 * @author	Christopher Durand <christopher.durand@rwth-aachen.de>
 * @author	strongly-typed
 */
class ZeroMQReader
{
public:
	static constexpr int PollTimeoutMs = 100;

	struct Packet
	{
		Packet(uint16_t size, const Header& inHeader) :
			header(inHeader), payload(size) {}

		xpcc::Header header;
		xpcc::SmartPointer payload;
	};

	ZeroMQReader(zmqpp::socket& socketIn_, std::size_t maxQueueSize_ = 1000);

	~ZeroMQReader();

	ZeroMQReader(const ZeroMQReader&) = delete;
	ZeroMQReader& operator=(const ZeroMQReader&) = delete;

	void
	start();

	void
	stop();

	bool
	isPacketAvailable() const;

	const Packet&
	getPacket() const;

	void
	dropPacket();

	void
	setReadEvent(xpcc::EventPoller::EventSender sender);

private:
	void
	receiveThread();

	bool
	readPacket(const zmqpp::message& message);

private:
	zmqpp::socket& socketIn;

	std::deque<Packet> queue;
	mutable std::mutex queueMutex;
	const std::size_t maxQueueSize;

	std::thread thread;
	std::atomic<bool> stopThread;

	xpcc::EventPoller::EventSender readEvent;
};

} // xpcc namespace

#endif // XPCC__ZEROMQ_CONNECTOR_HPP
