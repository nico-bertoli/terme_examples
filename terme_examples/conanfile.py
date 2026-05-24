import subprocess
from pathlib import Path

from conan import ConanFile
from conan.tools.cmake import cmake_layout

_RECIPE_ROOT = Path(__file__).resolve().parent
_NBKIT_RECIPE = _RECIPE_ROOT / "conan" / "nbkit"
_TERME_RECIPE = _RECIPE_ROOT / "conan" / "terme"
_LOCAL_NBKIT = (_RECIPE_ROOT / "../../nbkit").resolve()
_LOCAL_TERME = (_RECIPE_ROOT / "../../terme/terme").resolve()


class TermeExamplesRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def configure(self):
        if self.conf.get("user.terme_examples:local", default=False):
            subprocess.run(
                ["conan", "export", str(_LOCAL_NBKIT), "--name=nbkit", "--version=1.0.0"],
                check=True,
                capture_output=True,
            )
            subprocess.run(
                ["conan", "export", str(_LOCAL_TERME), "--name=terme", "--version=1.0.0"],
                check=True,
                capture_output=True,
            )
        else:
            subprocess.run(
                ["conan", "export", str(_NBKIT_RECIPE), "--name=nbkit", "--version=1.0.0"],
                check=True,
                capture_output=True,
            )
            subprocess.run(
                ["conan", "export", str(_TERME_RECIPE), "--name=terme", "--version=1.0.0"],
                check=True,
                capture_output=True,
            )

    def requirements(self):
        self.requires("terme/1.0.0")
        self.requires("nbkit/1.0.0")

    def layout(self):
        cmake_layout(self)
