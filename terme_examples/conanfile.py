from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout


class TermeExamplesConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    
    def requirements(self):
        self.requires("nbkit/1.0.0@local/stable")
        self.requires("terme/1.0.0@local/stable")
    
    def configure(self):
        # Ensure static linking for SFML (inherited from terme)
        if "sfml" in self.options:
            self.options["sfml"].shared = False
    
    def layout(self):
        cmake_layout(self)
    
    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()
