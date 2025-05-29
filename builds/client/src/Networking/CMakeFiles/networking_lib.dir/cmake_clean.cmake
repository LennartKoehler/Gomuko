file(REMOVE_RECURSE
  "libnetworking_lib.a"
  "libnetworking_lib.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/networking_lib.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
