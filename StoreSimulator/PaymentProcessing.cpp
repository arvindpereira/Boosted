/*
 * PaymentProcessing.cpp
 *
 *  Created on: Jul 1, 2013
 *      Author: arvind
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <set>
#include <deque>
#include <sstream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "Server.h"
#include "TimeTools.h"
#include "SignalTools.h"
#include "AccountHolders.h"

#include "PaymentMessage.h"


using namespace std;
using namespace ArvindsTools;
//----------------------------------------------------------------------



//----------------------------------------------------------------------
boost::mutex cout_mutex;
boost::mutex account_mutex;
boost::mutex msgs_in_mutex;

map<string,Customer>  creditCardAccounts;
deque<pair<int,PaymentInfoMessage> > paymentMsgsIn;
deque<PaymentStatus> paymentStatusOut;

class PaymentProcessingServer : public TCP_Server
{
public:
	PaymentProcessingServer( int _port ) :
		TCP_Server( _port ) {}

	void check_command( int fd, int len, unsigned char *buf ) {
		char *sbuf = (char*) buf;
		string packetReceived = sbuf;

		cout<<"Received: "<<packetReceived;

		istringstream sin(packetReceived);
		PaymentInfoMessage pim;
		boost::lock_guard<boost::mutex> guardMsgsIn( msgs_in_mutex );
		while( sin>>pim ) {
			paymentMsgsIn.push_back( pair<int,PaymentInfoMessage>(fd,pim) );
		}
	}

}pprocessor(10000);

void quitApp(int signum) {
	cerr << "Quitting Application.";
	pprocessor.StopServer();
	SingletonSignalHandler::getInstance()->endSignalCapture();
	sleep(1);
	cout << "\nDone cleaning up.\nGoodbye..." << endl;
	exit(0);
}

/** Use some of my funky signal tools to ensure that the program quits gracefully when
 * Ctrl-C or Ctrl-Z are pressed.
 */
void SetupMyselfForExit() {
	SingletonSignalHandler::getInstance()->captureSignal(InterruptSignal,
			::quitApp);
	SingletonSignalHandler::getInstance()->captureSignal(TerminateSignal,
			::quitApp);
}

// Test if a person has got enough credit-limit for this transaction and then update
// their credit-limit
PaymentStatus getTransactionStatus( PaymentInfoMessage &pim ) {
	boost::lock_guard<boost::mutex> guard(account_mutex);
	PaymentStatus ps; ps.paymentId = pim.paymentId;
	if ( creditCardAccounts.find(pim.cardInfo.cardNumber ) != creditCardAccounts.end() ) {
		Customer cust = creditCardAccounts[ pim.cardInfo.cardNumber ];
		if( cust.getCardInfo().creditLimit > pim.paymentAmount ) {
			ps.paymentStatus = PaymentStatus::PaymentSuccessful;
			ps.paymentAmount = pim.paymentAmount;
			CardInfo cardInfo = cust.getCardInfo(); cout<<cardInfo;
			cardInfo.creditLimit -= pim.paymentAmount; // Update Credit limit.
			creditCardAccounts[ pim.cardInfo.cardNumber ].setCardInfo( cardInfo );
		}
		else ps.paymentStatus = PaymentStatus::InsufficientBalance;
	}
	else {
		ps.paymentStatus = PaymentStatus::UnknownCard;
		ps.paymentAmount = 0; // Declined.
	}
	return ps;
}


bool readMsgsIn( int &fd, PaymentInfoMessage &pim )
{
	boost::lock_guard<boost::mutex> guardMsgsIn( msgs_in_mutex );
	if( paymentMsgsIn.size() ) {
		pair<int, PaymentInfoMessage> myPair = paymentMsgsIn.front();
		fd = myPair.first; pim = myPair.second;
		paymentMsgsIn.pop_front();
		return true;
	}
	return false; // nothing new to read...
}

void displayCustInfo( ) {
	//boost::lock_guard<boost::mutex> guardCout( ::cout_mutex );
	boost::lock_guard<boost::mutex> guardCustInfo( ::account_mutex );

	ostringstream sout;
	for( map<string,Customer>::iterator it=creditCardAccounts.begin(); it!=creditCardAccounts.end(); it++ ) {
		sout<<(*it).second<<'\n';
	}
	cout<< sout.str();
}


int main()
{
	SetupMyselfForExit();
	cout<<"This is the Payment Processor."<<endl;

	Accounts custInfo("CustomerAccounts.json");
	cout<<custInfo;
	creditCardAccounts = custInfo.getCustomers();

	cout<<"Started Server for 10 seconds...";
	while(true) {
		usleep( 10000 );
		PaymentInfoMessage nextMessage; int client_fd;
		if( readMsgsIn( client_fd, nextMessage ) ) {
			PaymentStatus ps = getTransactionStatus( nextMessage );
			ostringstream sout(""); sout<<ps;
			pprocessor.send_frame(client_fd, (const char*)sout.str().c_str(), sout.str().length() );
			displayCustInfo( );
		}
	}
	pprocessor.StopServer();
}
