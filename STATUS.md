# Forge — Project Status

**Last updated:** 2026-11-11  
**Current stage:** Stage 1 — Transformer Architecture (in progress; completion not yet verified)  
**Last completed stage:** Stage 0 — C++ Foundations (2026-11-10)  
**Overall status:** Stage 0 complete; Stage 1 architecture review next  
**Completed stages:** 1 of 7  
**Hours this week:** ~4–10 h (estimated, not timed)  
**Cumulative development time:** ~4–10 h (estimated)  
**Remaining estimate:** 66–85 h across Stages 1–6, plus 12–20 h reserve; provisional and subject to roadmap review  
**Projected finish:** TBD after a capacity forecast; the 16-week checkpoint is not a guaranteed completion date

> This file is the source of truth for verified Forge progress. Update it after meaningful work. A discussion, proposed design, or AI-generated answer is not evidence that code works.

## Stage tracker

| Stage | Status | Evidence / completion gate |
|---|---|---|
| 0 — C++ Foundations | Complete | `Tensor2D`, CMake/CTest; Debug, Release, and ASan + UBSan tests passed on 2026-11-10 |
| 1 — Transformer Architecture | In progress | Independent GPT-2 forward-pass and shape walkthrough still to be verified |
| 2 — Python Reference | Not started | Reference comparisons against Hugging Face GPT-2 |
| 3 — C++ Engine | Not started | Full FP32 inference and greedy generation validated |
| 4 — KV Cache | Not started | Cached and uncached outputs validated |
| 5 — Optimization | Not started | Profile-driven optimization with measured results |
| 6 — Evaluation | Not started | Reproducible benchmarks and technical documentation |

*Stage 1 is provisionally in progress based on prior architecture study, not a verified completion claim. Stage 0's completion does not imply the full C++ engine is implemented.*

## Remaining-effort forecast

Update at weeks 4, 8, and 12, and whenever capacity changes materially. Estimates are remaining work, not hours already spent.

| Stage | Hours remaining | Projected completion | Notes |
|---|---:|---|---|
| 0 — C++ Foundations | 0 | 2026-11-10 | Completed in ~4–10 h estimated; additional C++ practice may continue |
| 1 — Architecture | 4–5 | TBD | Reassess after reviewing existing understanding |
| 2 — Python Reference | 10–12 | TBD | |
| 3 — C++ Engine | 20–26 | TBD | |
| 4 — KV Cache | 8–10 | TBD | |
| 5 — Optimization | 14–18 | TBD | |
| 6 — Evaluation | 10–14 | TBD | |

**Remaining planned work:** 66–85 h, excluding 12–20 h reserve. These are unrevised planning estimates, not measured forecasts.

## Checkpoint decisions

| Checkpoint | Actual hours | Forecast hours left | Projected finish | Decision and reason |
|---|---:|---:|---|---|
| Week 4 | — | — | — | Pending |
| Week 8 | — | — | — | Pending |
| Week 12 | — | — | — | Pending |

## Frozen reference configuration

*Freeze before Stage 2 numerical comparisons; do not invent values.*

| Item | Version / value | Evidence / location |
|---|---|---|
| Hugging Face checkpoint and revision | TBD | — |
| `transformers` version | TBD | — |
| `torch` version | TBD | — |
| Tokenizer and version | TBD | — |
| Model config / precision / eval mode | GPT-2 small / FP32 planned; exact config TBD | — |
| Deterministic test token IDs / seeds | TBD | — |

## Last verified functionality

- Minimal `Tensor2D` with contiguous FP32 storage owned by `std::vector<float>`.
- Shape validation, multiplication-overflow protection, checked indexing, and const element access.
- Copy and move construction/assignment, including self-assignment behavior; moved-from tensors use an empty `(0, 0)` state.
- CMake static library and CTest test executable; always-active `CHECK()` test macro, including intentional failure detection.

## Test results

| Date | Command / test | Result | Inputs / shapes / max-abs / relative error / notes |
|---|---|---|---|
| 2026-11-10 | Debug build / CTest | 1/1 passed | `Tensor2D` construction, indexing, bounds, validation, const access, copy/move tests; exact command not recorded |
| 2026-11-10 | Release build / CTest | 1/1 passed | Same test suite; exact command not recorded |
| 2026-11-10 | AppleClang 17.0.0, ASan + UBSan / CTest | 1/1 passed; no reported sanitizer errors | Exact build flags/commands not recorded |
| 2026-11-10 | Intentional `CHECK()` failure | Failure detected as expected | Confirms test checks remain active; exact command not recorded |

