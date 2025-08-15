#!/bin/bash

DIR_BUILDS="builds"
BIN_BUILT_APP="$DIR_BUILDS/main"
BIN_BUILT_TESTS="$DIR_BUILDS/main_test"

DIR_APP="src"
DIR_TESTS="tests"
DIR_LIBS="libs"

DIR_SCRIPTS="scripts"
FILE_LOCKED_LIBS="$DIR_SCRIPTS/libs.lock.json"
FILE_NAME_LIB_VERSION=".version"

DIR_IGNORE=".ignore"
DIR_SCRIPT_LOGS="$DIR_IGNORE/logs"

COMPILER_VERSION="-std=c++20"

run_main() {
  headers_hpp="-I./$DIR_APP -I./$DIR_LIBS"
  app_cpp=$(find "$DIR_APP" -type f -name "*.cpp")

  lib_headers_hpp=$(_library_app_headers)
  lib_sources_cpp=$(_library_app_sources)

  mkdir -p "$DIR_BUILDS"

  echo "$headers_hpp" "$lib_headers_hpp" "$lib_sources_cpp" "$app_cpp" | xargs clang++ "$COMPILER_VERSION" -o "$BIN_BUILT_APP" && ./"$BIN_BUILT_APP"
}

run_tests() {
  test_files=$(_clean_var "$1" "")

  headers_hpp="-I./$DIR_APP -I./$DIR_TESTS -I./$DIR_LIBS"
  lib_headers_hpp=$(_library_test_headers)
  lib_sources_cpp=$(_library_test_sources)

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

  echo "$headers_hpp" "$lib_headers_hpp" "$lib_sources_cpp" "$app_cpp" "$test_cpp" | xargs clang++ "$COMPILER_VERSION" -o "$BIN_BUILT_TESTS" && "$BIN_BUILT_TESTS"
}

run_formatter() {
  src_dir=$(find "$DIR_APP" -type f -name "*.[ch]pp")
  tests_dir=$(find "$DIR_TESTS" -type f -name "*.[ch]pp")

  echo "$src_dir $tests_dir" | xargs clang-format -i
}

run_install_libraries() {
  if ! [ -z "$(jq empty $FILE_LOCKED_LIBS 2>&1)" ]; then
    echo "Error: '$FILE_LOCKED_LIBS' is invalid" >&2
    return 1
  fi

  declare -A local_libraries && _assign_local_libraries

  declare -A locked_libraries && _assign_locked_libraries

  _remove_invalid_libraries

  _install_libraries
}

###############################################################################
# Private functions                                                           #
###############################################################################

_library_app_headers() {
  jq -r --arg dir "$DIR_LIBS" '.[] | .app.headers[] | "-I./\($dir)/\(.)"' "$FILE_LOCKED_LIBS" | xargs
}

_library_app_sources() {
  jq -r --arg dir "$DIR_LIBS" '.[] | .app.sources[] | "\($dir)/\(.)"' "$FILE_LOCKED_LIBS" | xargs
}

_library_test_headers() {
  jq -r --arg dir "$DIR_LIBS" '.[] | .test.headers[] | "-I./\($dir)/\(.)"' "$FILE_LOCKED_LIBS" | xargs
}

_library_test_sources() {
  jq -r --arg dir "$DIR_LIBS" '.[] | .test.sources[] | "\($dir)/\(.)"' "$FILE_LOCKED_LIBS" | xargs
}

_install_libraries() {
  for lib_i in {0..$((n_libs - 1))}; do
    repo_url=$(jq -r --argjson i "$lib_i" '.[$i].url' "$FILE_LOCKED_LIBS")
    branch=$(jq -r --argjson i "$lib_i" '.[$i].version' "$FILE_LOCKED_LIBS")
    dir_repo_src_root=$(jq -r --argjson i "$lib_i" '.[$i].root' "$FILE_LOCKED_LIBS")

    _install_library "$repo_url" "$branch" "$dir_repo_src_root"
  done
}

_install_library() {
  repo_url="$1"
  branch="$2"
  dir_repo_src_root="$3"

  repo_name=$(_github_repo_name "$repo_url")
  dir_new_lib="$DIR_LIBS/$repo_name"

  if [ -d "$dir_new_lib" ]; then
    echo "Library '$repo_name:$branch' already exists in '$dir_new_lib'."
    return 0
  fi

  [ -d "$DIR_SCRIPT_LOGS" ] ||  mkdir $DIR_SCRIPT_LOGS

  git clone --branch "$branch" --depth 1 "$repo_url" "$repo_name" 2>> "$DIR_SCRIPT_LOGS/$repo_name.log"

  if [ ! -d "$repo_name" ]; then
    echo "Error: Failed to clone the repository '$repo_name:$branch'"
    return 1
  fi

  if [ ! -d "$DIR_LIBS" ]; then
    mkdir -p "$DIR_LIBS"
  fi

  cp -r "$dir_repo_src_root" "$dir_new_lib"

  if [ ! -d "$dir_new_lib" ]; then
    echo "Error: Failed to download the library"
    return 1
  fi

  echo "$branch" >"$dir_new_lib/$FILE_NAME_LIB_VERSION"

  rm -fr "./$repo_name"

  echo "New library '$repo_name:$branch' has been successfully downloaded in '$dir_new_lib'."
}

_assign_local_libraries() {
  for version_file in "$DIR_LIBS"/*/"$FILE_NAME_LIB_VERSION"; do
    [[ -f "$version_file" ]] || continue
    read -r version < "$version_file"
    repo_name=$(basename "$(dirname "$version_file")")
    local_libraries[$repo_name]=$version
  done
}

_assign_locked_libraries() {
  n_libs=$(jq 'length' "$FILE_LOCKED_LIBS")
  for lib_i in {0..$((n_libs - 1))}; do
    lib_url=$(jq -r --argjson i "$lib_i" '.[$i].url' "$FILE_LOCKED_LIBS")
    version=$(jq -r --argjson i "$lib_i" '.[$i].version' "$FILE_LOCKED_LIBS")
    repo_name=$(_github_repo_name "$lib_url")
    locked_libraries[$repo_name]=$version
  done
}

_remove_invalid_libraries() {
  for repo_name version in "${(@kv)local_libraries}"; do
    if ([[ -z "${locked_libraries[$repo_name]}" ]]); then
      echo "Library '$repo_name:$version' will be removed."
      rm -fr "$DIR_LIBS/$repo_name"
    elif ([[ "${locked_libraries[$repo_name]}" != "$version" ]]); then
      echo "Library '$repo_name:$version' will be replaced with '$repo_name:${locked_libraries[$repo_name]}'."
      rm -fr "$DIR_LIBS/$repo_name"
    fi
  done
}

_clean_var() {
  var=$(eval "v=($1)" && echo "$v")
  var_default=$2

  test -n "$var" && echo "$var" || echo "$var_default"
}

_github_repo_name() {
  local repo_url="$1"

  if [ -z "$repo_url" ]; then
    echo "Error: No repository URL provided" >&2
    return 1
  fi

  # Remove trailing slash if present
  repo_url="${repo_url%/}"

  local repo_name
  repo_name="$(basename "$repo_url" .git)"

  echo "$repo_name"
}
