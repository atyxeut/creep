# SPDX-FileCopyrightText: 2026 atyxeut <atyxeut@outlook.com>
# SPDX-License-Identifier: GPL-3.0-or-later

import re
import shutil
from pathlib import Path


def remove(path: Path):
    if not path.exists():
        return
    if path.is_dir():
        shutil.rmtree(path)
    else:
        path.unlink()


def get_ignore_list():
    res = [Path(".git"), Path(".jj")]
    for line in Path(".gitignore").read_text().splitlines():
        if re.fullmatch(r"![^/]+", line):
            res.append(Path(line[1:]))
    return res


def main():
    ignore_list = get_ignore_list()
    for path in Path(".").iterdir():
        if path not in ignore_list:
            remove(path)


if __name__ == "__main__":
    main()
