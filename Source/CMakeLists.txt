get_filename_component(BUILD_PARENT_DIR ${CMAKE_BINARY_DIR} PATH)
set(CMAKE_INSTALL_PREFIX "${BUILD_PARENT_DIR}" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)

set (SUDOKUSOLVER_SOURCE
  SUTypeDefs.cc
  SUDisplay.cc
  SUIncidenceMatrix.cc
  SUSolver.cc
  SudokuSolver.cc
  )

add_executable (SudokuSolver ${SUDOKUSOLVER_SOURCE})

set (SUDOKUGENERATOR_SOURCE
  SUTypeDefs.cc
  SUDisplay.cc
  SUIncidenceMatrix.cc
  Random.cc
  SUSolver.cc
  SUGenerator.cc
  SUProfiler.cc
  SudokuGenerator.cc
  )

add_executable (SudokuGenerator ${SUDOKUGENERATOR_SOURCE})

set (SUDOKUPROFILER_SOURCE
  SUTypeDefs.cc
  SUDisplay.cc
  SUIncidenceMatrix.cc
  Random.cc
  SUSolver.cc
  SUGenerator.cc
  SUProfiler.cc
  Histogram.cc
  SudokuProfiler.cc
  )

add_executable (SudokuProfiler ${SUDOKUPROFILER_SOURCE})

install (TARGETS SudokuSolver SudokuGenerator SudokuProfiler DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)