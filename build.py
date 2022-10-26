#!/usr/bin/python

## Imports
import argparse
import datetime
from io import StringIO
import os
import re
import shlex
import shutil
import subprocess
import sys
import tarfile
import time
from functools import reduce
from typing import Any

import colorama
import pygments
import pygments.formatters
import pygments.lexers

## Constants
HEADER_SEPARATOR = "_" * 20
GCC_PATH = "/usr/bin/gcc"
DEFAULT_ARGUMENTS = ["-Wall", "-Wextra", "-Werror", "-Wpedantic", "-O", "-g"]
IGNORE_FOLDERS = [".git", ".vscode", "build", "venv", "__pycache__", "mp2i-pv"]
DEFAULT_PACKAGE_NAME = "Alexis_Rossfelder_MP2I_{}.tgz"
TP_FOLDER = "/home/alexi/Documents/MP2I/INFO/mp2i-pv/docs/TP2022-2023"
DEFAULT_CWD = "/home/alexi/Documents/MP2I/INFO"
BUILD_DIR = os.path.join(DEFAULT_CWD, "build") + os.sep
UPDATE_COMMAND = ["git", "pull", "origin", "main", "--rebase"]
EMPTY_TP = 3
VSCODE_PATH = "/usr/bin/code"


## Global variables
_silent = False
_debug = False


## Logging functions
_print = print


def print(*args, **kwargs):
    if not _silent:
        _print(*args, **kwargs)


def red(*args, end="\n", **kwargs):
    print(colorama.Fore.RED, end="")
    print(*args, end="", **kwargs)
    print(colorama.Fore.RESET, end=end)


def green(*args, end="\n", **kwargs):
    print(colorama.Fore.GREEN, end="")
    print(*args, end="", **kwargs)
    print(colorama.Fore.RESET, end=end)


def gray(*args, end="\n", **kwargs):
    print(colorama.Fore.LIGHTBLACK_EX, end="")
    print(*args, end="", **kwargs)
    print(colorama.Fore.RESET, end=end)


def verbose(*args, level=1, **kwargs):
    levels = {
        1: gray,
        2: green,
        3: red,
        0: print,
    }
    if _debug:
        levels[level](*args, **kwargs)


def color(*args, color, end="\n", format: bool = False, **kwargs):
    buffer = StringIO() if format else None
    print(color, end="", file=buffer)
    print(*args, **kwargs, end="", file=buffer)
    print(colorama.Style.RESET_ALL, end=end, file=buffer)
    if format and buffer is not None:
        return buffer.getvalue()


def print_code(code: str, language: str) -> None:
    print(f"\n ---------  {language} code  --------- ")
    print(
        pygments.highlight(
            code,
            pygments.lexers.get_lexer_by_name(language),
            pygments.formatters.Terminal256Formatter(),
        ),
    )
    print(f" ---------  {len(language +' code') * '-'}  --------- \n")


