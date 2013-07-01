/*
 * Store.h
 *
 *  Created on: Jun 30, 2013
 *      Author: arvind
 */

#ifndef STORE_H_
#define STORE_H_

#include <iostream>
#include <vector>

#include <boost/thread.hpp>


using namespace boost::this_thread;

class Store
{
	int currentCustomerCount;
	boost::mutex _mut;






};


#endif /* STORE_H_ */
