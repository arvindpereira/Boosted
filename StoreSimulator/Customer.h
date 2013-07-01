/*
 * Customer.h
 *
 *  Created on: Jun 30, 2013
 *      Author: arvind
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <vector>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <json_spirit.h>
#include <iomanip>
#include "Constants.h"
#include "TimeTools.h"

using namespace boost;
using namespace json_spirit;
using namespace ArvindsTools;

struct CardInfo {
	string cardNumber;
	string name;
	double creditLimit;
	string expiryDate;
};


/** A customer comes to the store, picks up P items and goes to the cashier to buy them.
 *
 */
class Customer {
	vector<Item> shoppingCart;
	double totPrice;
	int id;
	CardInfo creditCard;
	Inventory *shelves;
	map<int,Item> itemInfo;
	TimeTools myTimer;
	boost::thread customerThread;
public:
	Customer() : id( StoreSim::UnknownCustomerId ), shelves(NULL) {}

	Customer( int _id, CardInfo c_info, Inventory *_storeInv ) : id(_id), shelves( _storeInv )
	{
		srand(time(NULL) );
		// Make a copy of the item info so we don't wait for it later. (Is a hack, relax later).
		itemInfo = shelves->getAllItemInfo();
	}

	Customer( const Customer &other ) :
			id(other.id), creditCard(other.creditCard),
			itemInfo(other.itemInfo), shelves(other.shelves)
	{}

	friend void swap( Customer &a, Customer &b ) {
		std::swap( a.id, b.id );
		std::swap( a.creditCard, b.creditCard );
		std::swap( a.totPrice, b.totPrice );
		std::swap( a.itemInfo, b.itemInfo );
		std::swap( a.shelves, b.shelves );
		//std::swap( a.customerThread, b.customerThread );
	}

	Customer& operator=( Customer other )
	{
		swap(*this,other);
		return *this;
	}

	void start() {
		customerThread = boost::thread(&Customer::processQueue, this, 1);
	}

	void join() {
		customerThread.join();
	}


	int getId() const { return id; }
	void setId( int _id ) { id=_id; }

	double getTotal() {
		totPrice = 0;
		for (int i=0;i<shoppingCart.size();i++) {
			totPrice+= shoppingCart[i].getPrice();
		}
		return totPrice;
	}

	void addToCart( Item item ) {
		shoppingCart.push_back( item );
	}

	void setCardInfo( CardInfo &ci ) {
		creditCard = ci;
	}

	CardInfo getCardInfo() {
		return creditCard;
	}

	void processQueue( int N ) {
		int totalUniqueThings = rand()%20; // The Customer will try to buy upto 20 unique types of items
		for( int j=0; j< totalUniqueThings; j++ ) {
			cout<<setprecision(3)<<myTimer.timeSinceStart()<<" Customer "<<id<<"Started Shopping.";
			int timeToNextItem = rand()%5; // Walk around for some random amount of time.
			int pickItemId = rand()%50; // Assuming we know the number of items.
			int itemQty = rand()%5; // Purchase a random amount of items.
			int qtyInCart = shelves->removeAnItem( pickItemId, itemQty );
			if( qtyInCart > 0 ) {
				cout<<setprecision(3)<<myTimer.timeSinceStart()<<" Customer "<<id<<" picked "<<qtyInCart<<" of "<< itemInfo[ pickItemId ].getName();
				for( int i=0; i<qtyInCart; i++ ) {
					shoppingCart.push_back( itemInfo[pickItemId] );
				}
			} else {
				cout<<setprecision(3)<<myTimer.timeSinceStart()<<" Customer "<<id<<" did not find item "<<pickItemId<<"\n";
			}
			cout<<". Total Price of Cart:"<<getTotal()<<'\n';
			boost::posix_time::seconds walkTime( timeToNextItem );
			boost::this_thread::sleep(walkTime);
		}
		cout<<setprecision(3)<<myTimer.timeSinceStart()<<"Customer"<<id<<" is done shopping."<<endl;
	}

	friend ostream& operator<<( ostream& os, Customer &cust );

};

ostream& operator<<( ostream& os, Customer &cust ) {
	os<<"Customer :"<<cust.id;
	os<<" Name: "<<cust.creditCard.name<<", Cred.Lim: "<<cust.creditCard.creditLimit<<
			", Exp.Date: "<<cust.creditCard.expiryDate<<", Card.Num: "<<cust.creditCard.cardNumber<<'\n';
	return os;
}


#endif /* CUSTOMER_H_ */
