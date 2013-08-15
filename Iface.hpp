/*
 * File:   Iface.hpp
 * Author: xpalam00
 *
 * Created on 12. únor 2012, 16:16
 */

#ifndef IFACE_HPP
#define	IFACE_HPP

#include <iostream>
#include <string>
#include <pthread.h>
#include <cstdlib>
#include <pcap/pcap.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Frame.hpp"

using namespace std;

class Switch;

class Iface
{
public:
	Iface(Switch *);
	virtual ~Iface();

	string getName() const;
	void setName(string);

	static void* _entry(void*);
	void* main();
	static void _gotPacket(u_char *, const struct pcap_pkthdr *, const u_char *);

	unsigned int getSentB();
	unsigned int getSentFrm();
	unsigned int getRecvB();
	unsigned int getRecvFrm();

	MacAddress *macAddress;

	void sendFrame(Frame *);
	void sendFrame(u_char *packet, int size);
	void sentFrame(unsigned int);

	pcap_t *pcap_handle;

private:
	string name; /** <<< Jméno zařízení vhodné pro pcap_open_live() */

	void processIncomingPacket(const struct pcap_pkthdr *, u_char *);

	unsigned int sentB;
	unsigned int sentFrm;
	unsigned int recvB;
	unsigned int recvFrm;

	Switch *mainSwitch;

	pthread_t _id;
};

#endif	/* IFACE_HPP */
