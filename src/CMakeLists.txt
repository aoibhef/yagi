set(YAGI_SRC_FILES ${YAGI_SRC_FILES}
        src/yagi/core/application.cpp
        src/yagi/core/glfw_callbacks.cpp
        src/yagi/core/oracle.cpp
        src/yagi/core/window.cpp

        src/yagi/gfx/gl/buffer_base.cpp
        src/yagi/gfx/gl/framebuffer.cpp
        src/yagi/gfx/gl/shader.cpp
        src/yagi/gfx/gl/vertex_array.cpp
        src/yagi/gfx/color.cpp
        src/yagi/gfx/context.cpp

        src/yagi/msg/bus.cpp

        src/yagi/util/noise/perlin.cpp
        src/yagi/util/averagers.cpp
        src/yagi/util/framecounter.cpp
        src/yagi/util/log.cpp
        src/yagi/util/rnd.cpp
        src/yagi/util/ticker.cpp)