def latex_format(string: str) -> str:
    """Format a latex string to be printed in the terminal"""
    # fmt: off
    subscript = "₀₁₂₃₄₅₆₇₈₉"
    latex_chars = {r"\forall": "∀",r"\exists": "∃",r"\in": "∈",r"\notin": "∉",r"\subset": "⊂",r"\subseteq": "⊆",r"\supset": "⊃",r"\supseteq": "⊇",r"\emptyset": "∅",r"\cap": "∩",r"\cup": "∪",r"\infty": "∞",r"\leq": "≤",r"\geq": "≥",r"\neq": "≠",r"\equiv": "≡",r"\approx": "≈",r"\pm": "±",r"\times": "×",r"\div": "÷",r"\cdot": "·",r"\circ": "∘",r"\angle": "∠",r"\nabla": "∇",r"\partial": "∂",r"\sum": "∑",r"\prod": "∏",r"\int": "∫",r"\to": "→",r"\gets": "←",r"\leftrightarrow": "↔",r"\leftarrow": "←",r"\rightarrow": "→",r"\Rightarrow": "⇒",r"\Leftarrow": "⇐",r"\Leftrightarrow": "⇔",r"\mapsto": "↦",r"\longmapsto": "⟼",r"\varnothing": "∅",r"\alpha": "α",r"\beta": "β",r"\gamma": "γ",r"\delta": "δ",r"\epsilon": "ε",r"\zeta": "ζ",r"\eta": "η",r"\theta": "θ",r"\iota": "ι",r"\kappa": "κ",r"\lambda": "λ",r"\mu": "μ",r"\nu": "ν",r"\xi": "ξ",r"\omicron": "ο",r"\pi": "π",r"\rho": "ρ",r"\sigma": "σ",r"\tau": "τ",r"\upsilon": "υ",r"\phi": "φ",r"\chi": "χ",r"\psi": "ψ",r"\omega": "ω",r"\Alpha": "Α",r"\Beta": "Β",r"\Gamma": "Γ",r"\Delta": "Δ",r"\Epsilon": "Ε",r"\Zeta": "Ζ",r"\Eta": "Η",r"\Theta": "Θ",r"\Iota": "Ι",r"\Kappa": "Κ",r"\Lambda": "Λ",r"\Mu": "Μ",r"\Nu": "Ν",r"\Xi": "Ξ",r"\Omicron": "Ο",r"\Pi": "Π",r"\Rho": "Ρ",r"\Sigma": "Σ",r"\Tau": "Τ",r"\Upsilon": "Υ",r"\Phi": "Φ",r"\Chi": "Χ",r"\Psi": "Ψ",r"\Omega": "Ω",}
    functions = {r"\sqrt": "√",r"\frac": "÷",r"\sin": "sin",r"\cos": "cos",r"\tan": "tan",r"\arcsin": "arcsin",r"\arccos": "arccos",r"\arctan": "arctan",r"\sinh": "sinh",r"\cosh": "cosh",r"\tanh": "tanh",r"\ln": "ln",r"\log": "log",r"\exp": "exp"}
    superscript = "⁰¹²³⁴⁵⁶⁷⁸⁹"
    other_superscripts = {r"\prime": "′",r"\prime\prime": "″",r"^e": "ᵉ",r"^i": "ⁱ",r"^j": "ʲ",r"^k": "ᵏ"}
    # fmt: on
    for latex, char in latex_chars.items():
        string = string.replace(latex, char)
    for latex, char in functions.items():
        string = string.replace(latex, char)
    for i, char in enumerate(subscript):
        string = string.replace(f"_{i}", char)
    for i, char in enumerate(superscript):
        string = string.replace(f"^{i}", char)
    for latex, char in other_superscripts.items():
        string = string.replace(latex, char)

    return string


def clear_screen() -> None:
    """Clear the terminal screen"""
    os.system("cls" if os.name == "nt" else "clear")


def print_exercises(ex_num: str, ex_title: str, ex_lines: list[str]) -> None:
    color(
        f"\t Exercise {ex_num} - {ex_title} ",
        color=colorama.Back.BLACK + colorama.Style.BRIGHT,
    )
    print()
    code_lines = list()
    code_language = ""
    for line in ex_lines:
        if line.startswith("```"):
            if code_language == "":
                code_language = line[3:-1]
            else:
                print_code("".join(code_lines), code_language)
                code_language = ""
                code_lines.clear()
            continue
        if code_language != "":
            code_lines.append(line)
            continue

        if "$$" in line:  # LaTeX
            # Highlight the latex code in green
            while "$$" in line:
                line = line.replace("$$", colorama.Fore.GREEN, 1).replace(
                    "$$", colorama.Fore.RESET, 1
                )
            line = latex_format(line)  # Format the latex code
        if re.match(r"^\d+\.", line):  # Numbered list
            line = color(line, end="", color=colorama.Style.BRIGHT, format=True)
            if line is None:
                line = ""

        if re.match(r"^\[.+\]\(.+\)$", line):
            line = color(
                "File:",
                line[1 : line.index("]")],
                end="",
                color=colorama.Fore.CYAN,
                format=True,
            )
            if line is None:
                line = ""
        # Gray background for the code inline
        if re.match(r"^.*\`.*\`", line):
            line = re.sub(
                r"`(.*?)`",
                colorama.Back.BLACK + r"\1" + colorama.Back.RESET,
                line,
                16,  # Max 16 code inline per line
            )

        if "$" in line:  # LaTeX
            # Highlight the latex code in green
            while "$" in line:
                line = line.replace("$", colorama.Fore.GREEN, 1).replace(
                    "$", colorama.Fore.RESET, 1
                )
            line = latex_format(line)  # Format the latex code
        print(line, end="")


