
//Solution of task 1b:

#include <lama.hpp>

#include <lama/storage/SparseAssemblyStorage.hpp>
#include <lama/matrix/CSRSparseMatrix.hpp>
#include <lama/DenseVector.hpp>

using namespace lama;

int main()
{
    IndexType size = 4;

    SparseAssemblyStorage<double> sas( size, size, 10 );

    for ( IndexType i = 0; i < size; i++ )
    {
        sas.set( i, i, 2 );
    }

    for ( IndexType i = 0; i < size - 1; i++ )
    {
        sas.set( i + 1, i, 1 );
    }

    for ( IndexType i = 0; i < size - 1; i++ )
    {
        sas.set( i, i + 1, 1 );
    }

    CSRSparseMatrix<double> m ( sas );

    DenseVector<double> rhs( size , 0.0 );
    HostWriteAccess<double> hwarhs( rhs.getLocalValues() );	
    for ( IndexType i = 0; i < size; i++ )
    {
    	hwarhs[i] = i + 1.0;
    }

    hwarhs.release();

    DenseVector<double> solution( size , 0.0 );

    return 0;
}
