# pipex

> 42 Common Core · Rank 02

The purpose of this project is to learn how UNIX processes, pipes and file descriptor redirection work by reproducing a shell pipeline in C.

```bash
./pipex infile "cmd1" "cmd2" outfile
```
behaves like:
```bash
< infile cmd1 | cmd2 > outfile
```

## How it works
```
infile ──► cmd1 ──► pipe ──► cmd2 ──► outfile
          (child 1)         (child 2)
```
1. `main` checks that exactly 4 arguments were given.
2. `pipe()` creates a pipe with a read end and a write end.
3. **Child 1** (`fork`): opens `infile`, uses `dup2` to make it the standard input and the pipe's write end the standard output, then runs `cmd1` with `execve`.
4. **Child 2** (`fork`): opens `outfile` (created if missing, emptied if it exists, permissions `0644`), uses `dup2` to make the pipe's read end the standard input and `outfile` the standard output, then runs `cmd2` with `execve`.
5. **Parent**: closes both ends of the pipe, waits for both children with `waitpid`, and exits with `cmd2`'s exit status, just like the shell.

### Finding the command
The command string is split on spaces into the program name and its arguments (`"wc -l"` → `wc`, `-l`).
- If the name contains a `/` (e.g. `/bin/cat`), it is used as given.
- Otherwise each directory in the `PATH` environment variable is tried in order until an executable file is found.

## Error handling
| Situation | What happens | Exit status |
|---|---|:--:|
| Wrong number of arguments | Prints `Error` | `1` |
| `infile` missing or unreadable | Prints the error; `cmd1` does not run, but `cmd2` still runs with empty input (same as the shell) | `cmd2`'s |
| `outfile` cannot be opened | Prints the error | `1` |
| Command not found | Prints `command not found` | `127` if it is `cmd2` |
| Empty command (`""`) | Prints an error | `1` if it is `cmd2` |
| `cmd2` fails | Its own error message | `cmd2`'s |

## Limitations
- Handles exactly two commands; multiple pipes and `here_doc` (the bonus part) are not implemented.
- Arguments are split on spaces only, so quotes are not interpreted: `"grep 'hello world'"` will not work as it does in the shell.

## Functions used
[`open`](https://man7.org/linux/man-pages/man2/open.2.html),
[`close`](https://man7.org/linux/man-pages/man2/close.2.html),
[`write`](https://man7.org/linux/man-pages/man2/write.2.html),
[`pipe`](https://man7.org/linux/man-pages/man2/pipe.2.html),
[`fork`](https://man7.org/linux/man-pages/man2/fork.2.html),
[`dup2`](https://man7.org/linux/man-pages/man2/dup2.2.html),
[`execve`](https://man7.org/linux/man-pages/man2/execve.2.html),
[`access`](https://man7.org/linux/man-pages/man2/access.2.html),
[`waitpid`](https://man7.org/linux/man-pages/man2/waitpid.2.html),
[`perror`](https://man7.org/linux/man-pages/man3/perror.3.html),
[`malloc`](https://man7.org/linux/man-pages/man3/malloc.3.html),
[`free`](https://man7.org/linux/man-pages/man3/free.3.html),
[`exit`](https://man7.org/linux/man-pages/man3/exit.3.html)

## Clone
Clone the repository:
```bash
git clone https://github.com/HowardHoJiaHao/pipex.git
```

## Compile and Run
To compile, `cd` into the cloned directory and:
```bash
make
```

This builds the `pipex` executable. Other targets: `make clean` (remove object files), `make fclean` (also remove `pipex`) and `make re` (rebuild from scratch).

Example:
```bash
printf "hello 42\nworld\nhello again\n" > infile
./pipex infile "grep hello" "wc -l" outfile
cat outfile        # 2
```

To check it against the real shell:
```bash
< infile grep hello | wc -l > outfile_shell
diff outfile outfile_shell && echo "same output"
```
