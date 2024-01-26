"""
@file      uf2.py
@author    Lewis He (lewishe@outlook.com)
@license   MIT
@copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
@date      2023-12-15
"""

import os
from platform import platform
import sys, os
import subprocess
import inspect

Import("env", "projenv")
from SCons.Script import COMMAND_LINE_TARGETS

current_script_path = os.path.abspath(inspect.getfile(inspect.currentframe()))
script_dir = os.path.dirname(current_script_path)

uf2conv_tools_dir = None
if sys.platform.startswith('linux'):
    uf2conv_tools_dir = script_dir + "/uf2conv.py"
elif sys.platform == 'win32':
    uf2conv_tools_dir = script_dir + os.altsep + "uf2conv.py"
elif sys.platform == 'darwin':
    uf2conv_tools_dir = script_dir + "/uf2conv.py"


def after_buildprog(source, target, env):
    firmware_path = "{0}{1}{2}{3}{4}.bin".format(
        env["PROJECT_BUILD_DIR"], os.altsep, env["PIOENV"], os.altsep, env["PROGNAME"]
    )

    uf2_cmd = (
        env["PYTHONEXE"]
        + " "
        + uf2conv_tools_dir
        + " "
        + firmware_path
        + " "
        + "-c -f 0xc47e5767 -b 0 -o "
        + "./build/firmware.uf2"
    )
    if not os.path.exists("./build"):
        os.mkdir("build")
    subprocess.getoutput(uf2_cmd)


env.AddPostAction("buildprog", after_buildprog)
