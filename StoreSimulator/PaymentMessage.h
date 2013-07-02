/*
 * PaymentMessage.h
 *
 *  Created on: Jul 1, 2013
 *      Author: arvind
 */

#ifndef PAYMENTMESSAGE_H_
#define PAYMENTMESSAGE_H_

#include <json_spirit.h>
#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace json_spirit;

struct CardInfo {
	string cardNumber;
	string name;
	double creditLimit;
	string expiryDate;

	friend void swap( CardInfo &a, CardInfo &b ) {
		std::swap( a.cardNumber, b.cardNumber );
		std::swap( a.name, b.name );
		std::swap( a.creditLimit, b.creditLimit );
		std::swap( a.expiryDate, b.expiryDate );
	}

	CardInfo& operator=(CardInfo &other) {
		swap( *this, other );
		return *this;
	}

	// Stupid Serialization ( For more extensible messages, use Json or binary messages ).
	friend ostream & operator<<( ostream& os, CardInfo &info ) {
		os<<"{ \"name\": \""<<info.name<<"\", \"cardNumber\": \""<<info.cardNumber<<
				"\", \"creditLimit\": "<<info.creditLimit<<", \"expiryDate\": \""<<info.expiryDate<<"\" }\n";
		return os;
	}

	friend istream & operator>>( istream &is, CardInfo &info ) {
		Value jsonData;
		read( is, jsonData );
		const Object& cardInfoList = jsonData.get_obj();
		for( Object::size_type i=0; i<cardInfoList.size(); i++ ) {
			const Pair& pair = cardInfoList[i];
			const string& name = pair.name_;
			const Value& value = pair.value_;
			if( name=="name" ) {
				info.name = value.get_str();
			}else if( name=="cardNumber" ) {
				info.cardNumber = value.get_str();
			}else if( name=="creditLimit" ) {
				info.creditLimit = value.get_real();
			}else if( name=="expiryDate" ) {
				info.expiryDate = value.get_str();
			}
		}

		return is;
	}
};


struct PaymentInfoMessage {
	int      paymentId;
	double   paymentAmount;
	CardInfo cardInfo;

	friend void swap( PaymentInfoMessage &a, PaymentInfoMessage &b ) {
		std::swap( a.paymentAmount, b.paymentAmount );
		std::swap( a.paymentId, b.paymentId );
		std::swap( a.cardInfo, b.cardInfo );
	}

	PaymentInfoMessage& operator=(PaymentInfoMessage& other) {
		swap( *this, other );
		return *this;
	}

	friend ostream & operator<<( ostream &os, PaymentInfoMessage &pim ) {
		os<<"{ \"paymentId\": "<<pim.paymentId<<", \"paymentAmount\": "<<pim.paymentAmount<<
				", \"name\": \""<<pim.cardInfo.name<<"\", \"cardNumber\": \""<<pim.cardInfo.cardNumber<<
				"\", \"creditLimit\": "<<pim.cardInfo.creditLimit<<", \"expiryDate\": \""<<pim.cardInfo.expiryDate<<"\" }\n";
		return os;
	}

	friend istream & operator>>( istream &is, PaymentInfoMessage &pim ) {
		Value jsonData;
		try {
			read( is, jsonData );
			const Object& cardInfoList = jsonData.get_obj();
			for( Object::size_type i=0; i<cardInfoList.size(); i++ ) {
				const Pair& pair = cardInfoList[i];
				const string& name = pair.name_;
				const Value& value = pair.value_;
				if( name=="name" ) {
					pim.cardInfo.name = value.get_str();
				}else if( name=="cardNumber" ) {
					pim.cardInfo.cardNumber = value.get_str();
				}else if( name=="creditLimit" ) {
					pim.cardInfo.creditLimit = value.get_real();
				}else if( name=="expiryDate" ) {
					pim.cardInfo.expiryDate = value.get_str();
				}else if( name=="paymentId" ) {
					pim.paymentId = value.get_int();
				}else if( name=="paymentAmount" ) {
					pim.paymentAmount = value.get_real();
				}
			}
		}
		catch( runtime_error &e ) {}
		return is;
	}

};

struct PaymentStatus {
	enum PaymentStatusType {
		InsufficientBalance,
		PaymentSuccessful,
		UnknownCard,
		ExpiredCard
	};

	int               paymentId;
	double            paymentAmount;
	int paymentStatus;

	friend void swap( PaymentStatus &a, PaymentStatus &b ) {
		std::swap( a.paymentAmount, b.paymentAmount );
		std::swap( a.paymentId, b.paymentId );
		std::swap( a.paymentStatus, b.paymentStatus );
	}

	// Using the copy and swap idiom to avoid code
	// duplication as well as to provide a strong
	// exception guarantee
	PaymentStatus& operator=( PaymentStatus other ) {
		swap( *this, other );
		return *this;
	}

	friend ostream& operator<<( ostream& os, PaymentStatus &ps ) {
		os<<"{ \"paymentId\": "<<ps.paymentId<<", \"paymentAmount\": "<<ps.paymentAmount<<
				", \"paymentStatus\": "<<ps.paymentStatus<<" }\n";
		return os;
	}

	friend istream& operator>>( istream& is, PaymentStatus& ps ) {
		Value jsonData;
		try {
			read( is, jsonData );
			const Object& paymentStateList = jsonData.get_obj();

			for( Object::size_type i=0; i< paymentStateList.size(); i++ ) {
				const Pair& pair = paymentStateList[i];
				const string& name = pair.name_;
				const Value& value = pair.value_;
				if( name=="paymentId" ) {
					ps.paymentId = value.get_int();
				}else if( name=="paymentAmount" ) {
					ps.paymentAmount = value.get_real();
				}else if( name=="paymentStatus" ) {
					ps.paymentStatus =value.get_int();
				}
			}
		}
		catch( runtime_error &e ) {}

		return is;
	}

};

#endif /* PAYMENTMESSAGE_H_ */
