#!/usr/bin/python
import argparse
import datetime
import os
import re
import shlex
import subprocess
import tarfile
import time
import json

import colorama

_silent = False


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


_debug = False

HEADER_SEPARATOR = "_" * 20
GCC_PATH = "/usr/bin/gcc"
DEFAULT_ARGUMENTS = ["-Wall", "-Wextra", "-Werror", "-Wpedantic", "-O", "-g"]
BUILD_DIR = "build" + os.sep
IGNORE_FOLDERS = [".git", ".vscode", "build", "venv", "__pycache__", "mp2i-pv"]
DEFAULT_PACKAGE_NAME = "Alexis_Rossfelder_MP2I_{}.tgz"
TP_FOLDER = "/home/alexi/Documents/MP2I/INFO/mp2i-pv/docs/TP2022-2023"
EMPTY_TP = 3


def verbose(*args, level=1, **kwargs):
    levels = {
        1: gray,
        2: green,
        3: red,
        0: print,
    }
    if _debug:
        levels[level](*args, **kwargs)


def get_tp_list(path: str):
    lst = [
        os.path.join(TP_FOLDER, p)
        for p in os.listdir(path) + [""] * EMPTY_TP
        if "TP" in p
    ]
    result = [""] * len(lst)
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
                verbose("Unable to get the source of the TP automatically", level=3)
        if source != "":
            if (x := re.match(r"\# TP(\d*) :", open(source).readline())) and int(
                x.group(1)
            ) - 1 in range(0, len(lst)):
                result[int(x.group(1)) - 1] = tp
            else:
                not_found.append(tp)
        else:
            not_found.append(tp)
    print(json.dumps(not_found, indent=2), json.dumps(lst, indent=2))
    for nf in not_found:
        for i, tp in enumerate(result):
            if tp == "":
                result[i] = nf
                break
    return result


def select_tp(path: str) -> str:
    lst = get_tp_list(path)

    print("Please select the file to import :")
    for i, t in enumerate(lst):
        print(f"\t{i + 1} - {os.path.basename(t).replace('_', ' ')}")
    try:
        result = int(input("Select one : "))
        assert result - 1 in range(0, len(lst))
    except (KeyboardInterrupt, ValueError, AssertionError):
        print("\nAborting ...")
        exit()
    p = lst[result - 1]
    if os.path.exists(os.path.join(p, os.path.basename(p).lower() + ".md")):
        return os.path.join(p, os.path.basename(p).lower() + ".md")
    else:
        files = [f for f in os.listdir(p) if f.lower().endswith(".md")]
        if len(files) == 1:
            return os.path.join(p, files[0])
        elif len(files) > 1:
            print("Please select the file to use :")
            for i, t in enumerate(files):
                print(f"\t{i + 1} - {os.path.basename(t)}")
            try:
                result = int(input("Select one : "))
                assert result - 1 in range(0, len(files))
            except (KeyboardInterrupt, ValueError, AssertionError):
                print("\nAborting ...")
                exit()
            return os.path.join(p, files[result - 1])
        else:
            print("No MD file associated with this TP.")
            return ""


def read_md_tp(path: str) -> dict[str, list[str]]:
    result = dict()
    current_part = "Introduction"
    with open(path, "r") as f:
        for line in f:
            if x := re.match(r"\#\# (.*) -- ", line):
                current_part = x.group(1)
                result[current_part] = []
            elif x := re.match(r"\[.*(?:\.c|\.h)\]\((.*)\)", line):
                result[current_part].append(x.group(1))
    return result


# print(read_md_tp(select_tp(TP_FOLDER)))
# exit()


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

        self._parsed = False
        self._built = False
        self._valid = True

        self._files = []
        self._args = []
        self._libs = []
        self._package = []
        self._authors = ["+Alexis Rossfelder, MP2I"]
        self._run_args = []
        self._provided_name = filename

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

    def __str__(self) -> str:
        return self.filename

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


def parse_args():
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
    return parser.parse_args()


def main():
    global _debug, _silent
    parsed = parse_args()
    _debug = parsed.verbose

    _silent = parsed.silent

    parsed.files = list(parsed.files)
    archive_pname = "TP"

    if parsed.files == ["Current directory"]:
        parsed.files = [os.getcwd()]

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


if __name__ == "__main__":
    main()
