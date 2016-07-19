# Ignoring files from mbed build

The `.mbedignore` file allows you to ignore files and directories from being processed by `mbed build` command.

## Usage
You can place the `.mbedignore` file in any directory where `mbed build` command is going to search for source files.

The most convenient place is the root directory of the library or application. However, this is not a requirement.

Avoid defining rules that would cross the library boundaries as those would lead to side effects or build problems that are hard to find.

## Syntax

Each line in the `.mbedignore` file is a file pattern used for matching files. Each matched file or directory is ignored from build.

The following wildcards are accepted:

|Pattern | Meaning|
|--------|--------|
| `*` | Matches everything. |
| `?` | Matches any single character. |
| `[seq]` | Matches any character in seq. |
| `[!seq]` | Matches any character not in seq. |

File is parsed with Python's [fnmatch](https://docs.python.org/2/library/fnmatch.html) functionality so the syntax follows basic shell patterns with the following exceptions:

1. Each line is internally prefixed with the path of the `.mbedignore` file.
2. Line cannot start with `.` or `/` (because of rule 1)

Globbing functionality is not used, so you cannot recursively match specific file pattern. You need to define rule per directory in that case.

Relative paths can be used, so you can match files deeper in the build tree. However, avoid crossing library boundaries.

### Example
A file located in `source/obsolete/.mbedignore` with following content:

```
*.c
*.h
second_level/*.c
```

After applying the rule 1, actual patterns used internally for matching the source files are:

```
source/obsolete/*.c
source/obsolete/*.h
source/obsolete/second_level/*.c
```
