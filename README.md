# nushell

## Description

`nushell` is a UNIX command line interpreter that replicates core
functionalities of the original Bourne Shell `sh`.

## Usage

To use nushell, you have first to clone the project:
```shell $ git clone
https://github.com/iamkhalil/nushell.git
```
Then you've to build it:
```shell
$ cd nushell
$ make all
```
You can now use `nushell` either in an `interactive
mode` where the shell reads commands from user input on a tty or in a
`non-interactive mode` via a script or by passing input to `nushell` via a
pipe.

You can also use `make` as an interface to run nushell using Valgrind or to run
the test suites.
```shell
$ make help
List of available options:
 * all - build the project
 * test - run the test suites
 * memcheck - check memory leaks using Valgrind
 * clean - delete build files
```

## Features

Besides basic functionalities including syscalls, nushell does not depend
heavily on the standard library, instead we've created our own custom
functions. As an example, we have created our own `getline` and `strtok` to
handle and process user input.

In addition to supporting external programs and handling both absolute and
relative PATHS, `nushell` has support also for the following features:

| Builtin commands | Functionality                                        |
| --------         | -----------                                          |
| env              | Print a list of the environment variables            |
| setenv           | Change or add an environment variable                |
| unsetenv         | Remove an environment variable                       |
| cd               | Change the working directory                         |
| history          | Prints command history                               |
| exit [status]    | Cause the shell to exit with an optional exit status |
| help             | Display information about builtin commands           |


| Other    | Functionality        |
| -------- | ----------           |
| #        | Comment indicator    |
| ;        | Commands separator   |
| $$       | Current process ID   |
| $?       | Exit code value      |
| &&       | AND logical operator |
| ||       | OR logical operator     |
| CTRL+D   | Exit the shell (EOF)    |
| CTRL+C   | Does not exit the shell |


## Examples

#### Interactive shell

![interactive]()

#### Non-interactive shell

![non-interactive-script]()

![non-interactive-pipe]()

## Author
- **Mohamed khalil El-Bechir** - [medkhalilelb@gmail.com](medkhalilelb@gmail.com) | [GitHub/iamkhalil](https://github.com/iamkhalil)
## Licence
MIT License
