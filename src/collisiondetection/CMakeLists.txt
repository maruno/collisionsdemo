#Collision detection library
set(awesomecollisions_INCS boundingvolume.hpp axisalignedboundingbox.hpp)

set(awesomecollisions_SRCS axisalignedboundingbox.cpp)

add_library(awesomecollisions STATIC ${awesomecollisions_SRCS} ${awesomecollisions_INCS})
