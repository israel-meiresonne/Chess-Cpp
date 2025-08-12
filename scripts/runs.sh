#!/bin/bash

DIR_BUILDS="builds"
DIR_APP="src"
DIR_TESTS="tests"
DIR_LIBS="libs"
BIN_BUILT_APP="$DIR_BUILDS/main"
BIN_BUILT_TESTS="$DIR_BUILDS/main_test"

COMPILER_VERSION="-std=c++20"

run_main() {
  headers_hpp="-I./$DIR_APP -I./$DIR_LIBS"
  app_cpp=$(find "$DIR_APP" -type f -name "*.cpp")

  mkdir -p "$DIR_BUILDS"

  echo "$headers_hpp" "$app_cpp" | xargs clang++ "$COMPILER_VERSION" -o "$BIN_BUILT_APP" && ./"$BIN_BUILT_APP"
}

run_tests() {
  test_files=$(_clean_var "$1" "")

  headers_hpp="-I./$DIR_APP -I./$DIR_TESTS -I./$DIR_LIBS"
  lib_headers_hpp="-I./$DIR_LIBS/googletest/include -I./$DIR_LIBS/googletest"
  gtest_cpp="./$DIR_LIBS/googletest/src/gtest-all.cc"

  app_cpp=$(find "$DIR_APP" -type f -name "*.cpp" | grep -v 'main.cpp')

  if [[ -z "${test_files}" ]]; then
    test_cpp=$(find "$DIR_TESTS" -type f -name "*.cpp")
  else
    test_cpp=$(
      find "$DIR_TESTS" -type f -name "*.cpp" | grep -E "main.*\.cpp"
      echo "$test_files"
    )
  fi

  mkdir -p "$DIR_BUILDS"

  echo "$headers_hpp" "$lib_headers_hpp" "$gtest_cpp" "$app_cpp" "$test_cpp" | xargs clang++ "$COMPILER_VERSION" -o "$BIN_BUILT_TESTS" && "$BIN_BUILT_TESTS"
}

run_formatter() {
  src_dir=$(find "$DIR_APP" -type f -name "*.[ch]pp")
  tests_dir=$(find "$DIR_TESTS" -type f -name "*.[ch]pp")

  echo "$src_dir $tests_dir" | xargs clang-format -i
}

run_install_libs() {
  # TODO: Add and remove libs following the config file
  #     : Move libs config to a config file
  #     : Update version stored in libs if different from config file
  echo "Downloading libs..."

  _install_lib 'https://github.com/google/googletest.git' 'v1.16.x' './googletest/googletest'
}

run_sync_libs() {
  # Add and remove libs following the config file
  echo 'Not implemented'
  return 1
}

###############################################################################
# Private functions                                                           #
###############################################################################

_install_lib() {
  repo_url="$1"
  branch="$2"
  dir_repo_src="$3"

  repo_name=$(_github_repo_name "$repo_url")
  dir_new_lib="$DIR_LIBS/$repo_name"

  if [ -d "$dir_new_lib" ]; then
    echo "Library '$repo_name' already exists in '$dir_new_lib'."
    return 0
  fi

  git clone --branch "$branch" --depth 1 "$repo_url" "$repo_name"

  if [ ! -d "$repo_name" ]; then
    echo "Error: Failed to clone the repository"
    return 1
  fi

  if [ ! -d "$DIR_LIBS" ]; then
    mkdir -p "$DIR_LIBS"
  fi

  cp -r "$dir_repo_src" "$dir_new_lib"

  if [ ! -d "$dir_new_lib" ]; then
    echo "Error: Failed to download the library"
    return 1
  fi

  echo "$branch" >"$dir_new_lib/.version"

  rm -fr "./$repo_name"

  echo "New library '$repo_name' has been successfully downloaded in '$dir_new_lib'."
}

_clean_var() {
  var=$(eval "v=($1)" && echo "$v")
  var_default=$2

  test -n "$var" && echo "$var" || echo "$var_default"
}

_github_repo_name() {
  local repo_url="$1"

  # Ensure argument is provided
  if [ -z "$repo_url" ]; then
    echo "Error: No repository URL provided" >&2
    return 1
  fi

  # Remove trailing slash if present
  repo_url="${repo_url%/}"

  # Extract last path component and strip .git if present
  local repo_name
  repo_name="$(basename "$repo_url" .git)"

  echo "$repo_name"
}
