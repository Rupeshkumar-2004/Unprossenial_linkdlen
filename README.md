Project: Job Portal (C++ + Crow)

Quick status
- The server entry point is `main_crow.cpp` and uses the Crow microframework with `nlohmann/json`.
- Core logic lives in `job_portal.cpp` and supporting headers in the repo root.
- A simple `Makefile` is included so you can build with `make` and run with `make run`.

How to build and run

```bash
# Build
make

# Run (foreground)
make run
# or
./run.sh
```

API endpoints (implemented)
- GET  /                -> serves `index.html`
# GET  /                -> serves `web/index.html`
- POST /api/jobs        -> post a new job (JSON body)
- GET  /api/jobs        -> list all jobs
- GET  /api/jobs/search?q=... -> search jobs
- POST /api/profile     -> update candidate profile
- GET  /api/recommendations?sessionId=... -> get recommendations

Suggested clean project layout (optional)

If you'd like the repo to look "cleaner" and more conventional, consider moving files into these folders:

- src/        -> `.cpp` files (implementation)
- include/    -> `.h` headers
- web/        -> `index.html`, CSS, JS and other frontend assets
- build/      -> build artifacts (ignored via `.gitignore`)
- backups/    -> any old/backup files you want to keep (not recommended inside repo)

Example reorganization commands (run from project root)

```bash
# create folders
mkdir -p src include web backups

# move sources and headers (adjust paths if you want)
git mv main_crow.cpp src/main_crow.cpp
git mv job_portal.cpp src/job_portal.cpp
git mv Trie.cpp src/Trie.cpp

# move headers
git mv job_portal.h include/job_portal.h
git mv job.h include/job.h
git mv job_portal.h include/job_portal.h
git mv Candidate.h include/Candidate.h
git mv Trie.h include/Trie.h

# move web files
git mv index.html web/index.html

# commit the reorg
git add -A
git commit -m "Reorganize project into src/, include/, web/"
```

Notes / next steps
- I added `.gitignore`, `Makefile`, and this README. If you want, I can also:
  - Apply the reorganization now (I won't move files without your permission).
  - Fix the single C++ warning about assigning a C string literal to a `char*` url param (small change).
  - Add a `backups/` script to snapshot the project before big changes.

Notes about web assets
- The frontend is stored in `web/index.html`. The server now serves that file directly.
- The `Makefile` includes a `prepare-web` target that will copy `web/index.html` to the repo root if you need the legacy behavior (not required since the server reads `web/index.html`).
- Use `./run.sh` to build (if needed) and start the server.

Tell me which of these follow-ups you'd like and I'll do them.
