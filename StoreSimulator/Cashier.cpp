#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <string>
#include <boost/thread.hpp>
#include "Cashier.h"
#include "Client.h"
#include "SignalTools.h"
#include "TimeTools.h"
#include "PaymentMessage.h"
#include "Customers.h"

using namespace std;
using namespace ArvindsTools;

boost::mutex cout_mutex;

class PaymentClient : public TCP_Client {
public:
	PaymentClient() : TCP_Client() {}
	~PaymentClient() {}

	int check_frame( string packetReceived ) {
		PaymentStatus ps;
		deque<PaymentStatus> psQ;
		istringstream sin(packetReceived);
		cout<<"Received"<<packetReceived<<endl;
		while( sin>>ps ) {
			psQ.push_back( ps );
		}
		for( int i=1; i<psQ.size(); i++ ) {
			cout<<psQ.front()<<"\n";
			psQ.pop_front();
		}

	}
 };





vector< PaymentClient > paymentClients;

void quitApp(int signum) {
	cerr<<"Quitting Application.";
	for (int i=0; i<paymentClients.size();i++) {
		paymentClients[i].CloseSocket();
	}
	SingletonSignalHandler::getInstance()->endSignalCapture();
	//sleep(1);
	cout<<"\nDone cleaning up.\nGoodbye..."<<endl;
	exit(0);
}

/** Use some of my funky signal tools to ensure that the program quits gracefully when
 * Ctrl-C or Ctrl-Z are pressed.
 */
void SetupMyselfForExit() {
	SingletonSignalHandler::getInstance()->captureSignal( InterruptSignal, ::quitApp );
	SingletonSignalHandler::getInstance()->captureSignal( TerminateSignal, ::quitApp );
}


int main(int argc, char *argv[] ) {
	SetupMyselfForExit();
	TimeTools myTimer;
	cout<<"This is the cashier test program.";
	string s;
	if( argc>1 )
		s=string(argv[1]);
	else
		s=string("localhost");
	PaymentClient cashier1;
	cashier1.CreateSocket(s,"10000");
	paymentClients.push_back(cashier1);

	ostringstream sout;
	PaymentInfoMessage pim;
	pim.paymentId = 1; pim.paymentAmount = 200.0; pim.cardInfo.cardNumber="1000-9789-9012-2345";
	pim.cardInfo.creditLimit = 2000.0; pim.cardInfo.expiryDate = "6/2020"; pim.cardInfo.name="Joanne Curry";
	while ( true ) {
		sout<<pim;
		cashier1.send_frame( sout.str() );
		sleep( 10 );
	}


}
