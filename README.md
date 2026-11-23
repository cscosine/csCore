# csCore

Core C++ utilities: asserts, formatting, Eigen helpers, math, named types, test utilities

<!-- cs-template:begin (this block is owned by cs-project-template: do not edit; the rest of the file is the project's) -->
## Getting started

csCore is built by the project that consumes it: no setup or build CI of its own. Formatting and checks:
`pre-commit install` once, then `pre-commit run --all-files` (also run by `.github/workflows/checks.yml`).

The shared files (setup, checks, CI, agent rules) come from
[cs-project-template](https://github.com/cscosine/cs-project-template); to bring them to its latest version, on a
branch: `copier update --defaults`.

## Working in VS Code

Once the project that builds csCore has built it (csorchestrator), open VS Code with `./open-code.sh [filter]`
(Windows: `.\open-code.ps1 [filter]`).
The filter is a part of the build folder name or of the compiler (`gcc`, `clang`, `msvc`, `clang-cl`),
needed when there are several builds.

The script writes a **workspace file** for the chosen build, `<build>/csCore.code-workspace`, and opens it
(not the folder): it holds the build's settings (build folder, configurations, IntelliSense, debugger). The window title
shows `csCore - <build>`. To come back: run the script again, or File > Open Recent and pick that
workspace. Opening the folder itself (`code .`) gives a window without the build: CMake Tools would ask for a kit and
configure a new build of its own. Each build has its own workspace file and window, so gcc and clang can be open side
by side. The builds are found from the csCMake layout of `csCMakeProjectSetup.json`; CMake is the one that made the
build (its path is in the build's `CMakeCache.txt`).

Needs VS Code with its `code` command on the PATH (the Linux packages and the Windows installer add it; otherwise the
script says which file to open by hand) and, on Linux, gdb to debug.

The first time in a window (VS Code remembers it):

1. CMake Tools asks "Select a kit": pick **`[Unspecified]`**, the build keeps its own compiler. CMake Tools then
   configures (Output, CMake).
2. Linux: clangd asks to install itself, or to disable the other IntelliSense: yes. Windows (Visual Studio builds):
   nothing to do, IntelliSense comes from CMake Tools.

Every day:

- The configuration (those of the build) and Build (F7): the status bar.
- Tests: the Testing view (Catch2: one entry per test case), run or debug one.
- The launch target (status bar): Run and Debug, "Debug the launch target" (F5, it builds first); with Catch2 also
  "Debug the launch target (Catch2 test spec)" (a test name or a `[tag]`).
- Install (for the next libraries, the deployments): command palette, "CMake: Install".
<!-- cs-template:end -->

## Assert levels (csAssert)

`cs_assert_generate_project(<project> <namespace>)` (in `csCore/assert/cmake/csAssert.cmake`, installed with the package
for the other projects) creates `<namespace>::<project>Assert` and its header `<project>/<project>Assert.hpp`, with the
macros `<PROJECT>_ASSERT_<LEVEL>(...)` (levels `RELEASE`, `RELWITHDEBINFO`, `DEBUG`, `PARANOID`, `SAFE_MATH`) and
`<namespace>::Asserts::is...Enabled()`.

The header is the same for every configuration; the levels enabled are the compile definitions
`<PROJECT>_ASSERT_<LEVEL>_ENABLED` of the target, per configuration: `$<$<CONFIG:c1,c2,...>:<define>>`, from the CMake
options `<PROJECT>_ASSERT_<LEVEL>_ENABLED_CONFIG_<Config>` (multi-config) or `<PROJECT>_ASSERT_<LEVEL>_ENABLED`
(single config: the configuration of the build; the defaults for the others, so that the installed package does not
depend on the configuration installed last). The consumers of the installed package get the levels of their own
configuration (or of the one CMake maps it to). No source of the target varies by configuration: the Xcode generator
works (it does not support per-configuration sources, which the previous design, one header per configuration, had).
The header is `csCore/assert/cmake/templates/ProjectAssert.hpp.in` (the project name substituted); a new level is
added there and in `DEFINE_NAMES` of `csAssert.cmake`.

## TODO

- **clang-tidy**: add a `.clang-tidy` with the checks of csCore and a compile profile rule running it (e.g. in Paranoid,
  with Clang on Linux), then fix its findings. The `cs_exec_clang_tidy` calls of the previous csCMake never ran
  clang-tidy and are removed: no target of csCore runs it today. csCMake: `cscmake_clang_tidy` and the `CLANG_TIDY`
  rules (see its documentation, "Compile profiles").
