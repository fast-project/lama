
//Solution of task 1b:

#include <lama.hpp>

#include <lama/storage/SparseAssemblyStorage.hpp>
#include <lama/matrix/CSRSparseMatrix.hpp>
#include <lama/DenseVector.hpp>

using namespace lama;

int main( int argc, char* argv[] )
{
    if ( argc < 2 )
    {
        std::cerr << "No input file specified" << std::endl;
        exit( -1 );
    }

    //Read a sparse matrix from the passed input file
    CSRSparseMatrix<double> m( argv[1] );

    IndexType size = m.getNumRows();

    DenseVector<double> rhs( size , 0.0 );
    HostWriteAccess<double> hwarhs( rhs.getLocalValues() );	
    for ( IndexType i = 0; i < size; ++i )
    { 
        hwarhs[i] = i + 1.0;
    }

    hwarhs.release();

    DenseVector<double> solution( size , 0.0 );

    return 0;
}
