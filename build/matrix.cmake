# Librar;
set(MATRIX matrix)

set (matrix_private_src
        bdf-font.cc
        content-streamer.cc
        framebuffer.cc
        framebuffer-internal.h
        gpio.cc
        gpio.h
        gpio-bits.h
        graphics.cc
        hardware-mapping.c
        hardware-mapping.h
        led-matrix.cc
        led-matrix-c.cc
        multiplex-mappers.cc
        multiplex-mappers-internal.h
        options-initialize.cc
        pixel-mapper.cc
        thread.cc
        utf8-internal.h)
set (matrix_public_src
        canvas.h
        content-streamer.h
        graphics.h
        led-matrix.h
        led-matrix-c.h
        pixel-mapper.h
        thread.h
        threaded-canvas-manipulator.h)

list (TRANSFORM matrix_private_src PREPEND "${MATRIX}/lib/")
list (TRANSFORM matrix_public_src PREPEND "${MATRIX}/include/")

add_library(rgbmatrix STATIC)
target_sources(rgbmatrix PRIVATE ${matrix_private_src})
target_sources(rgbmatrix PUBLIC ${matrix_public_src})
target_include_directories(rgbmatrix PUBLIC ${MATRIX}/include)

set (MATRIX_EXAMPLES
        c-example.c
        clock.cc
        demo-main.cc
        # image-example.cc
        input-example.cc
        ledcat.cc
        minimal-example.cc
        pixel-mover.cc
        scrolling-text-example.cc
        text-example.cc)

foreach (source ${MATRIX_EXAMPLES})
    string(REGEX MATCH "[a-zA-Z\\-]+" target_name ${source})
    string(REPLACE "-" "_" target_name ${target_name})
    add_executable(rgbmatrix_${target_name} ${MATRIX}/examples-api-use/${source})
    target_link_libraries(rgbmatrix_${target_name} PUBLIC rgbmatrix)
endforeach ()
