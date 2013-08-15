/*
 * File:   Iface.cpp
 * Author: xpalam00
 *
 * Created on 12. únor 2012, 16:16
 */

#include "Iface.hpp"
#include "Switch.hpp"

#define DBG false

pthread_mutex_t ifacemutex = PTHREAD_MUTEX_INITIALIZER;

Iface::Iface(Switch *mainSwitch)
{
	if(DBG) cerr << "Iface::iface()" << endl;

	this->mainSwitch = mainSwitch;

	pthread_create(&(this->_id), NULL, &_entry, this);
}

Iface::~Iface()
{
	int res;
	void *result;

	/* čekání na dokončení a převzetí stavu */
	if( (res = pthread_join(_id, &result)) != 0 )
	{
		if(DBG) cerr << "pthread_attr_init() err " << res << endl;
	}
}

string Iface::getName() const
{
	return this->name;
}

void Iface::setName(string name)
{
	this->name = name;
}

/**
 * Metoda volající
 * @param data
 * @return
 */
void *Iface::_entry(void *data)
{
  Iface *thread = (Iface *)data;
  return thread->main();
}

/**
 * Funkce vykonávající hlavní kód vlákna každého interface.
 * @return void
 */
void *Iface::main()
{
	if(DBG) cerr << "Iface::main(" << this->name << ")" << endl;

	pthread_mutex_lock(&ifacemutex);
	this->sentB = 0;
	this->sentFrm = 0;
	this->recvB = 0;
	this->recvFrm = 0;
	pthread_mutex_unlock(&ifacemutex);

	char errbuf_pcap[PCAP_ERRBUF_SIZE];
	int ret;

	this->pcap_handle = pcap_open_live(this->name.c_str(), BUFSIZ, 1, 1000, errbuf_pcap);
	if (this->pcap_handle == NULL)
	{
		if(DBG) cerr << "Couldn't open device " << this->name << ". (" << errbuf_pcap << ")" << endl;
		return(0);
	}

	ret = pcap_setdirection(this->pcap_handle, PCAP_D_IN);
	if(ret == -1)
	{
		if(DBG) cerr << "Couldn't set direction." << endl;
	}

	ret = pcap_loop(this->pcap_handle, -1, (pcap_handler)_gotPacket, (u_char*)this);
	if( ret != 0 )
	{
		if(DBG) cerr << "An error by process packets." << endl;
	}

	return this;
}

void Iface::_gotPacket(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	//if(DBG) cerr << "Iface::_gotPacket()" << endl;

	if(DBG) cerr << "Incomming payload " << header->len << endl;
	//if(DBG) cerr << packet << endl;

	Iface *self = (Iface *)args;
	self->processIncomingPacket(header, (u_char *)packet);
}

void Iface::processIncomingPacket(const struct pcap_pkthdr *header, u_char *packet)
{
	//if(DBG) cerr << "Iface::processIncomingPacket()" << endl;

	pthread_mutex_lock(&ifacemutex);
	this->recvFrm++;
	this->recvB += header->len;
	pthread_mutex_unlock(&ifacemutex);

	Frame *frame = new Frame(packet);
	frame->setSizeOf(header->len);

	// 1) uložit odesílatele do CAM
	// 2) nalézt adresáta v CAM, nebo odeslat na všechna ostatní rozhraní
	// 2a)nevracet data do stejného segmentu => na jednom Iface je více MacAdresses
	// 3) z odpovědi se naučit nové rozhraní

	if(DBG) cerr << frame->srcMac << "--->>---" << this->getName() << " (" << frame->dstMac << ")" << endl;

	// 1) uložit odesílatele do CAM
	// Pokusí se vložit odesílatele rámce do CAM tabulky
	this->mainSwitch->getCamTable()->add(this, frame->srcMac);

	// 2) nalézt adresáta v CAM, nebo odeslat na všechna ostatní rozhraní
	// Pokusí se vyhledat cílové rozhraní v CAM
	if( frame->dstMac->isBroadcast )
	{
		if(DBG) cerr << "Broadcast" << endl;
		Iface *dstIface;
		this->mainSwitch->getIfaces()->reset();
		for(dstIface = this->mainSwitch->getIfaces()->next(); dstIface != NULL; dstIface=this->mainSwitch->getIfaces()->next())
		{
			if(dstIface == this) continue;
			if(DBG) cerr << "                         " << this->getName() << "\t--->>---" << dstIface->getName() << " (" << frame->dstMac << ")" << endl;
			dstIface->sendFrame(packet, header->len);
		}
	}
	else
	{
		Iface *dstIface;
		dstIface = this->mainSwitch->getCamTable()->find(frame->dstMac);
		if(dstIface == NULL)
		{
			if(DBG) cerr << "Cílové zařízení nenalezeno - posílám všude" << endl;
			Iface *dstIface;
			this->mainSwitch->getIfaces()->reset();
			for(dstIface = this->mainSwitch->getIfaces()->next(); dstIface != NULL; dstIface=this->mainSwitch->getIfaces()->next())
			{
				if(dstIface == this) continue;
				if(DBG) cerr << "                         " << this->getName() << "\t--->>---" << dstIface->getName() << " (" << frame->dstMac << ")" << endl;
				dstIface->sendFrame(packet, header->len);
			}
		}
		else
		{
			if(DBG) cerr << "                         " << this->getName() << "\t--->>---" << dstIface->getName() << " (" << frame->dstMac << ")" << endl;
                        dstIface->sendFrame(packet, header->len);
		}
	}
}

void Iface::sendFrame(u_char *packet, int size)
{
	int ret = pcap_sendpacket(this->pcap_handle, packet, size);
	if(ret == -1)
	{
		//fprintf(stderr, "libnet_write_link() failed: %s\n", l->err_buf);
		if(DBG) cerr << "Packet hasn't been sent: "<< pcap_geterr(this->pcap_handle);
	}
	else
	{
		if(DBG) cerr << " - Sent OK" << endl;
		this->sentFrame(size);
	}
}

void Iface::sendFrame(Frame *frame)
{
	int ret;

	ret = pcap_sendpacket(this->pcap_handle, frame->getPayload(), frame->getSizeOf());
	if(ret == -1)
	{
		//fprintf(stderr, "libnet_write_link() failed: %s\n", l->err_buf);
		if(DBG) cerr << "Packet hasn't been sent: "<< pcap_geterr(this->pcap_handle);
	}
	else
	{
		if(DBG) cerr << " - Sent OK" << endl;
		this->sentFrame(frame->getSizeOf());
	}
}

unsigned int Iface::getSentB()
{
	return this->sentB;
}

unsigned int Iface::getSentFrm()
{
	return this->sentFrm;
}
unsigned int Iface::getRecvB()
{
	return this->recvB;
}
unsigned int Iface::getRecvFrm()
{
	return this->recvFrm;
}

void Iface::sentFrame(unsigned int sizeOfFrame)
{
	this->sentFrm++;
	this->sentB += sizeOfFrame;
}