## IDE functions
def open_ide(tp_path: str, file: str) -> None:
    # Open vscode
    if not os.path.exists(VSCODE_PATH):
        red(f"VSCode not found at {VSCODE_PATH}")
        exit(1)
    subprocess.run([VSCODE_PATH, tp_path, file])


def open_file_from_ex(
    exercises_group: list[tuple[str, str, list[str]]], ex_num: str, tp_source: str
) -> str:
    # Each exercise is a tuple (ex_num, ex_title, ex_content) where ex_content is a list of lines

    for ex in exercises_group:
        if str(ex[0]) == str(ex_num):
            for line in ex[2]:
                if x := re.match(r"\[.*(?:\.c|\.h)\]\((.*)\)", line):
                    if not os.path.exists(os.path.basename(x.group(1))):
                        shutil.copyfile(
                            os.path.join(tp_source, x.group(1)),
                            os.path.basename(x.group(1)),
                        )  # Copy the file to the current folder (we chdir to the good folder earlier)
                    open_ide(".", os.path.basename(x.group(1)))
                    return os.path.basename(
                        x.group(1)
                    )  # return the file that was opened
    return ""


## TP folder automations functions
def update_tp(exit_on_error=True):
    verbose("Updating TP...")
    code = subprocess.run(UPDATE_COMMAND, cwd=TP_FOLDER).returncode
    if code != 0:
        red(f"Failed to update TP ! (code: {code})")
        if exit_on_error:
            exit(code)
    else:
        verbose("TP updated !", level=2)


def get_tp_list(path: str) -> dict[int, str]:
    lst = [
        os.path.join(TP_FOLDER, p)
        for p in os.listdir(path) + [""] * EMPTY_TP
        if "TP" in p
    ]
    result = dict()
    not_found = list()
    for tp in lst:
        source = ""
        if os.path.exists(os.path.join(tp, os.path.basename(tp).lower() + ".md")):
            source = os.path.join(tp, os.path.basename(tp).lower() + ".md")
        else:
            files = [f for f in os.listdir(tp) if f.lower().endswith(".md")]
            if len(files) == 1:
                source = os.path.join(tp, files[0])
            else:
                verbose(
                    f"Unable to get the source of {os.path.basename(tp)} automatically",
                    level=3,
                )
        if source != "":
            if x := re.match(r"\# TP(\d+) :", open(source).readline()):
                result[int(x.group(1))] = tp
            else:
                not_found.append(tp)
        else:
            not_found.append(tp)

    for i, tp in enumerate(not_found):
        result[len(result) + min(result)] = tp
    return result


def select_tp(path: str) -> tuple[str, int]:
    tp_dict = get_tp_list(path)

    # TP selection
    print("Please select the file to import :")
    i = 0
    for i in sorted(tp_dict.keys()):
        print(f"\t{i} - {os.path.basename(tp_dict[i]).replace('_', ' ')}")
    try:
        result = input(f"Select one [{i}]: ")
        if result == "":
            result = i  # Default value
        else:
            result = int(result)
        assert result in tp_dict
    except (KeyboardInterrupt, ValueError, AssertionError):
        print("\nAborting ...")
        return "", -1

    # get the md file
    p = tp_dict[result]  # tp folder by number

    if os.path.exists(os.path.join(p, os.path.basename(p).lower() + ".md")):
        return (
            os.path.join(p, os.path.basename(p).lower() + ".md"),
            result,
        )  # Use the default file if it exists
    else:
        files = [f for f in os.listdir(p) if f.lower().endswith(".md")]
        if len(files) == 1:
            return os.path.join(p, files[0]), result  # Use the only md file
        elif len(files) > 1:  # Ask the user to select the file
            print("Please select the file to use :")
            for i, t in enumerate(files):
                print(f"\t{i + 1} - {os.path.basename(t)}")
            try:
                result = int(input("Select one : "))
                assert result - 1 in range(0, len(files))
            except (KeyboardInterrupt, ValueError, AssertionError):
                print("\nAborting ...")
                return "", -1
            return os.path.join(p, files[result - 1]), result
        else:  # No md file found
            print("No MD file associated with this TP.")
            return "", -1


