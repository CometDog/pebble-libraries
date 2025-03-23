#!/bin/bash

# Configuration
GITHUB_USER="CometDog"
REPO_NAME="pebble-libraries"
LIBRARIES_DIR="src/@pebble-libraries"
PACKAGE_FILE="pebble-package.json"
REQUIREMENTS_FILE="requirements.txt"

# Display usage information
show_usage() {
  echo "Pebble Package Manager (PPM)"
  echo "Usage: ./ppm.sh <command>"
  echo ""
  echo "Commands:"
  echo "  install    Install dependencies from pebble-package.json"
  echo "  help       Show this help message"
  echo ""
}

# Check if a dependency is in the package.json
is_dependency_in_package() {
  local dep_name="$1"
  grep -q "\"$dep_name\":" "$PACKAGE_FILE"
  return $?
}

# Check if a command was provided
if [ $# -eq 0 ]; then
  show_usage
  exit 1
fi

# Process commands
case "$1" in
  "install")
    # Create libraries directory if it doesn't exist
    mkdir -p "$LIBRARIES_DIR"

    # Check if package file exists
    if [ ! -f "$PACKAGE_FILE" ]; then
      echo "Error: $PACKAGE_FILE not found"
      exit 1
    fi

    # Parse dependencies from package file
    DEPS=$(grep -o '"[^"]*": "[^"]*"' "$PACKAGE_FILE" | sed 's/"//g' | sed 's/: /:/g')

    # Download each dependency
    for DEP in $DEPS; do
      LIB_NAME=$(echo $DEP | cut -d':' -f1)
      VERSION=$(echo $DEP | cut -d':' -f2)

      echo "Downloading $LIB_NAME version $VERSION"

      # Check if library version exists
      LIB_URL="https://api.github.com/repos/$GITHUB_USER/$REPO_NAME/contents/libs/$LIB_NAME/$VERSION"
      RESPONSE=$(curl -s "$LIB_URL")

      # Response returned Not Found
      if echo "$RESPONSE" | grep -q "Not Found"; then
        echo "Error: Library $LIB_NAME version $VERSION not found in repository"
        exit 1
      fi

      # Get list of files in the directory
      FILES=$(echo "$RESPONSE" | grep "name" | cut -d'"' -f4)

      if [ -z "$FILES" ]; then
        echo "Error: No libs found for $LIB_NAME version $VERSION"
        exit 1
      fi

      # Create directory for the library
      LIB_DIR="$LIBRARIES_DIR/$LIB_NAME"
      mkdir -p "$LIB_DIR"

      # Download library files from GitHub
      GITHUB_URL="https://raw.githubusercontent.com/$GITHUB_USER/$REPO_NAME/main/libs/$LIB_NAME/$VERSION"

      # Check if there's a requirements.txt file
      if echo "$FILES" | grep -q "$REQUIREMENTS_FILE"; then
        echo "  Found requirements file, checking dependencies..."

        # Download requirements file only
        TEMP_REQ=$(mktemp)
        curl -s "$GITHUB_URL/$REQUIREMENTS_FILE" -o "$TEMP_REQ"

        # For each dependency
        MISSING_DEPS=""
        while IFS= read -r REQ_DEP || [ -n "$REQ_DEP" ]; do
          # Skip empty lines and comments
          if [[ -z "$REQ_DEP" || "$REQ_DEP" =~ ^# ]]; then
            continue
          fi

          # Remove quotes and whitespace
          REQ_DEP=$(echo "$REQ_DEP" | sed 's/"//g' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')

          # Check if dependency exists in package.json
          if ! is_dependency_in_package "$REQ_DEP"; then
            MISSING_DEPS="$MISSING_DEPS '$REQ_DEP'"
          fi
        done < "$TEMP_REQ"

        # Check if any dependencies were missing and exit if there are
        if [ -n "$MISSING_DEPS" ]; then
          echo "Error: The following required dependencies for '$LIB_NAME' are not in your pebble-package.json:"
          echo "$MISSING_DEPS"
          echo "Please add them to your dependencies."
          rm "$TEMP_REQ"
          exit 1
        fi

        rm "$TEMP_REQ"
      fi

      # Download all library files
      for FILE in $FILES; do
        # Skip requirements.txt from being copied to the library directory
        if [ "$FILE" == "$REQUIREMENTS_FILE" ]; then
          continue
        fi

        echo "  Downloading $FILE"
        curl -s "$GITHUB_URL/$FILE" -o "$LIB_DIR/$FILE"

        # Check if file was downloaded successfully
        if [ ! -s "$LIB_DIR/$FILE" ]; then
          echo "Error: Failed to download $FILE"
          exit 1
        fi
      done
    done

    echo "All dependencies downloaded successfully!"
    ;;

  "help")
    show_usage
    ;;

  *)
    echo "Error: Unknown command '$1'"
    show_usage
    exit 1
    ;;
esac

exit 0
