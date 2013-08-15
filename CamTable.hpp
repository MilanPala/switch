/*
 * File:   CamTable.h
 * Author: xpalam00
 *
 * Created on 11. únor 2012, 21:15
 */

#ifndef CAMTABLE_H
#define	CAMTABLE_H

#include <iostream>
#include <map>

#include "Iface.hpp"
#include "CamTableRecord.hpp"

using namespace std;

class CamTable {
public:
	CamTable();
	virtual ~CamTable();

	/**
	 * Přidá do tabulky zařízení, které se je připojeno k danému interface.
	 * @param Iface Síťová karta switche.
	 * @param MacAddress Adresa zařízení, které je k interface připojeno.
	 */
	void add(Iface *, MacAddress *);

	Iface *find(MacAddress *);

	/**
	 * Exportuje tabulku v textovém formátu.
	 * @param ostream Cílový output stream
	 * @param CamTable CAM tabulka
	 * @return ostream
	 */
	friend ostream &operator<<(ostream &, const CamTable *);

private:
	map<string, CamTableRecord*> records;
	static const unsigned int MAX_AGE = 30;
};

#endif	/* CAMTABLE_H */