def read_md_tp(path: str) -> dict[str, list[str]]:
    # Read the file and get the exercises associated with each part
    result = {"Introduction": []}
    current_part = "Introduction"
    with open(path, "r") as f:
        for line in f:
            if x := re.match(r"\#\# (.*) -- ", line):
                current_part = x.group(1)
                result[current_part] = []
            elif x := re.match(r"\[.*(?:\.c|\.h)\]\((.*)\)", line):
                result[current_part].append(x.group(1))
    for k, v in list(result.items()):
        if len(v) == 0:
            del result[k]
    return result


def read_md_tp_ex(path: str) -> dict[str, list[tuple[str, str, list[str]]]]:
    # Read the file and get the exercises associated with each part
    result = {"Introduction": []}
    current_part = "Introduction"
    saving = False
    current_ex = 0
    current_title = ""
    current_content_lines = []
    with open(path, "r") as f:
        line = ""
        for line in f:
            if x := re.match(r"\#\# (.*) -- ", line):
                if saving:
                    result[current_part].append(
                        (current_ex, current_title, current_content_lines.copy())
                    )
                    current_content_lines.clear()

                current_part = x.group(1)
                result[current_part] = []
                saving = False
            elif x := re.match(r"\#\#\# Exercice (\d+) : (.*)", line):
                if saving:
                    result[current_part].append(
                        (current_ex, current_title, current_content_lines.copy())
                    )
                    current_content_lines.clear()

                current_ex = x.group(1)
                current_title = x.group(2).strip()
                saving = True
            elif saving:
                current_content_lines.append(line)
            elif current_ex != 0:
                result[current_part].append(
                    (current_ex, current_title, current_content_lines.copy())
                )
                current_content_lines.clear()

        if saving:
            result[current_part].append(
                (current_ex, current_title, current_content_lines)
            )

    for k, v in list(result.items()):
        if len(v) == 0:
            del result[k]
    return result


def prepare_folder(
    tp_path: str, files: list[str], tp_num: int, dest_folder: str
) -> None:
    tp_folder = f"{dest_folder}{os.sep}TP{tp_num:02d}"
    if os.path.exists(tp_folder):
        verbose(f"Did not create {tp_folder} : Already exists")
    else:
        os.mkdir(tp_folder)
        verbose(f"Created {tp_folder}")
    for f in files:
        if os.path.exists(os.path.join(tp_folder, os.path.basename(f))):
            verbose(f"Did not copy {f} : Already exists")
        else:
            shutil.copy(os.path.join(os.path.dirname(tp_path), f), tp_folder)
            verbose(f"Copied {f} to {tp_folder}")


def select_categories(tp_dict: dict[str, list[Any]]) -> tuple[list[Any], list[int]]:
    # read the md file
    print("Please select the categories to import :")
    i = -2
    for i, k in enumerate(tp_dict.keys()):
        print(f"\t{i + 1} - {k}")
    try:
        result = [
            int(x)
            for x in input(
                f"Select one or more (separated by spaces) [{i + 1}]: "
            ).split()
        ]
        assert all([x - 1 in range(0, len(tp_dict)) for x in result])
        if len(result) == 0:
            result = [len(tp_dict)]
    except (KeyboardInterrupt, ValueError, AssertionError):
        print("\nAborting ...")
        return [], []
    return (
        reduce(
            lambda x, y: x + y, [tp_dict[list(tp_dict.keys())[i - 1]] for i in result]
        ),
        result,
    )


def select_files(tp_path):
    return select_categories(read_md_tp(tp_path))[0]


## TP coding related functions
def main_init(tp_folder: str, dest_folder: str) -> None:
    tp_path, tp_num = select_tp(tp_folder)
    if tp_path == "":
        return
    files = select_files(tp_path)
    prepare_folder(tp_path, files, tp_num, dest_folder)


## C files related functions
class CFileSingleton(type):
    _instances = {}

    def __call__(cls, file, *args, **kwargs):
        if (cls, os.path.abspath(file)) not in cls._instances:
            cls._instances[(cls, os.path.abspath(file))] = super(
                CFileSingleton, cls
            ).__call__(file, *args, **kwargs)
        return cls._instances[(cls, os.path.abspath(file))]


