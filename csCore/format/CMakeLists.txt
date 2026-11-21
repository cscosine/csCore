project(format LANGUAGES CXX)

cs_add_library(format INTERFACE)
cs_add_library(csCore::format ALIAS format)

cs_find_package(format PUBLIC fmt REQUIRED CONFIG)

cs_target_sources(
  format
  PRIVATE_INSTALL
    FormatCore.hpp
    Args.hpp
    Base.hpp
    Chrono.hpp
    Color.hpp
    Compile.hpp
    Core.hpp
    Format.hpp
    Format-inl.hpp
    Os.hpp
    Ostream.hpp
    Printf.hpp
    Ranges.hpp
    Std.hpp
    Xchar.hpp
  PRIVATE
)

cs_target_link_libraries(format INTERFACE csCore::csCore fmt::fmt)

cs_install_target(format COMPONENT format)

# compile each single header in standalone, ensures includes are self contained
cs_target_check_installed_headers(format COMPILE_PROFILES cscmake_strict_warnings)
