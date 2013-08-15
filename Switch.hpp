/*
 * File:   Switch.hpp
 * Author: milan
 *
 * Created on 12. únor 2012, 12:41
 */

#ifndef SWITCH_HPP
#define	SWITCH_HPP

#include <cstdlib>
#include <pcap/pcap.h>
#include <string>
#include <iostream>
#include <pthread.h>

#include "CamTable.hpp"
#include "Ifaces.hpp"
#include "Iface.hpp"

using namespace std;

class Switch {
public:
	Switch();
	virtual ~Switch();

	CamTable * getCamTable();
	Ifaces * getIfaces();

	static void* _entry(void*);
	void* main();

private:
	CamTable *camTable;
	Ifaces *ifaces;
	void initDevices();
	void userPromt();

	pthread_t _id;
};

#endif	/* SWITCH_HPP */