class CFile(metaclass=CFileSingleton):
    def __init__(self, filename: str, out=None, base=None):

        # Filename as absolute path
        self.filename = os.path.abspath(filename)
        if base is None:
            self.basename = os.path.splitext(os.path.basename(self.filename))[0]

        else:
            self.basename = base

        if out is None:
            if not os.path.exists(BUILD_DIR):
                os.mkdir(BUILD_DIR)
            if not os.path.isdir(BUILD_DIR):
                raise FileExistsError("build is not a directory")
            out = os.path.join(
                BUILD_DIR, os.path.splitext(os.path.basename(filename))[0]
            )
        elif os.path.isdir(out):
            out = os.path.join(out, os.path.splitext(os.path.basename(filename))[0])

        self.out = out
        self._provided_name = filename

        self.reload()

    def __str__(self) -> str:
        return self.filename

    def reload(self):
        self._parsed = False
        self._built = False
        self._valid = True

        self._files = []
        self._args = []
        self._libs = []
        self._package = []
        self._authors = ["+Alexis Rossfelder, MP2I"]
        self._run_args = []

        if not os.path.exists(self._provided_name):
            self._valid = False
            verbose(f"File {self._provided_name} does not exist", level=3)

        # Check if the file is a c file
        elif not os.path.isfile(self._provided_name):
            self._valid = False
            verbose(f"File {self._provided_name} is not a text file", level=3)
        elif not self._provided_name.endswith(".c"):
            self._valid = False
            verbose(f"File {self._provided_name} is not a c file", level=3)

        if self._valid:
            self._args.extend(DEFAULT_ARGUMENTS)
            self._files.append("+" + self._provided_name)
            verbose(f"Found {self}")

    def _get_settings(self):
        if not self._valid:
            return []
        with open(self.filename, "r") as f:
            for line_num, line in enumerate(f):
                current_tag = "files:"
                line = line.strip()
                if line.startswith("//#"):  # Ignore commented includes
                    if line.startswith("//#include ") or line.startswith("//#define"):
                        continue
                    if line.startswith("//#ignore#"):
                        self._valid = False
                        verbose("Ignoring", self)
                        return []
                    line = iter(line.replace("//#", "").strip().split())
                    for word in line:
                        if word.startswith('"'):
                            word = word[1:]
                            while not word.endswith('"'):
                                try:
                                    word += " " + next(line)
                                except StopIteration:
                                    print(
                                        f"SyntaxError: '\"' was not closed at {self.basename}:{line_num}"
                                    )
                                    word += '"'
                                    break
                            word = word[:-1]
                        if word in (
                            "files:",
                            "args:",
                            "libs:",
                            "author:",
                            "package:",
                            "run:",
                        ):
                            current_tag = word
                        else:
                            yield word, current_tag

    def _parse(self):
        if self._parsed or not self._valid:
            return

        tag_list = {
            "files:": self._files,
            "args:": self._args,
            "libs:": self._libs,
            "author:": self._authors,
            "package:": self._package,
            "run:": self._run_args,
        }
        for argument, tag in self._get_settings():
            if not argument.startswith(("+", "^", "=")):
                argument = "+" + argument
            tag_list[tag].append(argument)
        if not self._valid:
            return
        self._parsed = True
        verbose(f"Parsed {self}")
        if self._files != ["+" + self._provided_name]:
            verbose(f"Files: {' '.join(self._files)}")
        if self._args != DEFAULT_ARGUMENTS:
            verbose(f"Args: {' '.join(self._args)}")
        if self._libs:
            verbose(f"Libs: {' '.join(self._libs)}")
        if self._authors != ["+Alexis Rossfelder, MP2I"]:
            verbose(f"Authors: {' '.join(self._authors)}")
        if self._package:
            verbose(f"Package: {' '.join(self._package)}")
        if self._run_args:
            verbose(f"Run: {' '.join(self._run_args)}")

    def _insert_header(self, author: list[str], command: list, date: bool):
        if not self._valid:
            return
        with open(self.filename, "r", encoding="utf-8") as f:
            lines = f.readlines()

        with open(self.filename, "w", newline="", encoding="utf-8") as f:
            header_sep_start = -1
            header_sep_end = -1
            for i, line in enumerate(lines):
                if line.startswith(HEADER_SEPARATOR) and header_sep_start == -1:
                    header_sep_start = i
                elif line.startswith(HEADER_SEPARATOR):
                    header_sep_end = i
                    break
            if header_sep_start != -1 and header_sep_end != -1:
                lines = lines[: header_sep_start - 1] + lines[header_sep_end + 2 :]

            command = command.copy()  # We do not want to modify the original list

            f.seek(0)
            out_arg = False
            for i, argument in enumerate(command):
                command[i] = shlex.quote(argument)
                if "-o" in argument:
                    out_arg = True
                elif out_arg:
                    command[i] = argument.replace(BUILD_DIR, "")
                    out_arg = False
                # Check if the argument is a file
                elif os.path.isfile(argument):
                    command[i] = os.path.basename(argument)

            escaped_run_args = [
                arg[1:] if arg.startswith("\\") else shlex.quote(arg)
                for arg in self.run_args()
            ]

            f.write("/*\n")
            f.write(HEADER_SEPARATOR + "\n")
            f.write(f"Author\t: {' '.join(author)}\n")
            f.write(f"Command\t: {' '.join(command)}\n")
            if date:
                f.write(f"Date\t: {datetime.datetime.now().strftime('%d.%m.%Y')}\n")
            if self._run_args:
                f.write(
                    f"Run \t: {self.out.replace(BUILD_DIR, '')} {' '.join(escaped_run_args)}\n"
                )
            f.write(HEADER_SEPARATOR + "\n")
            f.write("*/\n")

            # Add the rest of the file
            f.writelines(lines)

    def _get_cfile_object(self, file: str):
        if os.path.exists(file):
            return CFile(file, base=self.basename)
        elif os.path.exists(os.path.join(os.path.dirname(self.filename), file)):
            return CFile(
                os.path.join(os.path.dirname(self.filename), file), base=self.basename
            )
        else:
            return CFile(file, base=self.basename)

    def _get_build_command(self):
        if not self._valid:
            return []
        command = [GCC_PATH]
        command.extend(self.args())
        command.extend(str(name) for name in self.files())
        command.extend(("-o", self.out))
        command.extend(self.libs())
        return command

    def build(self, headers: bool = True):
        if not self._valid:
            return
        self._parse()
        command = self._get_build_command()
        verbose(f"Build command: {' '.join(command)}")
        if headers:
            self.add_header(command=command)
        rcode = subprocess.run(command)
        self._built = rcode.returncode == 0
        if self._built:
            verbose(f"Built {self}", level=2)
        else:
            red(f"Failed to build {self}")

    def run(self, header: bool = True):
        if not self._valid:
            return
        if not self._built:
            self.build(header)
            if not self._built:
                red("Build failed for :", self)
                return
        if all(False for a in self.run_args() if a.startswith("\\")):
            verbose(f"Running {self.out}")
            code = subprocess.run([self.out] + self.run_args()).returncode
        else:
            escaped_run_args = [
                arg[1:] if arg.startswith("\\") else shlex.quote(arg)
                for arg in self.run_args()
            ]
            verbose(f"Running raw command : {self.out}", " ".join(escaped_run_args))
            code = os.system(self.out + " " + " ".join(escaped_run_args))
        (red if code else green)(f"Exited with code : {code}")

    def add_header(
        self, author: list[str] | None = None, command: list[str] | None = None
    ):
        self._parse()
        if author is None:
            author = self.authors()
        if command is None:
            command = self._get_build_command()
        self._insert_header(author, command, True)

    def _get_applied_list(self, lst: list[str], deduplicate: bool = True) -> list[str]:
        self._parse()
        applied = []
        for a in lst:
            if a.startswith("+"):
                if not deduplicate or a[1:] not in applied:
                    applied.append(a[1:])
            elif a.startswith("^"):
                try:
                    applied.remove(a[1:])
                except ValueError:
                    verbose("Did not remove :", a[1:])
            elif a.startswith("="):
                applied.clear()
                applied.append(a[1:])
            else:
                if a not in applied:
                    applied.append(a)
        return applied

    def files(self):
        self._parse()
        if hasattr(self, "_files_obj"):
            return self._files_obj.copy()
        self._files_obj = []
        for file in self._get_applied_list(self._files):
            self._files_obj.append(self._get_cfile_object(file))
        return self.files()  # this time it should be cached

    def args(self):
        return self._get_applied_list(self._args)

    def libs(self):
        return self._get_applied_list(self._libs)

    def authors(self):
        return self._get_applied_list(self._authors)

    def package(self):
        return self._get_applied_list(self._package)

    def run_args(self):
        return self._get_applied_list(self._run_args, deduplicate=False)

    def __repr__(self):
        return f"CFile({self.filename})"


