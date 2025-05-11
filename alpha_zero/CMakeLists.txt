cmake_minimum_required(VERSION 3.10)


set(CMAKE_CUDA_COMPILER /usr/local/cuda-12/bin/nvcc)


project(AgentTraining CUDA CXX)



set(CMAKE_PREFIX_PATH /usr/local/lib/libtorch_cuda/libtorch/)

find_package(Torch REQUIRED)
find_package(OpenMP REQUIRED) # Find OpenMP
include_directories(
  "${TORCH_INCLUDE_DIRS}"
)
# # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")


# set (MY_ANACONDA_PATH "/home/lennart/miniconda3/envs/alpha_zero_cpp")

# list(APPEND CMAKE_PREFIX_PATH "${MY_ANACONDA_PATH}/lib/python3.11/site-packages/torch/")


# # pytorch cuda version and this one must match
# set(CUDNN_INCLUDE_PATH
#    ${MY_ANACONDA_PATH}/pkgs/cudnn-7.6.4-cuda10.1_0/include/
#    )
# set (CUDNN_LIBRARY_PATH
#    ${MY_ANACONDA_PATH}/pkgs/cudnn-7.6.4-cuda10.1_0/lib/libcudnn.so
#    )


# find_package(CUDA  REQUIRED)
# include_directories("${CUDA_INCLUDE_DIRS}")


add_executable(alpha_zero
main.cpp
Trainer.cpp
Gomoku.cpp
MCTS.cpp
)

target_link_libraries(alpha_zero  ${TORCH_LIBRARIES} OpenMP::OpenMP_CXX)
# target_link_libraries(alpha_zero  /usr/local/lib/libtorch_cuda/libtorch/lib/  )
set_property(TARGET alpha_zero PROPERTY CXX_STANDARD 17)



#use as library
add_library(
    alpha_zero_lib
    Model.h
    Gomoku.cpp
    Gomoku.h
    MCTS.cpp
    MCTS.h
    Trainer.cpp
    Trainer.h
    CustomDataset.h
    PlayOut.h
)
# # target_link_libraries(alpha_zero PRIVATE game_common_lib)
target_include_directories(alpha_zero_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# At the end of your original CMakeLists.txt
install(TARGETS alpha_zero EXPORT AlphaZeroTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)
install(EXPORT AlphaZeroTargets
        FILE AlphaZeroTargets.cmake
        NAMESPACE AlphaZero::
        DESTINATION lib/cmake/AlphaZero
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION include/alpha_zero
        FILES_MATCHING PATTERN "*.h"
)