project "array"
    language    "C++"
    kind        "StaticLib"
    warnings    "off"

    includedirs { "src" }

    files {
        "src/array.cpp",
        "src/array.h",
        "src/core.hpp",
    }
