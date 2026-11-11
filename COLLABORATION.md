# Forge — Multi-Model Collaboration

## Purpose

Forge is a C++20 GPT-2 inference engine built to develop independent understanding of transformer mathematics, systems programming, numerical correctness, and performance engineering. Multiple AI assistants are used to challenge reasoning and accelerate routine work—not to substitute for understanding.

This document complements the Forge ChatGPT Project instructions and `ROADMAP.md`. **`STATUS.md` is the source of truth for verified implementation progress.** A discussion, proposed patch, or AI assertion does not establish that code works.

## Responsibilities

| Participant | Primary role | Boundaries |
| --- | --- | --- |
| **Me (project owner)** | Make decisions, implement and explain core kernels, run experiments, accept changes | Own technical understanding and final verification |
| **ChatGPT** | Conversational tutor, design sounding board, reasoning and audit adjudication | Hints-first for learning-critical operations; no unsolicited complete kernels |
| **Claude** | Independent technical auditor of explanations, designs, tests, and performance claims | Findings are hypotheses to assess, not instructions to obey |
| **Claude Code** | Repository navigation, build/test tooling, debugging, profiling, targeted implementation assistance | Do not silently rewrite learning-critical kernels; request authorization before editing them |

## When to use each tool

**Everyday learning:** Ask ChatGPT about transformer concepts, shapes, numerical behavior, C++, and design tradeoffs. No mandatory second-model review.

**Consequential technical decisions:** Ask Claude to audit designs involving correctness, memory safety, tensor layouts, model compatibility, KV-cache semantics, benchmark fairness, or major optimization choices.

**Repository work (especially Stage 3 onward):** Use Claude Code for navigating code, inspecting diffs, running available tests, preparing build infrastructure, reproducing failures, and proposing small patches. Delegate boilerplate freely when useful; retain ownership of core computations.

**Disputed findings:** Bring Claude's audit to ChatGPT for independent evaluation, then settle remaining disagreements with tests, reference behavior, documentation, or measured results. Avoid endless model-to-model debate.

## Audit format

Ask Claude to return actionable findings in this form:

```text
[bug | risk | preference] Short title
Claim: What is wrong or questionable?
Evidence: Relevant code, equations, inputs, or documentation.
Impact: Why it matters for Forge.
Check: Smallest test, measurement, or authoritative source that could verify it.
Suggested fix: Minimal correction, if warranted.
```

- **Bug:** Demonstrable incorrect behavior, invalid assumption, memory-safety issue, or contradiction with specified requirements.
- **Risk:** Plausible failure mode, uncertain dependency, unvalidated assumption, or threat to schedule/correctness/performance.
- **Preference:** Reasonable alternative or stylistic suggestion without a demonstrated correctness or project-goal impact.

Severity labels are provisional; a finding may be reclassified after review. Request concrete evidence and a falsifiable check rather than vague criticism.

## Handling an audit in ChatGPT

For each finding:

1. **Classify and evaluate independently.** Do not agree merely because Claude raised it, and do not reject it to defend a previous answer.
2. **Prioritize bugs, then material risks.** Preferences may be deferred.
3. **State a disposition:** Accepted, Partially accepted, Rejected, or Needs verification.
4. **Give a concise rationale** grounded in Forge's actual requirements, implementation, and available evidence.
5. **Mark what changed** from the prior recommendation, design, or document. If nothing changed, say so.
6. **For disagreement or uncertainty, specify a decisive check:** minimal reproducible test, numerical comparison, compiler/sanitizer run, profiler experiment, or authoritative documentation.

Do not treat two models agreeing as proof. Tests and evidence decide technical questions.

## Implementation ownership

**Learning-critical work:** tensor indexing/layout, matrix multiplication, LayerNorm, softmax, attention and masking, GPT-2 block integration, KV caching, and performance kernels. Prefer explanation → targeted hint → pseudocode → full code only on explicit request. Claude Code may inspect and review these files but should not modify them without permission.

**Delegable infrastructure:** CMake, CI, CLI parsing, weight export/conversion, test harness setup, benchmark automation, documentation formatting, and routine scripts. Full code or direct edits are acceptable when requested, provided changes are reviewed and tests run.

Before accepting AI-written changes to important code, I should be able to explain its purpose, shapes, invariants, ownership, complexity, and tests. A passing test alone is not evidence of understanding.

## Typical work cycle

1. **Orient:** Read current `STATUS.md` and relevant `ROADMAP.md` stage gate. Identify one concrete task.
2. **Reason:** Use ChatGPT to understand the task and design the smallest implementation/test.
3. **Implement:** Write learning-critical logic myself; use Claude Code for supporting tooling as appropriate.
4. **Verify:** Run tests and compare with the established Python/Hugging Face reference. Record observed outputs, not assumed success.
5. **Audit selectively:** Request Claude review for significant correctness, architecture, or performance decisions.
6. **Adjudicate:** Evaluate findings with ChatGPT; use experiments to settle disputes.
7. **Record:** Update `STATUS.md` with verified results, decisions, outstanding issues, and next action.

For a quick conceptual question, skip steps that add no value. For a risky kernel change or benchmark claim, use the full cycle.

## Correctness and performance evidence

- Use identical weights, token IDs, and compatible numerical settings when comparing against the Python/Hugging Face reference.
- Compare logits and intermediate activations with justified tolerances; do not require bitwise equality after SIMD or reordered reductions.
- For greedy-decoding divergence, inspect the reference top-1/top-2 logit margin before concluding the implementation is wrong.
- Compare cached Forge decoding against a cached external baseline where appropriate; don't compare cached Forge against uncached Python and call the speedup fair.
- Record workload, compiler/build flags, hardware, threading, warmups, trial count, variability, and precision for performance claims.
- Treat profiler output and measured benchmarks as evidence; do not invent results.

Detailed acceptance criteria and benchmark protocols live in `ROADMAP.md` and take precedence over abbreviated examples here.

## Claude Code repository policy

Create a repository-root `CLAUDE.md` that tells Claude Code to:

- Read `ROADMAP.md` and `STATUS.md` before proposing stage-dependent work.
- Explain planned changes and list files to be modified.
- Ask before editing learning-critical transformer kernels.
- Prefer small, reviewable diffs and targeted tests.
- Report commands actually run, results, and limitations; never claim unexecuted tests passed.
- Avoid unrelated refactors, unrequested dependencies, and premature optimization.
- Preserve documented reference comparisons and benchmark reproducibility.

These are expectations to configure in Claude Code; this document alone does not automatically govern its behavior.

## Communication and efficiency

- ChatGPT should remain approachable and exploratory; rigorous does not mean constantly formal.
- Audits are optional for routine learning and useful for high-impact decisions.
- Do not optimize for agreement between assistants. Optimize for understanding, correctness, reproducibility, and clear engineering tradeoffs.
- If models disagree and no decisive test is practical yet, document the uncertainty and continue with the least risky reversible choice.

**Success criterion:** I can independently explain, implement, test, benchmark, and defend Forge's core operations, regardless of which assistant helped along the way.
