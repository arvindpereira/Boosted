/*
 * SimulateStore.cpp
 *
 *  Created on: Jun 30, 2013
 *      Author: arvind
 */

#include <iostream>
#include <vector>
#include "Inventory.h"
#include "Category.h"
#include "Cashier.h"
#include "Customers.h"
#include "Customer.h"
#include "Store.h"
#include "TimeTools.h"

using namespace std;
using namespace ArvindsTools;

boost::mutex cout_mutex;


int main()
{
	TimeTools myTimer;
	cout<<"This is the store simulator.\n";

	Inventory storeInv("ItemList.json");
	Customers custInfo("CustomerAccounts.json");

	cout<<custInfo;
	cout<<storeInv;

	map<int,Customer> myCustomers = custInfo.getCustomers();

	cout<<"\n\n\n\nStore Opened. Waiting for Customers.\n";
	vector<Customer> customers;
	int numCustomers = 20000;
	for(int i=1;i<=numCustomers;i++) {
		Customer c(i, myCustomers[i].getCardInfo(), &storeInv);
		customers.push_back(c);
	}

	for( int i=0; i<customers.size();i++ ) {
		customers[i].start();
	}

	for( int i=0;i<customers.size();i++ ) {
		customers[i].join();
	}


	cout<<"Main thread waiting for customer to finish."<<endl;
	cout<<"Closing for the day"<<endl;
	cout<<storeInv;
	cout<<"Done in : "<<myTimer.timeSinceStart()<<" seconds."<<endl;

	return 0;
}
