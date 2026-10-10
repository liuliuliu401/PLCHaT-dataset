# PLCHaT Dataset

A benchmark of IEC 61131-3 Structured Text (ST) programs for evaluating PLC
testing tools. It is designed for use with the PLCHaT agentic harness (main
PLCHaT repository).

## Origin

The reference programs come from the Siemens Cup Industrial Software
Competition (Generative AI Application Competition: Code Generation for
Industrial Software, 2024,
[biendata.xyz/competition/siemens-ai](https://www.biendata.xyz/competition/siemens-ai)),
formatted by the AutoPLC pipeline (Yang et al., ASE 2025). Each program was
manually adapted for MatIEC runtime compatibility. The 45 mutated tasks and
318 mutants are the evaluation set used in the PLCHaT paper.

## Contents

| Directory | Description | Scale |
|---|---|---|
| `benchmark_raw/` | Reference ST programs with input schemas and prebuilt C harnesses | 46 tasks |
| `benchmark_mutated/` | Mutation-testing variants of the reference programs | 45 tasks, 318 mutants |

`benchmark_raw/` also contains three auxiliary files that are not tasks:
`competition_origin.jsonl` (original competition problem statements),
`task_classification.json` (control-flow category per task), and
`review_schemas.txt` (per-task input schema review notes).

Tasks in `benchmark_mutated/` are a subset of `benchmark_raw/`. Each mutant
lives in its own `mutation_N` directory and carries the diff against the
reference program (`patch_diff.txt`) plus a review record
(`QUALITY_AUDIT.md`). Files named `.grade_*` are quality-gate markers.

## Task Layout

```
benchmark_raw/<task>/
├── plc_st_files/<task>.st     # Structured Text source
├── input/
│   ├── input.txt              # Default test input
│   ├── input_schema.json      # Input schema, v1 (I/O columns only)
│   └── input_schema_v2.json   # Input schema, v2 (read by PLCHaT; also
│   │                          # covers state variables and per-variable
│   │                          # IEC types, array ranges, and ST bindings)
└── plc_c_files/               # Prebuilt C harness artifacts

benchmark_mutated/<task>/mutation_N/
├── plc_st_files/<task>.st     # Mutated ST source
├── patch_diff.txt             # Diff from the reference program
├── QUALITY_AUDIT.md           # Mutation review record
└── ...                        # Same layout as raw tasks
```

Both schema versions ship with every task. PLCHaT reads
`input_schema_v2.json`: beyond the I/O columns listed by v1, it also covers
internal state variables and carries per-variable metadata (IEC type, array
range, and the binding to the ST-level variable name). The v1 schema is the
earlier I/O-only description.

## Usage with PLCHaT

Clone this repository next to the PLCHaT main repository:

```
git clone https://github.com/liuliuliu401/PLCHaT-release.git PLCHaT-release
git clone https://github.com/liuliuliu401/PLCHaT-dataset.git PLCHaT-dataset
```

PLCHaT resolves the dataset location in this order:

1. The `PLCHAT_BENCHMARK_ROOT` environment variable, if set.
2. A `PLCHaT-dataset` directory next to the PLCHaT root (default).

Example:

```bash
python main.py harness generate \
  PLCHaT-dataset/benchmark_raw/211_StackMin/plc_st_files/211_StackMin.st \
  output/211_StackMin
```

## License

Apache License 2.0 — see [LICENSE](LICENSE).
