from pathlib import Path

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.scm import Git


class TermeConan(ConanFile):
    name = "terme"
    version = "1.0.0"
    settings = "os", "compiler", "build_type", "arch"
    package_type = "static-library"

    def source(self):
        Git(self).clone("https://github.com/nico-bertoli/terme.git", target=".")

    def requirements(self):
        self.requires("nbkit/1.0.0", visible=False)
        self.requires("miniaudio/0.11.22")

    def layout(self):
        cmake_layout(self)

    def _cmake_script_folder(self) -> str:
        root = Path(self.source_folder)
        for rel in ("terme/terme", "terme"):
            if (root / rel / "CMakeLists.txt").is_file():
                return rel
        return "."

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_TESTING"] = "OFF"
        tc.generate()
        CMakeDeps(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder=self._cmake_script_folder())
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["terme"]
        self.cpp_info.set_property("cmake_file_name", "terme")
        self.cpp_info.set_property("cmake_target_name", "terme::terme")
