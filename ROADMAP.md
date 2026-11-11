# Forge — Engineering Roadmap

**Project:** GPT-2-compatible C++20 inference engine  
**Start:** Monday, November 9, 2026  
**Budget:** 6–8 focused hours/week; 14–16-week planning window  
**Primary development:** Apple Silicon MacBook Pro, CPU-first  
**Secondary hardware:** AMD Radeon gaming PC; GPU model, OS, and compatibility not yet established  
**Status source of truth:** `STATUS.md` in the repository

## 1. Objective and definition of success

Build an independently understood, reproducible GPT-2 small (~124M parameter) inference engine in C++20. Use a separately implemented Python/PyTorch reference and Hugging Face `transformers` GPT-2 as correctness oracles. Support FP32 inference, batch size 1, greedy decoding, and KV-cached generation. Profile the CPU implementation and implement **one measured, substantial optimization**.

**Core success (must-have):**
- Independently explain the GPT-2 forward pass, tensor shapes, ownership, and numerical behavior.
- Run a standalone C++ executable with exported pretrained weights and Python-supplied token IDs.
- Pass numerical and generation tests against reference implementations.
- Generate with a correct KV cache.
- Produce reproducible prefill/decode benchmarks, a PyTorch CPU comparison, and a profiling-based optimization report.
- Publish readable build, test, architecture, and limitations documentation.

**Not required:** native BPE, batching, sampling, GPU, quantization, multiple optimized kernels, or beating production BLAS. These are optional extensions, not completion gates.

## 2. Scope and architecture decisions

| Area | Initial choice | Rationale / boundary |
|---|---|---|
| Model | GPT-2 small, pretrained | Fixed architecture and known reference outputs |
| Precision | FP32 | Establish correctness before reduced precision |
| Batch / decoding | Batch 1; greedy | Limit complexity and isolate inference behavior |
| Tokenizer | Established GPT-2 tokenizer in Python | Native BPE is not a prerequisite |
| Weights | Python export to documented binary | Validate names, shapes, dtype, byte order, layout |
| Implementation | C++20, CMake, RAII, standard containers | Explicit, safe memory ownership |
| CPU | Single-threaded baseline | Controlled baseline before threading |
| GPU | Out of scope initially | No CUDA assumption; Metal/AMD feasibility later |
| Libraries | Testing, tooling, comparison allowed | Independently implement core operations; disclose external kernels |

Important GPT-2 compatibility details to check: Hugging Face `Conv1D` weight orientation, LayerNorm epsilon, GELU variant, attention scaling and masking, positional embedding indexing, final LayerNorm, and tied output embedding weights.

## 3. Effort budget and planning gates

At 6–8 hours/week, 16 fully available weeks would provide **96–128 hours**, but finals, winter break, and a possible January co-op may reduce that capacity. The stages below total **78–101 hours**; add a **12–20 hour contingency reserve** for a **90–121 hour planning envelope**. This is a workload estimate, **not** a promise that the work fits into 16 calendar weeks. Reforecast using actual available hours, and extend the end date if necessary.

| Stage | Target effort | Exit gate |
|---|---:|---|
| 0. C++ foundations | 12–16 h | Tested tensor container and reproducible build |
| 1. Architecture | 4–5 h | Independent forward-pass and shape explanation |
| 2. Python reference | 10–12 h | Verified against Hugging Face GPT-2 |
| 3. C++ engine | 20–26 h | Correct standalone FP32 inference and generation |
| 4. KV cache | 8–10 h | Cached and uncached agreement |
| 5. Profiling + one optimization | 14–18 h | Measured, validated improvement or defensible negative result |
| 6. Evaluation + documentation | 10–14 h | Reproducible benchmarks and technical report |
| **Planned work** | **78–101 h** | |
| **Contingency reserve** | **12–20 h** | |
| **Total planning envelope** | **90–121 h** | |

**Checkpoints:** Use the week 4/8/12 forecast-and-evidence rules in Section 5. Stage completion requires verified exit tests recorded in `STATUS.md`.

## 4. Stage details

### Stage 0 — C++ foundations (12–16 h, spread across weeks 1–4)

**Learn:** compilation/linking, CMake, Debug vs Release, compiler warnings, RAII, `std::vector`, `std::span`, const/reference semantics, move basics, indexing, ownership, unit tests, and sanitizers where supported.

**Build:** a minimal contiguous row-major FP32 tensor with shape metadata and safe indexing (strides optional; do not build a general tensor-view framework); add shape, indexing, and invalid-input tests. Establish a basic `cmake --build` and `ctest` workflow. The toy C++ softmax and LayerNorm exercises in weeks 3–4 **count toward Stage 0’s 12–16 hours**; they are not additional work.

