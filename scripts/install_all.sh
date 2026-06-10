#!/bin/bash

set -e

SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"

echo "Installing LASlib..."
bash "${SCRIPT_DIR}/install_laslib.sh"

echo "Installing PAPI..."
bash "${SCRIPT_DIR}/install_papi.sh"

echo "Installing Picotree..."
bash "${SCRIPT_DIR}/install_picotree.sh"

echo "Installing PCL (this may take several minutes)..."
bash "${SCRIPT_DIR}/install_pcl.sh"

echo "All dependencies installed successfully."
