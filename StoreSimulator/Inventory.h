/*
 * Inventory.h
 *
 *  Created on: Jun 30, 2013
 *      Author: arvind
 */

#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>

// Json Parsing
#include <json_spirit.h>

#include "Constants.h"
#include "Item.h"

using namespace std;
using namespace json_spirit;
using namespace boost;

/** Maintains the inventory in the store. Items picked up by the customer should
 * have their quantities updated atomically
 */
class Inventory
{
	map<int,Item> items;   // itemId, items
	map<int,int> quantity; // itemId, quantity
	mutable boost::mutex inventoryLock;
	mutable boost::mutex quantityLock;

public:
	Inventory( string itemFileName )
	{
		ifstream fin( itemFileName.c_str() );
		fin>>(*this);
	}

	boost::mutex &inventoryMutex() {
		return inventoryLock;
	}

	boost::mutex &quantityMutex() {
		return quantityLock;
	}

	map<int,Item> getAllItemInfo( ) {
		boost::lock_guard<boost::mutex> guard( inventoryMutex() );
		return items;
	}

	int removeAnItem( int itemId, int numberItems ) {
		boost::lock_guard<boost::mutex> guard( quantityMutex() );
		// Can remove an item only if the quantity
		int numberOfItemsRemoved = 0;
		if( quantity.find( itemId )!= quantity.end() ) {
			if( quantity[ itemId ]>numberItems ) {
				quantity[ itemId ] -= numberItems;
				numberOfItemsRemoved = numberItems;
			}
			else {
				numberOfItemsRemoved = quantity[ itemId ];
				quantity[ itemId ] = 0; // We're empty.
			}
		}
		return numberOfItemsRemoved;
	}

	int getQuantity( int itemId ) {
		boost::lock_guard<boost::mutex> guard( quantityMutex() );
		if ( quantity.find( itemId )!= quantity.end() ) {
				return quantity[ itemId ];
		}
		else throw invalid_argument("Item not found during quantity check.");
	}

	double getPrice( int itemId ) {
		boost::lock_guard<boost::mutex>( inventoryMutex() );
		if( items.find( itemId) != items.end() ) {
			return items[itemId].getPrice();
		}
		else { // Is thread safe because we use a scoped lock guard.
			throw invalid_argument("Item not found during price check.");
		}
	}

	double setPrice( int itemId, double price ) {
		boost::lock_guard<boost::mutex>( inventoryMutex() );
		if( items.find( itemId) != items.end() ) {
			items[itemId].setPrice( price );
		}
		else { // Is thread safe because we use a scoped lock guard.
			throw invalid_argument("Item not found during price set.");
		}
	}

	void lockInventory() {
		this->inventoryLock.lock();
	}

	void unlockInventory() {
		this->inventoryLock.unlock();
	}

	void lockQuantity() {
		this->quantityLock.lock();
	}

	void unlockQuantity() {
		this->quantityLock.unlock();
	}

	friend istream& operator >>( istream &is, Inventory &inv );
	friend ostream& operator <<( ostream &os, Inventory &inv );
	friend ostream& toJson( ostream &os, Inventory &inv );
};

ostream& toJson( ostream &os, Inventory &inv )
{
	boost::lock_guard<boost::mutex> guard( inv.inventoryMutex() );
	Value jsonData;
	// To be implemented!!!
}

ostream& operator <<( ostream &os, Inventory &inv )
{
	boost::lock_guard<boost::mutex> guard( inv.inventoryMutex() );
	os<<"\nItems are:\n";
	for( map<int,Item>::iterator it = inv.items.begin(); it!=inv.items.end(); it++ ) {
		os<<"itemId:"<<it->first<<", name: "<<it->second.getName()<<", price: "<<
				it->second.getPrice()<<", categoryId: "<<it->second.getCategoryId()<<
				inv.getQuantity(it->second.getItemId())<<'\n';
	}
	return os;
}


istream& operator >>( istream &is, Inventory &inv ) {
	boost::lock_guard<boost::mutex> guard1( inv.inventoryMutex() );
	boost::lock_guard<boost::mutex> guard2( inv.quantityMutex() );

	Value jsonData;
	try {
		read ( is, jsonData );
	}
	catch ( json_spirit::Error_position &ep ) {
		cerr<<"Error on "<<ep.line_<<", at Column "<<ep.column_<<", :"<<ep.reason_<<endl;
		throw;
	}
	const Object& itemList = jsonData.get_obj();
	const Pair& pair = itemList[0];
	const string& name = pair.name_;
	const Value& items = pair.value_;

	if ( name=="items" ) {
		const Array& itemArray = items.get_array();

		Item newItem; int qty = 0;
		for( unsigned i=0; i<itemArray.size(); i++ ) {
			const Object& itemObj = itemArray[i].get_obj();
			for( Object::size_type j=0; j!=itemObj.size(); j++ ) {
				const Pair& itemAttr = itemObj[j];
				if( itemAttr.name_ == "itemId" ) {
					newItem.setItemId( itemAttr.value_.get_int() );
					cout<<"itemId read:"<<newItem.getItemId()<<'\n';
				}
				else if( itemAttr.name_ == "name" ) {
					newItem.setName( itemAttr.value_.get_str() );
				}
				else if( itemAttr.name_ == "categoryId" ) {
					newItem.setCategoryId( itemAttr.value_.get_int() );
				}
				else if( itemAttr.name_ == "price" ) {
					newItem.setPrice( itemAttr.value_.get_real() );
				}
				else if( itemAttr.name_ == "quantity") {
					qty = itemAttr.value_.get_int();
				}
			}
			if( newItem.getItemId() != StoreSim::UnknownItemId ) {
				inv.quantity[ newItem.getItemId() ] = qty;
				inv.items[ newItem.getItemId() ] = newItem;
			}
			else throw invalid_argument("Item without ItemId cannot be added to inventory.");
		}
		// By now, we should have received all the information about
		// this item.

	}
	return is;
}

#endif /* INVENTORY_H_ */
