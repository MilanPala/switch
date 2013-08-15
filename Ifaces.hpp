/*
 * File:   Ifaces.hpp
 * Author: milan
 *
 * Created on 12. únor 2012, 16:17
 */

#ifndef IFACES_HPP
#define	IFACES_HPP

#include <iostream>
#include <vector>

#include "Iface.hpp"

using namespace std;

class Ifaces {
public:
	Ifaces();
	virtual ~Ifaces();

	void add(Iface *);
	Iface *next();
	void reset();
private:
	vector<Iface *> ifaces;
	vector<Iface *>::iterator currentIterator;

};

ostream &operator<<(ostream &, Ifaces *);

#endif	/* IFACES_HPP */

