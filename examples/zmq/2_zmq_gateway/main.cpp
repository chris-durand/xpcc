
#include <xpcc/debug/logger.hpp>
#include <xpcc/architecture.hpp>

#include <xpcc/architecture/interface/can.hpp>
#include <xpcc/architecture/platform/driver/uart/hosted/serial_interface.hpp>
#include <xpcc/architecture/platform/driver/can/canusb/canusb.hpp>
#include <xpcc/architecture/platform/driver/can/socketcan/socketcan.hpp>

#include <xpcc/communication.hpp>
#include <xpcc/communication/xpcc/backend/can.hpp>

#include <xpcc/communication/xpcc/backend/can/connector.hpp>
#include <xpcc/communication/xpcc/backend/zeromq/connector.hpp>

#include <xpcc/communication/xpcc/backend/event_poller.hpp>

/**
 * Listens to a CAN bus connected by a CAN2USB and publishes xpcc messages with zeromq.
 *
 * How to use:
 * - Connect a CAN2USB to a CAN bus with traffic.
 * - Connect this CAN2USB by USB to your computer.
 * - Make sure the user can access the /dev/ttyUSB0 device.
 *   Brute force method is: #sudo chmod 777 /dev/ttyUSB0
 *   A wiser method is to add the user the appropriate group
 * - Adjust the baud rate of the CAN bus in this example.
 * - Do
 *   scons run
 * - All xpcc messages will be published on port 8211 by zeromq
 */

// Default baud rate
static constexpr uint32_t canBusBaudRate = 125000;

static xpcc::hosted::SerialInterface serialInterface;

/* Either use an USB CAN2USB adaptor with xpcc Lawicel interpreter
   or use a CAN controller supported by Linux' SocketCAN.

   With SocketCAN the baudrate must be set with the operating system.
   $ ip link set can0 type can bitrate
*/
static xpcc::hosted::CanUsb< xpcc::hosted::SerialInterface > canUsb(serialInterface);
// static xpcc::hosted::SocketCan canSocket;

using CanDriver = xpcc::hosted::CanUsb< xpcc::hosted::SerialInterface >;

static xpcc::CanConnector< CanDriver > canConnector(&canUsb);
// static xpcc::CanConnector< xpcc::hosted::SocketCan > canConnector(&canSocket);

#undef XPCC_LOG_LEVEL
#define	XPCC_LOG_LEVEL xpcc::log::DEBUG

int
main()
{
	XPCC_LOG_DEBUG << "ZeroMQ SocketCAN XPCC bridge" << xpcc::endl;

	serialInterface.setBaudRate(115200);
	serialInterface.setDeviceName("/dev/ttyUSB0");

	if (not canUsb.open()) {
	//if (not canSocket.open("can0" /*, canBusBaudRate */)) {
		XPCC_LOG_ERROR << "Could not open port" << xpcc::endl;
		exit(EXIT_FAILURE);
	}

	xpcc::EventPoller readPoller;

	const std::string endpointOut = "tcp://*:8211";
	const std::string endpointIn  = "tcp://*:8212";

	xpcc::ZeroMQConnector zmqConnector(endpointIn, endpointOut, xpcc::ZeroMQConnector::Mode::PubPull);

	canUsb.setReadEvent(readPoller.getEventSender());
	zmqConnector.setReadEvent(readPoller.getEventSender());

	XPCC_LOG_DEBUG << "Entering main loop" << xpcc::endl;

	while(true)
	{
		readPoller.wait();

		canConnector.update();
		zmqConnector.update();

		while (canConnector.isPacketAvailable())
		{
			xpcc::Header header = canConnector.getPacketHeader();
			xpcc::SmartPointer payload = canConnector.getPacketPayload();

			XPCC_LOG_DEBUG << "C->Z " << header << " " << payload.getSize() << " " << payload << xpcc::endl;

			zmqConnector.sendPacket(header, payload);

			canConnector.dropPacket();
		}

		while (zmqConnector.isPacketAvailable())
		{
			xpcc::Header header = zmqConnector.getPacketHeader();
			xpcc::SmartPointer payload = zmqConnector.getPacketPayload();

			XPCC_LOG_DEBUG << "Z->C " << header << " " << payload.getSize() << " " << payload << xpcc::endl;

			canConnector.sendPacket(header, payload);

			zmqConnector.dropPacket();
		}
	}

	canUsb.close();
	// canSocket.close();
}
