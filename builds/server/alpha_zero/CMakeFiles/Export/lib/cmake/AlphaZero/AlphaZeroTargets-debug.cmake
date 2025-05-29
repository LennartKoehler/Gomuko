#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "AlphaZero::alpha_zero" for configuration "Debug"
set_property(TARGET AlphaZero::alpha_zero APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(AlphaZero::alpha_zero PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/alpha_zero"
  )

list(APPEND _IMPORT_CHECK_TARGETS AlphaZero::alpha_zero )
list(APPEND _IMPORT_CHECK_FILES_FOR_AlphaZero::alpha_zero "${_IMPORT_PREFIX}/bin/alpha_zero" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
