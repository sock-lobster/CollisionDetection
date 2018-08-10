#include "algchecker.h"
#include <boost/lexical_cast.hpp>

int main(int argc, char *argv[])
{
    // Check input
    if (argc != 7){
        std::cout<<"Invalid arguments: .exec dimensions alg winSize numParticles numTimesteps seed"<<std::endl;
        exit(0);
    }

    int dimensions;
    int alg;
    int winSize;
    int numParticles;
    int numTimesteps;
    int seed;

    // convert and save input
    try
        {
        dimensions = boost::lexical_cast<int>(argv[1]);
        alg = boost::lexical_cast<int>(argv[2]);
        winSize = boost::lexical_cast<int>(argv[3]);
        numParticles = boost::lexical_cast<int>(argv[4]);
        numTimesteps = boost::lexical_cast<int>(argv[5]);
        seed = boost::lexical_cast<int>(argv[6]);
        }
        catch( const boost::bad_lexical_cast & )
        {
        std::cout<<"Invalid arguments: all arguments must be of type int"<<std::endl;
        exit(0);
        }

    AlgChecker ac(dimensions,alg,winSize,numParticles,numTimesteps,seed);
    ac.run();

    return 0;
}
