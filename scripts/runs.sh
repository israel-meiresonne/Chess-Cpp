#!/bin/bash

COMPILER_VERSION="-std=c++20"

TRASH='/dev/null'

DIR_BUILDS='builds'
BIN_BUILT_APP="$DIR_BUILDS/main"
BIN_BUILT_TESTS="$DIR_BUILDS/main_test"

DIR_APP='src'
DIR_TESTS='tests'
DIR_LIBS='libs'

DIR_SCRIPTS='scripts'
FILE_LOCKED_LIBS="$DIR_SCRIPTS/libs.lock.json"
FILE_NAME_LIB_VERSION=".version"

DIR_IGNORE='.ignore'
DIR_SCRIPT_LOGS="$DIR_IGNORE/logs"

mkdir -p $DIR_SCRIPTS $DIR_LIBS $DIR_BUILDS $DIR_APP $DIR_TESTS $DIR_IGNORE $DIR_SCRIPT_LOGS

run_main() {
  headers_hpp="-I./$DIR_APP -I./$DIR_LIBS"
  app_cpp=$(find "$DIR_APP" -type f -name "*.cpp")

  lib_headers_hpp=$(_library_app_headers)
  lib_sources_cpp=$(_library_app_sources)
  lib_frameworks=$(_library_app_frameworks)

  mkdir -p "$DIR_BUILDS"

  echo "$headers_hpp" "$lib_headers_hpp" "$lib_sources_cpp" "$lib_frameworks" "$app_cpp" | xargs clang++ "$COMPILER_VERSION" -DTINYORM_USING_QTSQLDRIVERS -fPIC -o "$BIN_BUILT_APP" && ./"$BIN_BUILT_APP"
}

run_tests() {
  test_files=$(_expand_path "$1" "")

  headers_hpp="-I./$DIR_APP -I./$DIR_TESTS -I./$DIR_LIBS"
  app_cpp=$(find "$DIR_APP" -type f -name "*.cpp" | grep -v 'main.cpp')

  if [[ -z "${test_files}" ]]; then
    test_cpp=$(find "$DIR_TESTS" -type f -name "*.cpp")
  else
    test_cpp=$(
      find "$DIR_TESTS" -type f -name "*.cpp" | grep -E "main.*\.cpp"
      echo "$test_files"
    )
  fi

  lib_headers_hpp=$(_library_test_headers)
  lib_sources_cpp=$(_library_test_sources)
  lib_frameworks=$(_library_test_frameworks)

  mkdir -p "$DIR_BUILDS"

  echo "$headers_hpp" "$lib_headers_hpp" "$lib_sources_cpp" "$lib_frameworks" "$app_cpp" "$test_cpp" | xargs clang++ "$COMPILER_VERSION" -o "$BIN_BUILT_TESTS" && "$BIN_BUILT_TESTS"
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
# UP   Public functions                                                       #
# DOWN Main                                                                   #
###############################################################################

###############################################################################
# UP   Main                                                                   #
# DOWN Private functions                                                      #
###############################################################################

_library_app_headers() { _library_files app headers; }
_library_app_sources() { _library_files app sources; }
_library_app_frameworks() { _library_framework_files app; }

_library_test_headers() { _library_files test headers; }
_library_test_sources() { _library_files test sources; }
_library_test_frameworks() { _library_framework_files test; }

_library_framework_files() {
  domain="$1"

  _validate_execution_domain "$domain" || return 1

  framework_paths "$domain" && framework_names "$domain"
}

framework_paths() {
  domain="$1"

  fwk_paths=$(jq -r \
    --arg dir "$DIR_LIBS" \
    --arg domain "$domain" \
    '.[] | .[$domain].frameworks.paths[] | "\($dir)/\(.)"' \
    "$FILE_LOCKED_LIBS"
  )

  if [ -z "$fwk_paths" ]; then return 0; fi

  expanded_frameworks=()
  echo "$fwk_paths" | while read -r fwk_path ; do
      expanded_frameworks+=($(_expand_path "$fwk_path" | tr ' ' '\n' | sort -u | sed -E "s#^#-F./#; s#/\$##"))
  done

  echo "${expanded_frameworks[@]}" | tr '\n' ' '
}

framework_names() {
  domain="$1"

  fwk_names=$(jq -r \
    --arg domain "$domain" \
    '.[] | .[$domain].frameworks.names[]' \
    "$FILE_LOCKED_LIBS"
  )

  if [ -z "$fwk_names" ]; then return 0; fi

  formatted_names=$(echo "$fwk_names" | sed -E "s#^#-framework #")

  echo "$formatted_names" | tr '\n' ' '
}

_library_files() {
  local domain="$1"
  local file_type="$2"

  _validate_execution_domain "$domain" || return 1

  files=$(jq -r \
    --arg dir "$DIR_LIBS" \
    --arg domain "$domain" \
    --arg file_type "$file_type" \
    '.[] | .[$domain][$file_type][] | "\($dir)/\(.)"' \
    "$FILE_LOCKED_LIBS"
  )

  if [ -z "$files" ]; then return 0; fi

  case "$file_type" in
    headers) _format_headers "$files" ;;
    sources) _format_sources "$files" ;;
    *) echo "Unknown file_type: $file_type" >&2; return 1 ;;
  esac
}

