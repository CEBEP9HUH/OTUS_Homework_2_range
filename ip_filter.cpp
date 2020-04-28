#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include <range/v3/action/sort.hpp>
#include <range/v3/action/reverse.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/range/conversion.hpp>

std::vector<std::string> split(const std::string &str, const char d)
{
    std::vector<std::string> r;

    decltype(auto) stop = str.find_first_of(d);
    decltype(stop) start = 0;
    while(stop != std::string::npos)
    {
        r.emplace_back(str.substr(start, stop - start));
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.emplace_back(str.substr(start));

    return r;
}

std::vector<unsigned char> convert_to_uchars(const std::vector<std::string>& ip)
{
    std::vector<unsigned char> r;
    for(const auto& octet: ip)
    {
        if(octet.find_first_not_of("0123456789", 0) != std::string::npos)
        {
            break;
        }
        int int_octet = std::stoi(octet);
        if(0 <= int_octet && int_octet <= __UINT8_MAX__)
        {
            r.push_back(int_octet);
        }
    }
    if(r.size() != ip.size() || r.size() != 4)
    {
        throw(std::invalid_argument("Ip error"));
    }
    return r;
}

void print_ip(const std::vector<std::vector<unsigned char> >& ip_pool)
{
    for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
    {
        for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
        {
            if (ip_part != ip->cbegin())
            {
                std::cout << ".";

            }
            std::cout << +*ip_part;
        }
        std::cout << std::endl;
    }
}

int main()
{
    try
    {
        std::vector<std::vector<unsigned char> > ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            try{
                ip_pool.emplace_back(convert_to_uchars(split(v.at(0), '.')));
            }
            catch(const std::exception &e)
            {
                std::cout << e.what() << "\n";
            }
        }

        ip_pool |= ranges::actions::sort | ranges::actions::reverse;

        decltype(ip_pool) ip_1 = ip_pool | ranges::view::filter([](const auto& ip){return ip[0]==1;}) | ranges::to_vector;
        decltype(ip_pool) ip_46_70 = ip_pool | ranges::view::filter([](const auto& ip){return ip[0]==46 && ip[1]==70;}) | ranges::to_vector;
        decltype(ip_pool) ip_any_46 = ip_pool | ranges::view::filter([](const auto& ip){return ip[0]==46 || ip[1]==46 || ip[2]==46 || ip[3]==46;}) | ranges::to_vector;

        print_ip(ip_pool);
        print_ip(ip_1);
        print_ip(ip_46_70);
        print_ip(ip_any_46);
        
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