## Known bugs and unresolved questions

- No outstanding Stage 0 completion requirements reported.
- Verify Stage 1 architecture knowledge and identify gaps before beginning the next implementation milestone.
- Record exact test commands and configurations for future reproducibility.
- Confirm hardware details only when hardware-specific optimization becomes relevant.

## Latest benchmarks

No inference performance measurements recorded. Do not infer performance from theoretical expectations.

| Date | Hardware / build | Workload | Metric | Result |
|---|---|---|---|---|
| — | — | — | — | — |

## Engineering decisions

| Decision | Rationale | Date |
|---|---|---|
| GPT-2 small, FP32, batch size 1 | Limit initial scope and simplify validation | 2026-11-08 |
| CPU-first C++20 + CMake | Focus on understanding and reproducibility | 2026-11-08 |
| Python tokenization and weight export | Avoid noncritical tokenizer/parser work | 2026-11-08 |
| Hugging Face GPT-2 as correctness oracle | Independent reference for activations and logits | 2026-11-08 |
| Core kernels authored with understanding | Preserve the project's learning objective | 2026-11-08 |
| `std::vector<float>` owns tensor storage; shape metadata private | RAII ownership and a minimal public interface | 2026-11-10 |
| Moved-from tensors use empty `(0, 0)` shape | Predictable post-move state | 2026-11-10 |
| CMake library, CTest, and always-active `CHECK()` | Reproducible builds and tests independent of `NDEBUG` | 2026-11-10 |

## Development log

### 2026-11-10 — Stage 0 Tensor and Build Verification

- **Stage:** Stage 0 — C++ Foundations
- **Status:** Complete
- **Time spent:** ~4–10 hours (estimated)
- **Cumulative project time:** ~4–10 hours (estimated)
- **Implemented:** Minimal `Tensor2D` with contiguous FP32 storage, shape validation, checked indexing, const access, and copy/move operations.
- **Build system:** CMake static library and CTest executable.
- **Debug:** 1/1 CTest tests passed.
- **Release:** 1/1 CTest tests passed.
- **Sanitizers:** AppleClang 17.0.0, ASan + UBSan, 1/1 tests passed, no reported sanitizer errors.
- **Test infrastructure:** Always-active `CHECK()` macro, with intentional failure detection verified.
- **Design decisions:** `std::vector<float>` owns storage; shape metadata remains private; moved-from tensors use an empty `(0, 0)` state.
- **Remaining:** No outstanding Stage 0 completion requirements. Further C++ practice can continue alongside subsequent stages.
- **Next action:** Review existing Stage 1 architecture knowledge, identify gaps, and begin the next roadmap milestone.

## Next concrete action

1. Review Stage 1 requirements in `ROADMAP.md` and assess existing GPT-2 architecture knowledge.
2. Complete an independent forward-pass and tensor-shape walkthrough; record any gaps.
3. Begin the next roadmap milestone only after defining its verification criteria.

## Session update template

Copy and fill in after a substantial work session:

```markdown
### YYYY-MM-DD — Session summary
- **Time spent (this session / this week / cumulative):**
- **Stage / objective:**
- **Changed files:**
- **What I implemented or learned:**
- **Tests run and exact results (include inputs, shapes, max-abs and relative error for numerical comparisons):**
- **Numerical differences / benchmarks (if applicable):**
- **Bugs or unresolved questions:**
- **Decisions and rationale:**
- **Remaining hours / projected finish changes:**
- **Next concrete action:**
```

## Status rules

- Use **Not started**, **In progress**, **Blocked**, or **Complete**; mark **Complete** only with evidence that the stage gate passed.
- Record actual commands, inputs, hardware, and results for important claims.
- Keep detailed historical session notes below or in a separate log; keep the top summary current.
- If available hours fall below 4/week, use the 2–3 hour maintenance plan from `ROADMAP.md` and revise the forecast.
- For disagreements between ChatGPT and Claude, follow `COLLABORATION.md`; record only resolved decisions here.
