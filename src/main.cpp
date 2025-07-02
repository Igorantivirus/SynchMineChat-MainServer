#include <iostream>
#include <string>

#include "Services/Service.hpp"

#include "Subs/ClientBrocker.hpp"

int main()
{
    Service::staticConstruct("resources\\ServerConfig.json");

    return 0;
}