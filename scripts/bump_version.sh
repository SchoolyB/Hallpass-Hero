#!/bin/bash

# Path to the JSON file
json_file="./version.json"

# !ONLY use this in locally
# json_file="../version.json" 

# Check if the JSON file exists
if [ ! -f "$json_file" ]; then
  echo "Version JSON file not found: $json_file"
  exit 1
fi

# Extract the current version numbers
current_version=$(jq -r .version "$json_file")

# Function to increment version numbers
increment_version() {
  local version=$1
  local updated_version=""
  local patch=""
  if [[ $version =~ ([0-9]+)$ ]]; then
    patch="${BASH_REMATCH[1]}"
    updated_patch=$((patch + 1))
    updated_version="${version%$patch}$updated_patch"
    echo "$updated_version"
  else
    echo "Invalid version format: $version"
  fi
}

# Increment the patch numbers
patch_build=$(increment_version "$current_version")


# Update the JSON file with the new version numbers
jq ".version = \"$patch_build\"" "$json_file" > "$json_file.tmp"
mv "$json_file.tmp" "$json_file"

echo "Bump Hallpass Hero to $patch_build"