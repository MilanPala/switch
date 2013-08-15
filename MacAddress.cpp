/*
 * File:   MacAddress.cpp
 * Author: xpalam00
 *
 * Created on 13. únor 2012, 21:08
 */

#include "MacAddress.hpp"

MacAddress::MacAddress()
{
	this->isBroadcast = false;
}

MacAddress::MacAddress(u_char *address)
{
	for(int i=0; i<6; i++) this->rawMac[i] = address[i];

	u_char broadcast[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	if( memcmp(this->rawMac, broadcast, sizeof(this->rawMac)) == 0 ) this->isBroadcast = true; else this->isBroadcast = false;
}

MacAddress::MacAddress(libnet_ether_addr *address)
{
	for(int i=0; i<6; i++) this->rawMac[i] = address->ether_addr_octet[i];

	u_char broadcast[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	if( memcmp(this->rawMac, broadcast, sizeof(this->rawMac)) == 0 ) this->isBroadcast = true; else this->isBroadcast = false;
}

MacAddress::MacAddress(MacAddress *mac)
{
	memcpy(this->rawMac, mac->rawMac, sizeof(mac->rawMac));
	this->isBroadcast = mac->isBroadcast;
}

MacAddress::~MacAddress() {
}

string MacAddress::getAddress() const
{
	string address;
	char _address[18];
	sprintf(_address, "%02x%02x.%02x%02x.%02x%02x", this->rawMac[0], this->rawMac[1], this->rawMac[2], this->rawMac[3], this->rawMac[4], this->rawMac[5]);
	address.append(_address);
	return address;
}

bool MacAddress::operator==(MacAddress *second)
{
	return this->getAddress() == second->getAddress();
}

ostream &operator<<(ostream &stream, const MacAddress *address)
{
  stream << address->getAddress();

  return stream;
}

