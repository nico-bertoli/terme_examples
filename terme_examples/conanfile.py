from conan import ConanFile
from conan.tools.cmake import cmake_layout

class TermeExamplesRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("terme/1.0.0")
        self.requires("nbkit/1.0.0")

    def layout(self):
        cmake_layout(self)