## Packaging
def package_files(files, out_path, pname):
    """Put every file and it's dependencies in the files list in the tgz file"""
    if "{}" in out_path:
        out_path = out_path.format(pname)

    with tarfile.open(out_path, "w:gz", compresslevel=9) as tar:
        # Add all the c files without the path
        for file in files:
            if not file.filename.endswith(".c"):
                continue
            for f in file.files():
                info = tar.gettarinfo(f.filename)
                info.name = os.path.basename(f.filename)
                info.mode = 0o777
                info.uname = ", ".join(f.authors())
                info.gname = f.basename
                info.mtime = time.time_ns() // 10**9
                info.uid = 0
                info.gid = 0

                verbose(f"Adding {f} to {out_path} ... ", end="")
                tar.addfile(info, open(f.filename, "rb"))
                verbose("Done", level=2)
            for cfile in file.files():
                for file in cfile.package():
                    info = tar.gettarinfo(file)
                    info.name = os.path.basename(file)
                    info.mode = 0o666
                    info.uname = cfile.basename
                    info.gname = "Packaged files"
                    info.mtime = time.time_ns() // 10**9
                    info.uid = 0
                    info.gid = 0

                    verbose(f"Adding linked {file} to {out_path} ... ", end="")
                    tar.addfile(info, open(file, "rb"))
                    verbose("Done", level=2)


