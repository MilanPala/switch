/*
 * File:   MacAddress.hpp
 * Author: milan
 *
 * Created on 13. únor 2012, 21:08
 */

#ifndef MACADDRESS_HPP
#define	MACADDRESS_HPP

#include <iostream>
#include <sys/types.h>
#include <stdio.h>

#include <libnet.h>

using namespace std;

class MacAddress {
public:
	MacAddress();
	MacAddress(u_char *);
	MacAddress(libnet_ether_addr *);
	MacAddress(MacAddress *);
	virtual ~MacAddress();
	string getAddress() const;
	bool operator==(MacAddress *);
	friend ostream &operator<<(ostream &, const MacAddress *);
	bool isBroadcast;

private:
	u_char rawMac[6];

};
#endif	/* MACADDRESS_HPP */

