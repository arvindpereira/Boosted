/*
 * SharedLocks.h
 *
 *  Created on: Jun 30, 2013
 *      Author: arvind
 */

#ifndef SHAREDLOCKS_H_
#define SHAREDLOCKS_H_

#include <boost/thread.hpp>

extern boost::mutex cout_mutex;
boost::mutex& getCoutMutex() { return ::cout_mutex; }

#endif /* SHAREDLOCKS_H_ */