**Exit:** clean Debug tests; an optimized Release build; ability to explain storage layout and object lifetime. Avoid building a general-purpose tensor framework.

### Stage 1 — Transformer architecture (4–5 h)

**Learn:** token/position embeddings, residual stream, LayerNorm, Q/K/V, causal multi-head attention, projection, MLP/GELU, output logits, and autoregressive decoding.

**Build:** `docs/architecture.md` with one transformer-block diagram or textual data flow and shape table (e.g. batch 1, hidden size 768, 12 heads, head size 64, MLP size 3072). Explain prefill versus decode and why KV caching works.

**Exit:** independently trace a prompt through embeddings → 12 blocks → final normalization → logits and explain key tensor dimensions.

### Stage 2 — Python correctness reference (10–12 h)

**Build:** a GPT-2 forward pass from basic PyTorch operations without calling built-in transformer/attention modules for the core math. Load matching pretrained weights. Validate each block and final logits against Hugging Face `transformers` GPT-2. Implement greedy token generation.

**Tests:** deterministic token IDs, multiple sequence lengths, attention-mask edge cases within scope, intermediate activation diffs, final logits, and greedy generation. Export representative intermediate activations for C++ testing.

**Exit:** a repeatable test script showing numerical agreement and token-identical greedy output on fixed prompts. Record max absolute and appropriately defined relative error.

### Stage 3 — C++ FP32 engine (20–26 h)

**Build in this order:**
1. Binary weight exporter/loader with validated shapes and layouts.
2. FP32 tensor primitives and naive matrix multiplication.
3. LayerNorm, GELU, attention masking, softmax, and projections.
4. Complete transformer block and full GPT-2 forward pass.
5. Greedy token-generation loop and minimal CLI accepting token IDs.

**Validation:** compare individual kernels, selected block activations, final logits, and full greedy generation to Python. Diagnose first divergence layer-by-layer rather than debugging only final text.

**Exit:** standalone C++ CPU inference from pretrained weights, passing documented numerical tests and generating the same token IDs as the reference on fixed prompts.

### Stage 4 — KV cache (8–10 h)

**Build:** prefill mode plus incremental decode mode, per-layer key/value storage, explicit position bookkeeping, and explicit context-boundary behavior: **reject/stop before processing token positions beyond 0–1023**. As a conservative generation policy, require `prompt tokens + generated tokens ≤ 1024`. Do not silently implement a sliding window: GPT-2 uses learned absolute positional embeddings, and sliding requires separate design and validation. Avoid repeatedly reallocating cache storage if possible.

**Tests:** compare cached and uncached logits over multiple prompt lengths and decode steps; test sequence boundaries and correct causal indexing. Numerical agreement uses tolerances, not bitwise identity.

**Exit:** correct cached decoding and initial measured prefill/decode timings; document cache shape, lifetime, and memory cost.

### Stage 5 — Profiling and optimization (14–18 h)

**Profile first:** use Instruments Time Profiler on macOS. Determine whether runtime is dominated by matrix multiplication, allocation, attention, memory traffic, or another bottleneck. Capture a baseline profile and benchmark table.

**Choose one major optimization based on evidence:** start with cache-aware tiled matmul if appropriate; consider ARM NEON only if justified. Other possibilities: layout changes, reduced allocations, threading, or weight-only INT8 as a stretch goal. Measure both kernel-level and end-to-end effects.

**Exit:** a before/after comparison, correctness regression results, explanation of the bottleneck and change, and an honest account of regressions or limited improvements. A well-supported negative result can still be documented, but seek a meaningful demonstrated optimization when feasible.

### Stage 6 — Evaluation and documentation (10–14 h)

**Compare:** naive Forge, optimized Forge, PyTorch eager CPU with `torch.set_num_threads(1)`, and PyTorch with default threading. An established compatible inference runtime (e.g. llama.cpp if its model and configuration can be matched fairly) is optional; do not force a misleading comparison.

**Publish:** README, architecture, build/test guide, weight format, correctness results, benchmark scripts/results, profiler evidence, optimization write-up, known limitations, and reproducibility instructions.

**Exit:** another developer can build, test, and run benchmarks with documented assumptions. Prepare a concise technical explanation suitable for an interview.

## 5. Date-aware schedule (16-week checkpoint, not guaranteed finish)

Week 1 begins **Monday, November 9, 2026**. Finals dates and co-op start are not confirmed; the indicated periods are **planning assumptions** to verify against the university calendar and any offer. A 2–3-hour maintenance week is not equivalent to a full 6–8-hour week. The **Stage 3 engine is intentionally not scheduled to begin during presumed finals**. Stage 0 continues through weeks 3–4, overlapping Stage 2; the toy-kernel practice is part of the Stage 0 estimate, not extra hours.

