file(REMOVE_RECURSE
  "libgame_client_lib.a"
  "libgame_client_lib.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/game_client_lib.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
