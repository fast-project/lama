/**
 * @file BenchContext.cpp
 *
 * @brief Benchmarking of memory transfers HOST <-> context
 * @author: Thomas Brandes
 * @date 14.09.2015
 **/

#include <scai/hmemo/Context.hpp>
#include <scai/hmemo/LAMAArray.hpp>
#include <scai/hmemo/ReadAccess.hpp>
#include <scai/hmemo/WriteOnlyAccess.hpp>
#include <scai/common/Walltime.hpp>

#include <iostream>
#include <scai/logging.hpp>

using namespace std;
using namespace scai::hmemo;

void bench( ContextPtr host, ContextPtr device )
{
    static int ITER_VEC[] = { 1000, 1000, 1000, 300, 100, 50, 20 };
    static int N_VEC[]    = { 1, 10, 100, 1000, 10000, 100000, 1000000 };

    int NCASES = sizeof( ITER_VEC ) / sizeof( int );

    for ( int k = 0; k < NCASES; ++k )
    {
        int ITER = ITER_VEC[k];
        int N    = N_VEC[k];

        cout << "Case " << k << ": N = " << N << ", ITER = " << ITER << endl;

        double time = scai::common::Walltime::get();

        // measure time for allocation

        for ( int i = 0; i < ITER; ++i )
        {
            LAMAArray<double> array;

            // write access always allocates the data on device

            {
                WriteOnlyAccess<double> devWrite( array, device, N );
            }
        }

        double t0 = ( scai::common::Walltime::get() - time ) * 1000.0;

        time = scai::common::Walltime::get();

        LAMAArray<double> array;

        for ( int i = 0; i < ITER; ++i )
        {
            // write only access invalidates all data

            {
                WriteOnlyAccess<double> hostWrite( array, host, N );
            }
            {
                ReadAccess<double> devRead( array, device );
            }
        }

        double t1 = ( scai::common::Walltime::get() - time ) * 1000.0;

        time = scai::common::Walltime::get();

        for ( int i = 0; i < ITER; ++i )
        {
            {
                WriteOnlyAccess<double> devWrite( array, device, N );
            }
            {
                ReadAccess<double> hostRead( array, host );
            }
        }

        double t2 = ( scai::common::Walltime::get() - time ) * 1000.0;

        double Bytes = static_cast<double>( N ) * sizeof( double ) * ITER;
        double MBytes0 = Bytes / ( 1024.0 * 1024.0 * t0 ) * 1000;
        double MBytes1 = Bytes / ( 1024.0 * 1024.0 * t1 ) * 1000;
        double MBytes2 = Bytes / ( 1024.0 * 1024.0 * t2 ) * 1000;

        cout << "Case " << k << ": N = " << N << ", ITER = " << ITER << ", Bytes = " << Bytes << endl;

        cout << "Alloc Dev: " << t0 << " ms, is " << MBytes0 << " MByte/s" << endl;
        cout << "Host->Dev: " << t1 << " ms, is " << MBytes1 << " MByte/s" << endl;
        cout << "Dev->Host: " << t2 << " ms, is " << MBytes2 << " MByte/s" << endl;
        cout << endl;
    }
}

int main()
{
    ContextPtr host = Context::getContextPtr( context::Host );

    if ( Context::canCreate( context::CUDA ) )
    {
        ContextPtr cuda = Context::getContextPtr( context :: CUDA );
        std::cout << "CUDA bench test, context = " << *cuda << std::endl;
        bench( host, cuda );
    }
    else
    {
        std::cout << "No CUDA bench test, CUDA context not available" << std::endl;
    }

    if ( Context::canCreate( context::MIC ) )
    {
        ContextPtr mic = Context::getContextPtr( context :: MIC );
        std::cout << "MIC bench test, context = " << *mic << std::endl;
        bench( host, mic );
    }
    else
    {
        std::cout << "No CUDA bench test, CUDA context not available" << std::endl;
    }
}
