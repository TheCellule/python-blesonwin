call setenv.cmd

"%PY35%\scripts\pip" install --upgrade pip setuptools wheel twine tox
"%PY36%\scripts\pip" install --upgrade pip setuptools wheel twine tox
"%PY36_AMD%\scripts\pip" install --upgrade pip setuptools wheel twine tox
"%PY35_AMD%\scripts\pip" install --upgrade pip setuptools wheel twine tox