_validate_execution_domain() {
  domain="$1"
  if [ "$domain" != "app" ] && [ "$domain" != "test" ]; then
    echo "Error: Execution domain must be 'app' or 'test', instead of '$domain'" >&2
    return 1
  fi
}

_format_headers() {
  headers=$1

  if [ -z "$headers" ]; then return 0; fi

  expanded_headers=()
  echo "$headers" | while read -r header ; do
      expanded_headers+=($(_expand_path "$header" | tr ' ' '\n' | sort -u | sed -E "s#^#-I./#; s#/\$##"))
  done

  echo "${expanded_headers[@]}" | tr '\n' ' '
}

_format_sources() {
  sources=$1

  if [ -z "$sources" ]; then return 0; fi

  expanded_sources=()
  echo "$sources" | while read -r source ; do
      expanded_sources+=($(_expand_path "$source" | tr ' ' '\n' | sort -u | sed -E "s#/\$##"))
  done

  echo "${expanded_sources[@]}" | tr '\n' ' '
}

_install_libraries() {
  [ -d "$DIR_LIBS" ] || mkdir -p "$DIR_LIBS"
  [ -d "$DIR_SCRIPT_LOGS" ] ||  mkdir $DIR_SCRIPT_LOGS

  for lib_i in {0..$((n_libs - 1))}; do
    lib_origin=$(jq -r --argjson i "$lib_i" '.[$i].origin' "$FILE_LOCKED_LIBS")
    lib_target=$(jq -r --argjson i "$lib_i" '.[$i].target' "$FILE_LOCKED_LIBS")
    lib_version=$(jq -r --argjson i "$lib_i" '.[$i].version' "$FILE_LOCKED_LIBS")
    lib_root=$(jq -r --argjson i "$lib_i" '.[$i].root' "$FILE_LOCKED_LIBS")

    if [ "$lib_origin" = 'git' ]; then
      _install_git_library "$lib_target" "$lib_version" "$lib_root"
    elif [ "$lib_origin" = 'local' ]; then
      _install_local_library "$lib_target" "$lib_version" "$lib_root"
    else
      echo "Error: Unknown library origin '$lib_origin'" >&2
    fi
  done
}

