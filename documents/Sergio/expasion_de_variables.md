# Estructura actual del t_token

| Campo	 | Tipo	| Significado|
| --- | --- | --- |
| value	| char *	| Texto literal del token (tal y como lo vio el lexer) |
| type	| t_token_type	| WORD, PIPE, REDIR, etc. |
| quote	| t_token_quote	| NONE, SINGLE, DOUBLE |
| is_expanded	| bool	| Si debe permitir expansión de variables |
| has_quote	| bool	| Si el token contiene alguna comilla (aunque sea interna) |
| next	| struct s_token *	| siguiente token en la lista |

## Concepto de como funciona

El lexer no elimina comillas ni expande variables.
Su trabajo es entender el contexto de cada token y marcar los metadatos.

Después, el expander o ejecutor se encargará de:
- Quitar las comillas.
- Expandir $VAR si procede.
- Concatenar trozos si hay comillas internas.

```bash
Token [ls]
Type: WORD
Quote: NONE
Expand: true
Has Quote: false
```

```bash
ls '-l''a'
Token [ls]             → quote = NONE
Token ['-l''a']         → quote = SINGLE, has_quote = true
```

```bash
Entrada Bash        →        Lexer Tokens              →      Valor semántico real
-----------------------------------------------------------------------------------
ls                  → [ls]                             → ls
ls '                → ERROR (open quote)               → ❌
ls '-la'             → [ls] ['-la']                      → ls -la
ls '-l''a'           → [ls] ['-l''a']                    → ls -la
l''s '-l''a'         → [l''s] ['-l''a']                  → ls -la
-----------------------------------------------------------------------------------
```

## Orden simplificado de fases:

- 1️⃣ Lexing: divides el input en tokens.
- 2️⃣ Parsing: construyes la estructura de comandos (pipes, redirecciones…).
- 3️⃣ Expansión: reemplazas $VAR, ~, $(cmd)… por sus valores reales.
- 4️⃣ Ejecución: ejecutas cada comando con sus argumentos expandidos.

## Ejemplo

```bash
export R="grep R"

ls -la | $R

Lexer:
[ls] [-la] [|] [$R]

Parser:
cmd1: ls -la
pipe
cmd2: $R

Expansion:

$R → grep R
cmd1: ls -la
pipe
cmd2: grep R

```
