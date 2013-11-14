

FIND_PROGRAM( ROOT_genmap_CMD genmap PATHS $ENV{ROOTSYS}/bin $ENV{PATH} )

MESSAGE( STATUS " *** MakeROOTMap.cmake run command : ${ROOT_genmap_CMD} -debug -i ${libname} -o ${rootmapfile}
                 WORKING_DIRECTORY ${genmap_install_dir} "
)

if(APPLE)
  #MESSAGE( STATUS " set DYLD_LIBRARY_PATH to  ./:$ENV{DYLD_LIBRARY_PATH} " )
  SET ( ENV{DYLD_LIBRARY_PATH} ./:$ENV{DYLD_LIBRARY_PATH}:$ENV{ROOTSYS}/lib:${genmap_install_dir} )
else()
  SET ( ENV{LD_LIBRARY_PATH} ./:$ENV{LD_LIBRARY_PATH}:$ENV{ROOTSYS}/lib:${genmap_install_dir} )
endif()

EXECUTE_PROCESS( COMMAND 
		 ${ROOT_genmap_CMD} -debug -i ${libname} -o ${rootmapfile}
                 WORKING_DIRECTORY ${genmap_install_dir}
)