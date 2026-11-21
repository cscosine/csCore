# csCore

Core C++ utilities: asserts, formatting, Eigen helpers, math, named types, test utilities

<!-- cs-template:begin (this block is owned by cs-project-template: do not edit; the rest of the file is the project's) -->
## Getting started

csCore is built by the project that consumes it: no setup or CI of its own. Formatting and checks:
`pre-commit install` once, then `pre-commit run --all-files`.

The shared files (setup, checks, CI, agent rules) come from
[cs-project-template](https://github.com/cscosine/cs-project-template); to bring them to its latest version, on a
branch: `copier update --defaults`.
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
