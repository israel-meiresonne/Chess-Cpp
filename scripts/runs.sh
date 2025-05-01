#!/bin/bash

COMPILER_VERSION="-std=c++20"

run_main() {
  headers_src="-I./src -I./libs"
  app_src=$(find src -type f -name "*.cpp")
  built_binary="builds/main"

  mkdir -p builds

  echo "$headers_src" "$app_src" | xargs clang++ "$COMPILER_VERSION" -o "$built_binary" && ./"$built_binary"
}

run_tests() {
  test_files=$(_clean_var "$1" "")

  headers_src="-I./src -I./tests -I./libs/googletest/include -I./libs/googletest"
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

  echo "$headers_src" "$gtest_src" "$app_srcs" "$test_srcs" | xargs clang++ "$COMPILER_VERSION" -o "$built_binary" && "$built_binary"
}

run_formatter() {
  src_dir=$(find src -type f -name "*.[ch]pp")
  tests_dir=$(find tests -type f -name "*.[ch]pp")

  echo "$src_dir $tests_dir" | xargs clang-format -i
}

download_libs() {
  echo "Downloading libs..."

  _download_lib 'https://github.com/google/googletest.git' 'v1.16.x' './googletest/googletest'

  _download_lib 'https://github.com/CLIUtils/CLI11.git' 'v2.5.0' './CLI11/include/CLI'
}

###############################################################################
# Private functions                                                           #
###############################################################################

_download_lib() {
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
