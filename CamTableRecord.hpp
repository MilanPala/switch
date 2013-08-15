/*
 * File:   CamTableRecord.hpp
 * Author: xpalam00
 *
 * Created on 11. únor 2012, 22:04
 */

#ifndef CAMTABLERECORD_HPP
#define	CAMTABLERECORD_HPP

#include <time.h>

#include "MacAddress.hpp"
#include "Iface.hpp"

class CamTableRecord {
public:
	CamTableRecord(Iface *, MacAddress *);
	virtual ~CamTableRecord();
	void resetAge();
	friend ostream &operator<<(ostream &, const CamTableRecord *);
	Iface *iface;
	MacAddress *macAddress;
	unsigned int getAge() const;
private:
	unsigned int age;
	time_t lastTime;

};

#endif	/* CAMTABLERECORD_HPP */

