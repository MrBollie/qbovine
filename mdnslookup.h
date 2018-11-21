#ifndef MDNSLOOKUP_H
#define MDNSLOOKUP_H

#include <QObject>
#include <avahi-client/client.h>
#include <avahi-client/lookup.h>
#include <avahi-qt5/qt-watch.h>

class MDNSLookup : public QObject {
    Q_OBJECT

public:
    MDNSLookup();
    ~MDNSLookup();

    void lookup(const QString service);
    static void clientCallback(AvahiClient *c, AvahiClientState, void *userdata);
    static void browseCallback( AvahiServiceBrowser *b, AvahiIfIndex interface,
                         AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, const char *type,
                         const char *domain, AVAHI_GCC_UNUSED AvahiLookupResultFlags flags, void* userdata);
    static void resolveCallback(AvahiServiceResolver *r,
        AVAHI_GCC_UNUSED AvahiIfIndex interface,
        AVAHI_GCC_UNUSED AvahiProtocol protocol, AvahiResolverEvent event,
        const char *name, const char *type, const char *domain,
        const char *host_name, const AvahiAddress *address,
        uint16_t port, AvahiStringList *txt, AvahiLookupResultFlags flags,
        AVAHI_GCC_UNUSED void* userdata);
private:
    AvahiClient *avahiClient;
    AvahiServiceBrowser *avahiServiceBrowser;
    const AvahiPoll* avahiPoll;

signals:
    void deviceFound(QString ip, uint16_t port);

};


#endif // MDNSLOOKUP_H
