include ../make.inc

INCLUDE_FLAGS = -I${BOOST_INCLUDE_DIR} -I${SCAI_ROOT}/include

LD_FLAGS = ${SCAI_LD_FLAGS} -Wl,-rpath=${SCAI_ROOT}/lib 

EXECUTABLE: matrix_generator.exe cg_solver.exe jacobi_solver.exe amg_solver.exe gmres_solver.exe

cg_solver.exe: LamaConfig.hpp
jacobi_solver.exe: LamaConfig.hpp
amg_solver.exe: LamaConfig.hpp

.SUFFIXES:

.SUFFIXES: .cpp .o
.SUFFIXES: .o .exe

.o.exe: 
	${CXX} -o $*.exe $*.o ${LD_FLAGS} 

.cpp.o:
	${CXX} ${CXX_FLAGS} ${INCLUDE_FLAGS} -c $*.cpp

clean:
	rm -rf *.exe *.o *.frm *.frv *.amg *.vec *.trace