## Command line interface function
def parse_args(args: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="A simple C file builder and runner, with a simple syntax, and a simple way to package files",
        formatter_class=argparse.HelpFormatter,
    )
    parser.add_argument(
        "files",
        nargs="*",
        default=["Current directory"],
        help="The files to be built",
    )
    parser.add_argument(
        "-H",
        "--no-header",
        action="store_true",
        help="Do not add a header to the files being built",
    )
    parser.add_argument(
        "-B", "--no-build", action="store_true", help="Do not build the file"
    )
    parser.add_argument(
        "-R", "--no-run", action="store_true", help="Do not run the file"
    )
    parser.add_argument(
        "-S", "--silent", action="store_true", help="Show no output from the script"
    )
    parser.add_argument(
        "-v", "--verbose", action="store_true", help="Show verbose output"
    )
    parser.add_argument("-o", "--output", help="Path to the output file")
    parser.add_argument(
        "-p", "--package", action="store_true", help="Zip the files into a tgz file"
    )
    parser.add_argument(
        "--package-file",
        action="store",
        default=DEFAULT_PACKAGE_NAME,
        help="The name of the package file",
    )
    parser.add_argument(
        "-i", "--init", action="store_true", help="Initialize a TP folder"
    )
    parser.add_argument(
        "-U", "--update", action="store_true", help="Update the tp folder from git"
    )
    parser.add_argument(
        "-I",
        "--interactive",
        action="store_true",
        help="Run in interactive mode."
        "This opens a special shell to allow you to run commands efficiently and "
        "overrides all other actions, and the files argument. -S is not recommended",
    )
    return parser.parse_args(args)


def main(args: list[str] | None = None, interactive: bool = False):
    global _debug, _silent
    if args is None:
        args = sys.argv[1:]

    parsed = parse_args(args)

    if not interactive:
        _debug = parsed.verbose

        _silent = parsed.silent

    parsed.files = list(parsed.files)
    archive_pname = "TP"

    if parsed.update:
        only_command = (
            not parsed.init
            and parsed.files == ["Current directory"]
            and not parsed.package
        )
        update_tp(only_command)
        if only_command:
            return

    if parsed.files == ["Current directory"]:
        parsed.files = [os.getcwd()]

    if parsed.interactive:
        interactive_shell()
        return

    if parsed.init:
        if not len(parsed.files) == 1 and os.path.isdir(parsed.files[0]):
            parsed.files = [os.getcwd()]
            verbose(
                f"You must select an existing folder for the root of the TP folders. Using {parsed.files[0]}",
                level=3,
            )
        main_init(TP_FOLDER, parsed.files[0])
        return

    if len(parsed.files) == 1:
        if os.path.isdir(parsed.files[0]):
            verbose("Directory detected", level=2)
            archive_pname = os.path.basename(os.path.normpath(parsed.files[0]))
            files = []
            # Walk the directory and add all the files
            for root, _, filenames in os.walk(parsed.files[0]):
                for ignored in IGNORE_FOLDERS:
                    if ignored in root:
                        verbose(f"Ignoring {root}", level=2)
                        break
                else:
                    for filename in filenames:
                        if filename.endswith(".c"):
                            # verbose(f"Adding {filename} to the list")
                            files.append(os.path.join(root, filename))
            parsed.files = files

    if parsed.package:
        files = [CFile(f) for f in parsed.files]
        package_files(files, parsed.package_file, archive_pname)
    for file in parsed.files:
        if parsed.output:
            cfile = CFile(file, parsed.output)
        else:
            cfile = CFile(file)
        if not parsed.no_run:
            cfile.run(not parsed.no_header)
        elif not parsed.no_build:
            cfile.build(not parsed.no_header)


