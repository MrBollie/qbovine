#include "mdnslookup.h"
#include <QDebug>
#include <QtNetwork>
#include <QtWidgets>

MDNSLookup::MDNSLookup()
{
    avahiServiceBrowser = nullptr;
    avahiClient = nullptr;
}

MDNSLookup::~MDNSLookup()
{
    if (avahiServiceBrowser)
        avahi_service_browser_free(avahiServiceBrowser);
    if (avahiClient)
        avahi_client_free(avahiClient);
}

void MDNSLookup::browseCallback( AvahiServiceBrowser *b, AvahiIfIndex interface,
    AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, const char *type,
    const char *domain, AVAHI_GCC_UNUSED AvahiLookupResultFlags flags, void* userdata) {
    MDNSLookup *ml = (MDNSLookup*)userdata;
    switch (event) {
        case AVAHI_BROWSER_NEW:
            AvahiLookupFlags flags = AvahiLookupFlags();
            avahi_service_resolver_new(ml->avahiClient, interface, protocol,
                name, type, domain, protocol, flags,
                &MDNSLookup::resolveCallback, ml
            );
    }
}

void MDNSLookup::clientCallback(AvahiClient *c, AvahiClientState state, void *userdata) {
    if (state == AVAHI_CLIENT_FAILURE) {
        qDebug() << "avahi client failed.";
    }

}
void MDNSLookup::resolveCallback(AvahiServiceResolver *r,
    AVAHI_GCC_UNUSED AvahiIfIndex interface,
    AVAHI_GCC_UNUSED AvahiProtocol protocol, AvahiResolverEvent event,
    const char *name, const char *type, const char *domain,
    const char *host_name, const AvahiAddress *address,
    uint16_t port, AvahiStringList *txt, AvahiLookupResultFlags flags,
    AVAHI_GCC_UNUSED void* userdata)
{
    MDNSLookup *pml = (MDNSLookup*)userdata;
    qDebug() << "Found Host " << host_name << " with port " << port;
    char ip[200];
    avahi_address_snprint(ip, 200, address);
    qDebug() << "IP: " << ip;
    emit pml->deviceFound(ip, port);
}

/**
 * @brief MDNSLookup::lookup
 * @param service
 */
void MDNSLookup::lookup(const QString service) {
    avahiPoll = avahi_qt_poll_get();
    int error;
    AvahiClientFlags flags = AvahiClientFlags();
    avahiClient = avahi_client_new(avahiPoll, flags, &this->clientCallback, this, &error);
    if (avahiClient) {
        AvahiLookupFlags lflags = AvahiLookupFlags();
        avahiServiceBrowser = avahi_service_browser_new(avahiClient,
            AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC,
            service.toUtf8(), "local", lflags,
            &this->browseCallback, this
        );
    }
}
