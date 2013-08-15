/*
 * File:   CamTableRecord.cpp
 * Author: xpalam00
 *
 * Created on 11. únor 2012, 22:04
 */

#include "CamTableRecord.hpp"

CamTableRecord::CamTableRecord(Iface *iface, MacAddress *mac)
{
	this->iface = iface;
	this->macAddress = new MacAddress(mac);
	this->resetAge();
}

CamTableRecord::~CamTableRecord() {
}

void CamTableRecord::resetAge()
{
	this->lastTime = time(NULL);
}

unsigned int CamTableRecord::getAge() const
{
    time_t currentTime = time(NULL);
    return (unsigned int)currentTime-this->lastTime;
}

ostream &operator<<(ostream &stream, const CamTableRecord *ctr)
{
	stream << ctr->macAddress << "\t" << ctr->iface->getName() << "\t" << ctr->getAge();

	return stream;
}
