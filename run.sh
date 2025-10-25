#!/usr/bin/env bash
# Simple launcher for the job portal server
set -euo pipefail

# Build if binary missing
if [ ! -x ./job_portal_server ]; then
  echo "Binary not found, building..."
  make
fi

# Ensure web assets exist
if [ -f web/index.html ]; then
  echo "Serving web/index.html"
else
  echo "Warning: web/index.html not found"
fi

./job_portal_server
