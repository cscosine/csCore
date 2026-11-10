## csCore

## TODO

- **clang-tidy**: add a `.clang-tidy` with the checks of csCore and a compile profile rule running it (e.g. in Paranoid,
  with Clang on Linux), then fix its findings. The `cs_exec_clang_tidy` calls of the previous csCMake never ran
  clang-tidy and are removed: no target of csCore runs it today. csCMake: `cscmake_clang_tidy` and the `CLANG_TIDY`
  rules (see its documentation, "Compile profiles").
