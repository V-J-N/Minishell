
1. Configuración inicial del repositorio (Owner)

mkdir minishell && cd minishell
git init

echo "# Minishell Proyecto 42" > README.md

# Ignorar archivos binarios/temporales
cat > .gitignore << EOF
minishell
*.o
*.a
*~
*.swp
.DS_Store
obj/
.vscode/
*.dSYM/
EOF

# Crear estructura básica

git add .
git commit -m "Initial commit: "

# Crear repo en GitHub y conectar
git remote add origin https://github.com/TU_USUARIO/minishell.git
git branch -M main
git push -u origin main

Luego, en GitHub:

Invita a tu compañero como collaborator.

Protege la rama main (desde Settings → Branches):

No permitir pushes directos.

Requiere PRs para fusionar.

2. Flujo de trabajo con ramas: simple y funcional

Usa este modelo ligero, ideal para 2 personas:

main: código estable y funcional. Solo se toca con PRs.

feature/<nombre>: ramas para trabajar cada funcionalidad (por ej: feature/parser, feature/executor).

3. ¿Cómo trabaja cada uno diariamente?

🔹 Al iniciar el día

git checkout main
git pull origin main

🔹 Crear nueva rama para una funcionalidad

git checkout -b feature/parser

Trabaja ahí. Haz commits frecuentes y descriptivos:

git add .
git commit -m "feat(parser): añadir manejo de comillas"

Sube tus avances:

git push -u origin feature/parser

4. Pull Requests y revisiones

Cuando termines una funcionalidad:

Asegúrate de actualizar tu rama:

git pull origin main
git merge main

Crea un Pull Request en GitHub: de tu rama feature/* hacia main.

Tu compañero lo revisa:

Lee el código.

Compila, prueba.

Comenta si hay mejoras.

Una vez aprobado, merge desde GitHub.

5. Después del merge

Borrar la rama feature/*:

git branch -d feature/parser         # local
git push origin --delete feature/parser  # remoto

6. Resolución de conflictos (si ocurre)

Cuando Git no puede fusionar cambios automáticamente:
git status  # ver archivos en conflicto
# Editar archivos, buscar <<<<<<< ======= >>>>>>>
git add archivo_resuelto
git merge --continue  # o git rebase --continue si estabas rebaseando

Consejo: Antes de operaciones complejas, crea una rama backup:

git checkout -b backup-2025-08-19

Recomendaciones clave

No hacer push a main directamente.

Siempre trabajar en ramas feature/*.

Hacer commits pequeños y con sentido.

Revisar PRs de tu compañero antes de fusionar.

| Acción                   |Comando                                                    |
| ------------------------ | ---------------------------------------------------------- |
| Clonar repo              | `git clone <url>`                                          |
| Cambiar rama             | `git checkout <rama>`                                      |
| Crear rama               | `git checkout -b feature/nombre`                           |
| Subir rama               | `git push -u origin feature/nombre`                        |
| Fusionar cambios de main | `git pull origin main && git merge main`                   |
| Resolver conflictos      | `git status`, editar, `git add`, `git merge --continue`    |
| Crear PR                 | Desde GitHub, entre `feature/*` y `main`                   |
| Borrar rama              | `git branch -d <rama>` y `git push origin --delete <rama>` |


Opción simple y efectiva para dos personas:

Solo usar main y ramas feature/*.

Hacer Pull Requests desde feature/* a main.

Revisar PR mutuamente antes de fusionar.

Evitar develop y ramas release (innecesarias para este tamaño).