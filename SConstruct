from SCons.Environment import Environment
from SCons.Script import ARGUMENTS

import platform

mode: str = ARGUMENTS.get("mode", "release")

project_name: str = "QuickJS-RAII"
out_dir: str = "out"

env: Environment = Environment()

def add_flags(env: Environment) -> None:
    # Add C++ flags
    cpp_flags: list[str] = []
    c_flags: list[str] = []
    link_flags: list[str] = []
    cpp_defines: list[str] = []

    if platform.system() == "Windows":
        cpp_flags.append("/std:c++17")
        cpp_flags.append("/EHsc")

        if mode == "debug":
            link_flags.append("/DEBUG")


    elif platform.system() == "Linux":
        cpp_flags.append("-std=c++17")

        if mode == "debug":
            cpp_flags.append("-g")

    if platform.system() == "Windows":
        pass
    elif platform.system() == "Linux":
        pass

    env.Append(CXXFLAGS=cpp_flags)
    env.Append(CCFLAGS=c_flags)
    env.Append(LINKFLAGS=link_flags)
    env.Append(CPPDEFINES=cpp_defines)

add_flags(env)

sources: list = env.Glob("*.cpp")

target: str = f'{out_dir}/{project_name}'

env.StaticLibrary(target=target, source=sources)