| Week | Dates (Mon–Sun) | Capacity assumption | Focus / evidence |
|---|---|---|---|
| 1 | Nov 9–15 | 6–8 h | Stage 1 architecture (~2–3 h); Stage 0 compiler/CMake setup (~3–5 h) |
| 2 | Nov 16–22 | 6–8 h | Stage 1 architecture finish (~1–2 h); Stage 0 tensor ownership, indexing, tests (~4–6 h) |
| 3 | Nov 23–29 | 5–7 h | Stage 2 LayerNorm/attention; **1–2 h Stage 0 C++ toy softmax** (included in Stage 0 budget) |
| 4 | Nov 30–Dec 6 | 5–7 h | Stage 2 block checks; **1–2 h Stage 0 C++ toy LayerNorm** (included in Stage 0 budget); checkpoint |
| 5 | Dec 7–13 | 2–3 h (finals assumption) | Finish reference tests or small C++ kernel test; no engine kickoff |
| 6 | Dec 14–20 | 2–3 h (finals/break transition) | Fix reference mismatches; light documentation |
| 7 | Dec 21–27 | 4–8 h (break, variable) | Stage 3 weight export/loader, only if reference gate passed |
| 8 | Dec 28–Jan 3 | 4–8 h (break, variable) | Stage 3 C++ kernels; checkpoint/reforecast |
| 9 | Jan 4–10 | 3–6 h (possible co-op start) | Stage 3 attention/block correctness |
| 10 | Jan 11–17 | 3–6 h | Stage 3 full-model logits |
| 11 | Jan 18–24 | 3–6 h | Stage 3 greedy generation and tests |
| 12 | Jan 25–31 | 3–6 h | Stage 4 cache storage/prefill; checkpoint |
| 13 | Feb 1–7 | 3–6 h | Stage 4 cached decode and boundary tests |
| 14 | Feb 8–14 | 3–6 h | Stage 5 baseline profiling, identify bottleneck |
| 15 | Feb 15–21 | 3–6 h | Stage 5 one contained optimization and regression tests |
| 16 | Feb 22–28 | 3–6 h | Stage 6 benchmark/report progress; reforecast remaining work |

**Capacity reality check:** Under these assumptions, the 16-week window may supply only about **58–100 hours**, depending on break and co-op availability—possibly less than the 90–121-hour planning envelope. Expect Stage 5/6 and polish to extend into March or later unless actual hours are higher or work proceeds faster. Never count reserve hours twice. Track *actual* weekly hours and remaining estimated hours in `STATUS.md`; update target dates at each checkpoint.

**Checkpoint rules (authoritative):** At weeks 4, 8, and 12, compare verified stage gates and remaining effort with **forecast capacity**, not the original calendar. Record actual hours, remaining stage estimates, and the revised projected finish date in `STATUS.md`. If the forecast cannot fit, extend the date or cut optional work; never skip correctness gates. Keep 1–2 hours of Stage 0 C++ exercises during Python-heavy weeks when feasible; count these hours once, under Stage 0.

## 6. Numerical correctness protocol

1. Freeze pretrained checkpoint, tokenizer version, token IDs, and model configuration.
2. Use identical inputs and weight layouts across Hugging Face, independent PyTorch, and C++.
3. Test primitive outputs, intermediate activations, final logits, and greedy decoding.
4. Record `max_abs_error` and relative error with a denominator floor for near-zero reference values; include shapes and test inputs.
5. Start with a **tentative** FP32 final-logit max absolute target near `1e-4`; adjust only after investigating summation order, kernels, and scale. Do not relax tolerances to hide bugs.
6. Verify fixed-prompt greedy generation (target at least 32 tokens per prompt) and cached-versus-uncached agreement. **On the first token mismatch**, log both implementations’ top logits and the reference top-1 minus top-2 margin. A near-tie with logits inside tolerance can legitimately flip argmax; a large-margin mismatch indicates a more serious discrepancy. Report token identity and numerical agreement separately.
7. After tiling/SIMD, re-run comparisons to the reference **within justified tolerance**, not bitwise equality; floating-point reduction order may change.
8. If quantization is later added, report separate quality measures such as perplexity rather than requiring FP32-equivalent logits.

**Potential failure points:** incorrect `Conv1D` transposition, epsilon, GELU formulation, masking, attention scaling, output weight tying, positional indices, or softmax stability.

## 7. Benchmark protocol

**Metrics:** prefill tokens/sec; decode tokens/sec; time to first token; per-token decode latency; peak resident memory; correctness results.

