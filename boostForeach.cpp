#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <boost/foreach.hpp>
#include <sys/time.h>
using namespace std;

namespace
{
    // millisecond = 1000 * microsecond(usec)
    unsigned int GetCurrentTime()
    {
        struct timeval now;
        gettimeofday(&now, NULL);
        unsigned int now_in_ms = now.tv_sec;
        now_in_ms += now_in_ms * 1000 + now.tv_usec / 1000;
        return now_in_ms;
    }

    class BenckmarkObject
    {
        const string m_output_prefix;
        unsigned int m_begin_time;

        public:
        explicit BenckmarkObject(const string& output_prefix = "")
            : m_output_prefix(output_prefix)
            {
                m_begin_time = GetCurrentTime();
            }

        ~BenckmarkObject()
        {
            const unsigned int end_time = GetCurrentTime();
            cout << m_output_prefix << " total runtime(:millisecond): " << end_time - m_begin_time << endl;
        }
    };
}

int add_one(int & a)
{
    a += 1;
}

int main()
{
        const int N = 10000000; // ten million
        vector<int> data(N, -1);
        data.reserve(N);

        {
            BenckmarkObject bench_obj("index:");
            for(int i = 0; i < N; i++)
            {
                    data[i] += 1;
            }
            assert(0 == data[N-1]);
        }


        {
            BenckmarkObject bench_obj("iterator:");
            for(vector<int>::iterator it = data.begin(); it != data.end(); ++it)
            {
                    *it = *it + 1;
            }
            assert(1 == data[N-1]);
        }

        {

            BenckmarkObject bench_obj("BOOST_FOREACH:");
            BOOST_FOREACH(int& val, data)
            {
                    val += 1;
            }
            assert(2 == data[N-1]);
        }

        {

            BenckmarkObject bench_obj("std::for_each:");
            std::for_each(data.begin(), data.end(), add_one);
            assert(3 == data[N-1]);
        }

        return 0;

}


