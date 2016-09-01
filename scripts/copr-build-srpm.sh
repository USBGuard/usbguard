#!/bin/bash

CURL=$(which curl)
JQ=$(which jq)

if [ -z "$CURL" ]; then
  echo "Required curl command missing. Aborting."
  exit 1
fi
if [ -z "$JQ" ]; then
  echo "Required jq command missing. Aborting."
  exit 1
fi

COPR_URL_API2="https://copr.fedorainfracloud.org/api_2/"

COPR_LOGIN="${COPR_LOGIN:=invalidlogin}"
COPR_TOKEN="${COPR_TOKEN:=invalidtoken}"
COPR_PROJECT_ID="${COPR_PROJECT_ID:=0}"
SRPM="$1"

if [ -z "$SRPM" ]; then
  echo "Usage: $(basename $0) <srpm>"
  exit 1
fi

echo "[+] Fetching project chroots..."
#
# Fetch project chroots
#
COPR_PROJECT_CHROOTS=$(
  $CURL -o - -s "https://copr.fedorainfracloud.org/api_2/projects/${COPR_PROJECT_ID}/chroots" | \
    $JQ -c '[ .chroots | .[] | .chroot | .name ]'
)

if [ $? -ne 0 ]; then
  echo "[!] FAILED: Can't retrieve list of project chroots. Aborting."
  exit 1
fi

COPR_BUILD_METADATA="{ \"project_id\": ${COPR_PROJECT_ID}, \"chroots\": $COPR_PROJECT_CHROOTS, \"enable_net\": false }"

echo "[+] Submitting SRPM for build: $SRPM"
#
# Upload and build the srpm
#
COPR_RESPONSE=$($CURL \
  -s \
  --basic -u $COPR_LOGIN:$COPR_TOKEN \
  -F "metadata=$COPR_BUILD_METADATA" \
  -F "srpm=@$SRPM;type=application/x-rpm" \
  -i \
  -o - \
  "$COPR_URL_API2/builds")

if [ $? -ne 0 ]; then
  echo "[!] FAILED: Unable to submit the srpm. Server responded:"
  echo "$COPR_RESPONSE"
  exit 1
else
  echo "[+] Build successfully submitted. Server responded:"
  echo "$COPR_RESPONSE"
fi
