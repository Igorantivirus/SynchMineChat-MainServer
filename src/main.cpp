#ifndef _DEBUG

#define _DEBUG

#endif

#include "Subs/ClientBrocker.hpp"

#include "Subs/Subscribers/TgBotSubscriber.hpp"
#include "Subs/Subscribers/CrowSubscriber.hpp"

int main()
{
    Service::staticConstruct("resources/ServerConfig.json");

    ClientBrocker brocker;
    TgBotSubscriber tg{brocker};
    CrowSubscriber crow{brocker};

    crow.asynchRun();
    tg.runNoAsynch();

    return 0;
}