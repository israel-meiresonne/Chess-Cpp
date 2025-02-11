#!/bin/bash

run_main() {
  compiler="-std=c++20"
  app_src=$(find src -type f -name "*.cpp")
  built_binary="builds/main"

  mkdir -p builds

  echo $app_src | xargs clang++ $compiler -o $built_binary && ./$built_binary
}

run_tests() {
  compiler="-std=c++20"
  headers_src="-I./src -I./libs/googletest/include -I./libs/googletest"
  gtest_src='./libs/googletest/src/gtest-all.cc'
  app_src=$(find src -type f -name "*.cpp" | grep -v 'main.cpp')
  tests_src=$(find tests -type f -name "*.cpp")
  built_binary='./builds/tests'

  mkdir -p builds

  echo $headers_src $gtest_src $app_src $tests_src | xargs clang++ $compiler -o $built_binary && $built_binary
}

download_libs() {
  echo "Downloading libs..."

  download_lib 'https://github.com/google/googletest.git' 'v1.16.x' 'googletest' './googletest/googletest'
}

download_lib() {
  # Define variables
  REPO_URL="$1"
  BRANCH="$2"
  LIB_NAME="$3"
  REPO_SRC_DIR="$4"

  DEST_DIR="./libs/"

  # Clone the repository and switch to the specified branch
  git clone --branch "$BRANCH" --depth 1 "$REPO_URL" "$LIB_NAME"

  # Check if cloning was successful
  if [ ! -d "$LIB_NAME" ]; then
    echo "Error: Failed to clone the repository."
    return 1
  fi

  # Ensure the destination directory exists
  mkdir -p "$DEST_DIR"

  # Copy the required folder
  cp -r "$REPO_SRC_DIR" "$DEST_DIR"

  # Check if copying was successful
  if [ ! -d "$DEST_DIR/$LIB_NAME" ]; then
    echo "Error: Failed to download the library."
    return 1
  fi

  # Remove the cloned repository
  rm -rf "$LIB_NAME"

  echo "$LIB_NAME has been successfully downloaded into $DEST_DIR."
}
