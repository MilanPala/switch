Projekt switch do předmětu PDS
==============================

Autor: Milan Pála, xpalam00
Školní rok: 2011/2012

Překlad
> make

Spuštění
> ./switch

Switch je implementován v jazyce C++. Po spuštění dojde k otevření všech ethernetových
portů na daném počítači a pro každý se vytvoří jedno vlastní vklákno, které obstrarává příjem,
zpracování a posílání paketů. Pro práci se síťovou vrstvou je využita knihovna libpcap,
pro práci s vlákny knihovna pthread.

Omezení
-------
Implementace neumí přepínat multicastové rámce, pouze unicastové a broadcastové.

