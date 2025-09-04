# You need conan to be able to run this script, and for conan you need python
# Once python is installed, you can install conan with pip
# python3 -m pip install conan
# Then you need to specify two conan profiles, one for debug one for release build
# Invoke 'conan profile detect'
# This will create a default profile under ${HOME}/.conan2/profiles/default,
# with the following content (given you have Visual Studio installed):
# [settings]
# arch=x86_64
# build_type=Release
# compiler=msvc
# compiler.cppstd=20
# compiler.runtime=dynamic
# compiler.version=193
# os=Windows
# (compiler.version) and compiler.cppstd might change in the future, but that is OK)
# This pforiel is actually the release profile, so just rename the file to vs-2022-release
# Make a copy of it and call it vs-2022-debug
# In the debug profile change the 'build_type' from 'Release' to 'Debug'
# You are done, you can invoke this script :)
# The built library will be placed under build/lib/Debug and /build/lib/Release

conan install . --profile vs-2022-debug --build=missing
conan install . --profile vs-2022-release --build=missing
cmake . --preset conan-default
cmake --build --preset conan-release
cmake --build --preset conan-debug