**Workloads:** use explicit `(prompt tokens, generated tokens)` pairs such as `(128, 32)`, `(256, 64)`, `(512, 128)`, `(896, 128)`, and `(992, 32)`; **prompt + generation ≤ 1024**. For prefill-only tests, a 1024-token prompt is valid. Its final logits can select one next token without processing it, but feeding that token back would require the invalid position index 1024. The conservative benchmark/generation policy still requires `prompt + generated ≤ 1024`, so a full-length prompt is measured as prefill-only. Keep batch size 1 and greedy decoding. Separate prefill from decoding; distinguish kernel microbenchmarks from end-to-end inference.

**Method:**
- Use optimized **Release** builds with recorded compiler and flags. Run ASan/UBSan separately in instrumented Debug builds, never as performance comparisons.
- Warm up, repeat trials, report median and spread; note whether runs are in fresh processes.
- Record CPU, machine, OS, compiler, model/checkpoint, precision, thread count, prompt/generation lengths, and benchmark commands.
- Benchmark **Hugging Face `transformers` GPT-2** (not the uncached Stage 2 teaching reference) with pretrained matching weights, `model.eval()`, no-grad/inference mode, and `use_cache=True` for decoding. Verify that cache reuse actually occurs in the benchmark loop. Run PyTorch at **one thread** and at **default threading**; report both and explain the role of optimized BLAS. Ensure equivalent token generation work and clearly label any differences in runtime overhead.
- Prefer identical CPU hardware for within-machine comparisons; report Apple Silicon and AMD-PC measurements separately.
- For peak memory, use a documented isolated-process method such as `getrusage(RUSAGE_SELF).ru_maxrss`; note that macOS reports bytes and Linux generally reports KiB. State whether measurement includes model load and whether it captures process high-water mark rather than instantaneous usage. **Primary memory claim:** compare Forge peak RSS against its own analytic estimate (FP32 weights: ~124M × 4 bytes ≈ 500 MB decimal; full 1024-position KV cache: `2 × 12 × 1024 × 768 × 4` ≈ 75 MB decimal), plus allocator/activations/overhead. PyTorch RSS includes the Python interpreter and framework, so report it as context, **not a directly comparable memory-efficiency score**.
- Preserve raw results in `benchmarks/results/` and summary tables in docs. Never infer a speedup from a single noisy run.

**Interpretation:** improvements over naive Forge are meaningful engineering results, but do not imply superiority to production runtimes. Explicitly report the remaining gap to optimized external implementations.

## 8. Scope controls and contingencies

**If week 4 is behind:** reduce reference test breadth but retain essential layer-level and final-logit checks; defer ancillary tooling. Do not skip reference validation.

**If week 8 is behind:** reforecast the engine completion date rather than rushing the C++ implementation; prioritize full C++ inference over a polished CLI, native tokenization, or optimization work. Use a simple token-ID input format.

**If week 12 is behind:** focus on KV cache correctness, a baseline profile, and one contained optimization. Drop SIMD/quantization/GPU unless already nearly complete.

**Minimum viable week:** When possible, preserve 2–3 hours for one bounded task, a test, or a documented investigation, even during finals or co-op transitions. **If weekly hours fall below 4:** freeze new features, maintain one small weekly milestone when feasible, update the forecast in `ROADMAP.md`, and preserve tests. Extend the project beyond 16 weeks rather than declaring incomplete work finished.

**If implementation proves too slow:** first simplify interfaces and reduce feature scope. Do not substitute external inference kernels for the core implementation without explicitly changing and documenting the project's objective.

## 9. Repository structure (suggested, not mandatory)

```text
forge/
  CMakeLists.txt
  README.md
  ROADMAP.md
  STATUS.md
  src/                 # C++ engine, kernels, model, CLI
  include/             # C++ public headers
  tests/               # C++ tests and fixtures
  python/              # reference, weight export, validation
  benchmarks/          # runners, configs, results/
  docs/                # architecture, weight format, optimization report
  models/              # local weights (gitignored)
```

Do not add directories before they serve an actual need. Keep large model files and generated artifacts out of Git unless intentionally managed.

## 10. Progress record

Use the actual `STATUS.md` as the canonical structure for hours, stage forecasts, checkpoints, frozen reference configuration, tests, decisions, and next actions. Do not maintain a competing template here. Update after substantial sessions and at the week 4/8/12 checkpoints; only mark a stage Complete when its exit gate is verified.

## 11. Interview-ready final narrative

Be prepared to explain: how GPT-2 maps to C++ tensors and kernels; why LayerNorm/attention/MLP are implemented as they are; ownership and memory layout; how numerical equivalence was tested; how the KV cache changes work per token; what profiling revealed; how the optimization changes memory access or arithmetic; and why external BLAS/runtimes may remain faster.

**Final standard:** a working, understood, tested, benchmarked engine with an evidence-based engineering story—not an unverified feature checklist.
