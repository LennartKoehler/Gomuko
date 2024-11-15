file(REMOVE_RECURSE
  "libgame_server_lib.a"
  "libgame_server_lib.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/game_server_lib.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