def interactive_shell():
    os.chdir(DEFAULT_CWD)
    # Ask the user to select a TP
    tp_md, tp_num = select_tp(TP_FOLDER)
    os.chdir(f"TP{tp_num:02d}")
    tp_source = os.path.dirname(tp_md)
    # Ask the user to select a level
    exercises, _ = select_categories(read_md_tp_ex(tp_md))
    if not exercises:
        color("No exercises found", color=colorama.Fore.RED)
        return

    color(">>> Welcome to the productive mode <<<", color=colorama.Fore.BLUE)
    color(
        "Type 'help' or 'h' for a list of commands", color=colorama.Fore.LIGHTMAGENTA_EX
    )
    color(
        "Type 'exit' or 'quit' to exit the shell",
        color=colorama.Fore.LIGHTMAGENTA_EX,
    )

    def list_ex():
        for exercise in exercises:
            color(
                f"Type 's {exercise[0]}' to go to the exercise '{exercise[1]}'",
                color=colorama.Fore.CYAN,
            )

    ex_by_num = {str(ex[0]): ex for ex in exercises}
    current_ex = ("0", "", [])
    current_file = None
    command: str = ""
    multiple = ["list"]
    while True:
        if multiple:
            command = multiple.pop(0)
        else:
            command = input(f"{current_ex[1]} > ").strip()
        match command.split():
            case ["list"] | ["l"]:
                list_ex()
            case ["select", num] | ["s", num]:
                if num not in ex_by_num:
                    color(f"Exercise {num} does not exist", color=colorama.Fore.RED)
                    continue
                current_ex = ex_by_num[num]
                color(
                    f"Going to exercise {current_ex[0]}: {current_ex[1]}",
                    color=colorama.Fore.LIGHTGREEN_EX,
                )
                current_file = CFile(open_file_from_ex(exercises, num, tp_source))
            case ["run"] | ["r"]:
                if current_file is None:
                    color("No exercise selected", color=colorama.Fore.RED)
                    continue
                color(
                    f"Running exercise {current_ex[0]}: {current_ex[1]}",
                    color=colorama.Fore.LIGHTGREEN_EX,
                )
                current_file.reload()
                current_file.run()
            case ["print"] | ["p"]:
                if current_ex[0] == "0":
                    color("No exercise selected", color=colorama.Fore.RED)
                    continue
                print_exercises(*current_ex)  # type: ignore
            case ["clear"] | ["c"]:
                clear_screen()
            case ["pwd"]:
                print(f"Current directory: {os.getcwd()}")
            case ["all"] | ["a"] | []:
                multiple = ["clear", "print", "run"]

            case ["help"] | ["h"]:
                color("List of commands:", color=colorama.Fore.LIGHTMAGENTA_EX)
                color(
                    "list, l: List the exercises"
                    "select, s: Select an exercise"
                    "run, r, (default): Run the selected exercise"
                    "pwd: Print the current directory"
                    "help, h: Show this help message",
                    color=colorama.Fore.LIGHTMAGENTA_EX,
                )
            case ["exit"] | ["quit"] | ["q"] | ["e"]:
                color("Exiting...", color=colorama.Fore.LIGHTMAGENTA_EX)
                break


if __name__ == "__main__":
    # interactive()
    main(sys.argv[1:])
