# Build instructions for generating the boost program_options library
# and installing it in the cwd

BOOSTDIR = boost_1_43_0
BJAM     = ${BOOSTDIR}/bjam

all: ${BOOSTDIR} ${BJAM} lib/libboost_program_options.a

${BOOSTDIR}:
	tar -zxf boost_1_43_0.tgz

${BJAM}:
	cd ${BOOSTDIR}; ./bootstrap.sh --prefix=../ --with-libraries=program_options

lib/libboost_program_options.a:  
	cd ${BOOSTDIR}; ./bjam install
