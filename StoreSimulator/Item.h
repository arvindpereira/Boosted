/*
 * Item.h
 *
 *  Created on: Jun 29, 2013
 *      Author: arvind
 */

#ifndef ITEM_H_
#define ITEM_H_

// Boost libraries
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/thread/condition_variable.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>
// Json Parsing
#include <json_spirit.h>
// Other libraries
#include <iostream>
#include <string>
#include <utility>

using namespace boost::gregorian;
using namespace boost;
using namespace std;

/** An item in the store
 * */
class Item
{
	int  	itemId;
	double 	price;
	int		categoryId;
	//date	expiryDate;
	string  name;
public:
	Item() : itemId(StoreSim::UnknownItemId) {}

	Item( int _id, double _price, int _category, date _expiration ) :
			itemId( _id ), price(_price), categoryId(_category)
			//,expiryDate(_expiration)
	{}

	friend void swap( Item &a, Item &b ) {
		std::swap( a, b );
	}

	Item( const Item& item )
	{
		this->itemId = item.itemId;
		this->price = item.price;
		this->categoryId = item.categoryId;
		this->name = item.name;
	}

	~Item() {}

    int getCategoryId() const;
    //date getExpiryDate() const;
    int getItemId() const;
    double getPrice() const;
    void setCategoryId(int categoryId);
    //void setExpiryDate(date expiryDate);
    void setItemId(int itemId);
    void setPrice(double price);
    string getName() const;
    void setName( string name );

    friend ostream &operator<<( ostream & os, Item &item )  {
    	os<<"itemId:"<<item.getItemId()<<", name: "<<item.getName()<<", price: "<<
    					item.getPrice()<<", categoryId: "<<item.getCategoryId()<<'\n';
    	return os;
    }

};

int Item::getCategoryId() const
{
    return categoryId;
}

//date Item::getExpiryDate() const
//{
//    return expiryDate;
//}

int Item::getItemId() const
{
    return itemId;
}

double Item::getPrice() const
{
    return price;
}

void Item::setCategoryId(int categoryId)
{
    this->categoryId = categoryId;
}

//void Item::setExpiryDate(date expiryDate)
//{
//    this->expiryDate = expiryDate;
//}

void Item::setItemId(int itemId)
{
    this->itemId = itemId;
}

void Item::setPrice(double price)
{
    this->price = price;
}

void Item::setName( string name_ )
{
	this->name = name_;
}

string Item::getName( ) const
{
	return this->name;
}


#endif /* ITEM_H_ */
