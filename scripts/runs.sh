#!/bin/bash

COMPILER_VERSION="-std=c++20"
DIR_BUILDS="builds"
BIN_BUILT_APP="$DIR_BUILDS/main"
BIN_BUILT_TESTS="$DIR_BUILDS/main_test"

run_main() {
  headers_hpp="-I./src -I./libs"
  app_src=$(find src -type f -name "*.cpp")

  mkdir -p "$DIR_BUILDS"

  echo "$headers_hpp" "$app_src" | xargs clang++ "$COMPILER_VERSION" -o "$BIN_BUILT_APP" && ./"$BIN_BUILT_APP"
}

run_tests() {
  test_files=$(_clean_var "$1" "")

  headers_hpp="-I./src -I./tests -I./libs/googletest/include -I./libs/googletest"
  gtest_cpp='./libs/googletest/src/gtest-all.cc'

  app_cpp=$(find src -type f -name "*.cpp" | grep -v 'main.cpp')

  if [[ -z "${test_files}" ]]; then
    test_cpp=$(find tests -type f -name "*.cpp")
  else
    test_cpp=$(
      find tests -type f -name "*.cpp" | grep -E "main.*\.cpp"
      echo "$test_files"
    )
  fi

  mkdir -p "$DIR_BUILDS"

  echo "$headers_hpp" "$gtest_cpp" "$app_cpp" "$test_cpp" | xargs clang++ "$COMPILER_VERSION" -o "$BIN_BUILT_TESTS" && "$BIN_BUILT_TESTS"
}

run_formatter() {
  src_dir=$(find src -type f -name "*.[ch]pp")
  tests_dir=$(find tests -type f -name "*.[ch]pp")

  echo "$src_dir $tests_dir" | xargs clang-format -i
}

install_libs() {
  # TODO: Add and remove libs following the config file
  #     : Move libs config to a config file
  #     : Update version stored in libs if different from config file
  echo "Downloading libs..."

  _install_lib 'https://github.com/google/googletest.git' 'v1.16.x' './googletest/googletest'
}

sync_libs() {
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

  dir_local_libs="./libs"
  repo_name=$(_github_repo_name "$repo_url")
  dir_storage_new_lib="$dir_local_libs/$repo_name"

  if [ -d "$dir_storage_new_lib" ]; then
    echo "Library '$repo_name' already exists in '$dir_storage_new_lib'."
    return 0
  fi

  git clone --branch "$branch" --depth 1 "$repo_url" "$repo_name"

  if [ ! -d "$repo_name" ]; then
    echo "Error: Failed to clone the repository."
    return 1
  fi

  if [ ! -d "$dir_local_libs" ]; then
    mkdir -p "$dir_local_libs"
  fi

  cp -r "$dir_repo_src" "$dir_storage_new_lib"

  if [ ! -d "$dir_storage_new_lib" ]; then
    echo "Error: Failed to download the library."
    return 1
  fi

  echo "$branch" >"$dir_storage_new_lib/.version"

  rm -fr "./$repo_name"

  echo "New library '$repo_name' has been successfully downloaded in '$dir_storage_new_lib'."
}

_clean_var() {
  var=$(eval "v=($1)" && echo "$v")
  var_default=$2

  test -n "$var" && echo "$var" || echo "$var_default"
}

_github_repo_name() {
  repo_url="$1"
  echo "$repo_url" | sed -E 's#.+/(.+)\.git#\1#g'
}
