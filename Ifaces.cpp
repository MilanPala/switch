/*
 * File:   Ifaces.cpp
 * Author: xpalam00
 *
 * Created on 12. únor 2012, 16:17
 */

#include <vector>

#include "Ifaces.hpp"

Ifaces::Ifaces()
{
}

Ifaces::~Ifaces()
{
	this->ifaces.clear();
}

void Ifaces::add(Iface *iface)
{
	this->ifaces.push_back(iface);
}

Iface *Ifaces::next()
{
	if(this->currentIterator == this->ifaces.end()) return NULL;
	Iface *iface = *(this->currentIterator);
	if( this->currentIterator == this->ifaces.end() ) return NULL;
	else
	{
		this->currentIterator++;
		return iface;
	}
}

void Ifaces::reset()
{
	this->currentIterator = this->ifaces.begin();
}

ostream &operator<<(ostream &stream, Ifaces *ifaces)
{
	stream << "Iface\tSent-B\tSent-frm\tRecv-B\tRecv-frm" << endl;
	Iface *iface = NULL;
	ifaces->reset();
	while( (iface=ifaces->next()) != NULL )
	{
		stream << iface->getName() << "\t" << iface->getSentB() << "\t" << iface->getSentFrm() << "\t" << iface->getRecvB() << "\t" << iface->getRecvFrm() << endl;
	}

  return stream;
}