_install_local_library() {
  dir_source=$(_expand_path "$1")
  branch="$2"
  dir_repo_root="$3"

  if [ ! -d "$dir_source" ]; then
    echo "Error: Local library source directory is invalid: '$dir_source'" >&2
    return 1
  fi

  lib_name=$(_library_name 'local' "$dir_source")
  dir_new_lib="$DIR_LIBS/$lib_name"

  if [ -d "$dir_new_lib" ]; then
    echo "Library '$lib_name:$branch' already exists in '$dir_new_lib'."
    return 0
  fi

  cp -r "$dir_source" "$lib_name" &> "$DIR_SCRIPT_LOGS/$lib_name.log"

  if [ ! -d "$lib_name" ]; then
    echo "Error: Failed to clone the repository '$lib_name:$branch'"
    return 1
  fi

  git -C "$lib_name" checkout $branch &>> "$DIR_SCRIPT_LOGS/$lib_name.log"

  if [ $? -ne 0 ]; then
    echo "Error: Failed to checkout branch '$branch' in '$lib_name'" >&2
    return 1
  fi

  cp -R "$dir_repo_root" "$dir_new_lib" &>> "$DIR_SCRIPT_LOGS/$lib_name.log"

  if [ ! -d "$dir_new_lib" ]; then
    echo "Error: Failed to download the library"
    return 1
  fi

  echo "$branch" >"$dir_new_lib/$FILE_NAME_LIB_VERSION"

  rm -fr "./$lib_name" &>> "$DIR_SCRIPT_LOGS/$lib_name.log"

  echo "New library '$lib_name:$branch' has been successfully installed in '$dir_new_lib'."
}

_install_git_library() {
  repo_url="$1"
  branch="$2"
  dir_repo_root="$3"

  repo_name=$(_git_repo_name "$repo_url")
  dir_new_lib="$DIR_LIBS/$repo_name"

  if [ -d "$dir_new_lib" ]; then
    echo "Library '$repo_name:$branch' already exists in '$dir_new_lib'."
    return 0
  fi

  git clone --branch "$branch" --depth 1 "$repo_url" "$repo_name" 2>> "$DIR_SCRIPT_LOGS/$repo_name.log"

  if [ ! -d "$repo_name" ]; then
    echo "Error: Failed to clone the repository '$repo_name:$branch'"
    return 1
  fi

  cp -R "$dir_repo_root" "$dir_new_lib"

  if [ ! -d "$dir_new_lib" ]; then
    echo "Error: Failed to download the library"
    return 1
  fi

  echo "$branch" >"$dir_new_lib/$FILE_NAME_LIB_VERSION"

  rm -fr "./$repo_name"

  echo "New library '$repo_name:$branch' has been successfully downloaded in '$dir_new_lib'."
}

_assign_local_libraries() {
  if [ -z "$(ls $DIR_LIBS 2> $TRASH)" ]; then return 0; fi

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
    lib_origin=$(jq -r --argjson i "$lib_i" '.[$i].origin' "$FILE_LOCKED_LIBS")
    lib_target=$(jq -r --argjson i "$lib_i" '.[$i].target' "$FILE_LOCKED_LIBS")
    lib_version=$(jq -r --argjson i "$lib_i" '.[$i].version' "$FILE_LOCKED_LIBS")
    lib_name=$(_library_name "$lib_origin" "$lib_target")
    locked_libraries[$lib_name]=$lib_version
  done
}

_remove_invalid_libraries() {
  for lib_name lib_version in "${(@kv)local_libraries}"; do
    if ([[ -z "${locked_libraries[$lib_name]}" ]]); then
      echo "Library '$lib_name:$lib_version' will be removed."
      rm -fr "$DIR_LIBS/$lib_name"
    elif ([[ "${locked_libraries[$lib_name]}" != "$lib_version" ]]); then
      echo "Library '$lib_name:$lib_version' will be replaced with '$lib_name:${locked_libraries[$lib_name]}'."
      rm -fr "$DIR_LIBS/$lib_name"
    fi
  done
}

_expand_path() {
  var=$(eval "v=($1)" && echo "$v")
  var_default=$2

  test -n "$var" && echo "$var" || echo "$var_default"
}

_library_name() {
  lib_origin=$1
  lib_target=$2

  if [ -z "$lib_origin" ] || [ -z "$lib_target" ]; then
    echo "Error: Library origin or target is not specified" >&2
    return 1
  fi

  if [ "$lib_origin" = "local" ] || [ "$lib_origin" = "git" ]; then
    echo "$(_git_repo_name "$lib_target")"
    return 0
  fi

  echo "Error: Unknown library origin '$lib_origin'" >&2 && return 1
}

_git_repo_name() {
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
