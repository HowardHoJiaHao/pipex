# pipex

> 42 Common Core · Rank 02

Reproduces the behaviour of a shell pipeline using UNIX system calls.

```bash
./pipex infile "cmd1" "cmd2" outfile
# behaves like:  < infile cmd1 | cmd2 > outfile
```

## Key concepts
- Processes with `fork`, `execve` and `waitpid`
- Inter-process communication with `pipe`
- File descriptor redirection with `dup2`
- Resolving commands through the `PATH` environment variable
- Error handling for missing files, permissions and unknown commands

## Usage
```bash
make
./pipex infile "ls -l" "wc -l" outfile
```
