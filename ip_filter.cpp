#include <iostream>
#include <string>
#include <vector>

template<typename TFunctor>
auto split(const std::string &str, char d, TFunctor functor) -> std::vector<std::invoke_result_t<TFunctor, std::string>>
{
    using RetType = std::vector<std::invoke_result_t<TFunctor, std::string>>;
    RetType r;

    auto stop = str.find_first_of(d);
    decltype(stop) start = 0;
    while(stop != std::string::npos)
    {
        r.emplace_back(functor(str.substr(start, stop - start)));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.emplace_back(functor(str.substr(start)));

    return r;
}

template<typename T, typename TPredicate>
std::vector<T> filter(const std::vector<T>& input, TPredicate functor) {
    std::vector<T> ret;
    std::copy_if(input.cbegin(), input.cend(), std::back_inserter(ret), functor);
    return ret;
}

template<typename IntegralOutput = int, typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<std::vector<T>>& vec) {
    for(const auto& ip: vec) {   
        size_t counter{};
        for (const auto& ip_part: ip) {
            if constexpr (std::is_integral_v<T>) {
                out << static_cast<IntegralOutput>(ip_part);
            } else {
                out << ip_part;
            }
            if (++counter < ip.size())
                out << '.';
        }
        out << std::endl;
    }
    return out;
}

int main()
{
    try
    {
        std::vector<std::vector<unsigned char>> ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            auto v = split(line, '\t', [](const std::string& str) { return str; });
            ip_pool.emplace_back(std::move(split(v.at(0), '.', [](const std::string& str) { 
                return static_cast<unsigned char>(std::stoi(str)); 
            })));
        }

        std::sort(ip_pool.begin(), ip_pool.end(), std::greater<>());

        std::cout << ip_pool;

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)
        std::cout << filter(ip_pool, [](const std::vector<unsigned char>& ip) {
            return ip.at(0) == 1;
        });

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        std::cout << filter(ip_pool, [](const std::vector<unsigned char>& ip) {
            return ip.at(0) == 46 && ip.at(1) == 70;
        });

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)

        std::cout << filter(ip_pool, [](const std::vector<unsigned char>& ip) {
            return ip.at(0) == 46 || ip.at(1) == 46 || ip.at(2) == 46 || ip.at(3) == 46;
        });

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
