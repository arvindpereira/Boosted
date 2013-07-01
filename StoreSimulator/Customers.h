/*
 * Customers.h
 *
 *  Created on: Jun 30, 2013
 *      Author: arvind
 */

#ifndef CUSTOMERS_H_
#define CUSTOMERS_H_

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <json_spirit.h>
#include "Customer.h"

using namespace std;
using namespace json_spirit;

class Customers // Helper class acting as a container for customer information.
{
	map<int,Customer> customers;
public:
	Customers( string fileName ) {
		fstream fin( fileName.c_str() );
		fin>>(*this);
	}

	friend ostream& operator<<( ostream& os, Customers& );
	friend istream& operator>>( istream& is, Customers& customers);

	map<int,Customer> getCustomers() {
		return this->customers;
	}
};

ostream& operator<<( ostream& os, Customers& cust)
{
	for( map<int,Customer>::iterator it=cust.customers.begin(); it!=cust.customers.end(); it++ ) {
		os<<it->second;
	}
	return os;
}

istream& operator>>( istream& is, Customers& customers )
{
	Value custData;
	read( is, custData );

	const Object& custAccounts = custData.get_obj();
	const Pair& creditCards = custAccounts[0];
	if( creditCards.name_ == "creditCards" ) {
		const Array& custAccountArray = creditCards.value_.get_array();
		for (unsigned i=0; i<custAccountArray.size(); i++ ) {
			const Object& cardData = custAccountArray[i].get_obj();
			Customer newCust;
			CardInfo newCard;
			for( Object::size_type j=0; j!=cardData.size(); j++ ) {
				const Pair& aCard = cardData[j];
				if( aCard.name_ =="id" ) {
					newCust.setId( aCard.value_.get_int() );
				}
				else if( aCard.name_=="cardNumber" ) {
					newCard.cardNumber = aCard.value_.get_str();
				}
				else if( aCard.name_=="creditLimit" ) {
					newCard.creditLimit = aCard.value_.get_real();
				}
				else if( aCard.name_=="expiryDate" ) {
					newCard.expiryDate = aCard.value_.get_str();
				}
				else if( aCard.name_=="name" ) {
					newCard.name = aCard.value_.get_str();
				}
			}
			if( newCust.getId() != StoreSim::UnknownCustomerId ) {
				newCust.setCardInfo( newCard );
				customers.customers[ newCust.getId() ] = newCust;
			}
		}
	}
	else throw invalid_argument("Expected creditCards as object type.");

}


#endif /* CUSTOMERS_H_ */
