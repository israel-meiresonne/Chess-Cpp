#!/bin/bash

COMPILER="-std=c++20"

run_main() {
  headers_src="-I./src"
  app_src=$(find src -type f -name "*.cpp")
  built_binary="builds/main"

  mkdir -p builds

  echo $headers_src $app_src | xargs clang++ $COMPILER -o $built_binary && ./$built_binary
}

run_tests() {
  test_files=$(_clean_var "$1" "")

  headers_src="-I./src -I./libs/googletest/include -I./libs/googletest"
  gtest_src='./libs/googletest/src/gtest-all.cc'
  built_binary='./builds/main_test'

  app_srcs=$(find src -type f -name "*.cpp" | grep -v 'main.cpp')

  if [[ -z "${test_files}" ]]; then
    test_srcs=$(find tests -type f -name "*.cpp")
  else
    test_srcs=$(
      find tests -type f -name "*.cpp" | grep -E "main.*\.cpp"
      echo "$test_files"
    )
  fi

  mkdir -p builds

  echo "$headers_src" "$gtest_src" "$app_srcs" "$test_srcs" | xargs clang++ $COMPILER -o $built_binary && $built_binary
}

run_formatter() {
  SRC_DIR=$(find src -type f -name "*.[ch]pp")
  TESTS_DIR=$(find tests -type f -name "*.[ch]pp")

  echo "$SRC_DIR $TESTS_DIR" | xargs clang-format -i
}

download_libs() {
  echo "Downloading libs..."

  _download_lib 'https://github.com/google/googletest.git' 'v1.16.x' 'googletest' './googletest/googletest'
}

###############################################################################
# Private functions                                                           #
###############################################################################

_download_lib() {
  REPO_URL="$1"
  BRANCH="$2"
  LIB_NAME="$3"
  REPO_SRC_DIR="$4"

  DEST_DIR="./libs/"

  git clone --branch "$BRANCH" --depth 1 "$REPO_URL" "$LIB_NAME"

  if [ ! -d "$LIB_NAME" ]; then
    echo "Error: Failed to clone the repository."
    return 1
  fi

  mkdir -p "$DEST_DIR"

  cp -r "$REPO_SRC_DIR" "$DEST_DIR"

  if [ ! -d "$DEST_DIR/$LIB_NAME" ]; then
    echo "Error: Failed to download the library."
    return 1
  fi

  rm -rf "$LIB_NAME"

  echo "'$LIB_NAME' has been successfully downloaded into '$DEST_DIR/$LIB_NAME'."
}

_clean_var() {
  var=$(eval "v=($1)" && echo "$v")
  var_default=$2

  test -n "$var" && echo "$var" || echo "$var_default"
}
