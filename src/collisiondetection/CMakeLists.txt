#Collision detection library
set(awesomecollisions_INCS boundingvolume.hpp axisalignedboundingbox.hpp objectorientedboundingbox.hpp collidable.hpp vclip.hpp)

set(awesomecollisions_SRCS axisalignedboundingbox.cpp objectorientedboundingbox.cpp vclip.cpp)

add_library(awesomecollisions STATIC ${awesomecollisions_SRCS} ${awesomecollisions_INCS})
