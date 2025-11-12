# Minishell 42 - serjimen & vjan-nie

A minimalist command interpreter developed as a 42 school project.

## Description

Minishell is a simplified implementation of a POSIX shell that replicates the basic functionalities of Bash. It includes:

- **Lexer**: Input tokenization with quote handling
- **Parser**: Syntax analysis and command tree construction
- **Executor**: Command execution with pipe and redirection support
- **Builtins**: Internal commands (cd, export, unset, echo, exit, pwd, env)
- **Expander**: Environment variable expansion
- **Redirections**: Standard input/output and heredocs

## Requirements

- GCC or Clang
- GNU Make
- Readline library (`libreadline-dev` on Debian/Ubuntu)

## Compilation

```bash
make
```

To clean object files:

```bash
make clean
```

To remove everything (including the executable):

```bash
make fclean
```

To rebuild from scratch:

```bash
make re
```

## Usage

```bash
./minishell
```

Once started, you can execute commands like in a normal shell:

```bash
minishell> echo "Hello World"
minishell> ls -la
minishell> cat file.txt | grep word > output.txt
minishell> exit
```

## Project Structure

```
src/
├── core/          # Main core (signals, initialization)
├── lexer/         # Lexical analysis (tokenization)
├── parser/        # Syntax analysis
├── executor/      # Command execution
├── builtins/      # Internal commands
├── expander/      # Variable expansion
├── env/           # Environment management
└── redirection/   # Redirection and heredoc handling
```

## Authors

- **serjimen** - Servidor de Schutter
- **vjan-nie** - Victor Jimenez

## Notes

This project implements a functional but simplified shell. It does not include all Bash features, focusing on the fundamental logic of a command interpreter.