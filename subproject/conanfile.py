from conan import ConanFile
from conan.tools.files import load, copy, collect_libs
from conan.tools.cmake import cmake_layout, CMake, CMakeToolchain
import re
import os


class PackageConan(ConanFile):
    license = "Apache 2.0"
    description = "Conan example package"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False],
               "fPIC": [True, False]}
    default_options = {"shared": True,
                       "fPIC": True}
    exports_sources = [
        "cmake*",
        "includes*",
        "sources*",
        "CMakeLists.txt",
        "conanfile.py",
    ]
    generators = "CMakeDeps"
    short_paths = True

    @property
    def cwd(self):
        return os.path.dirname(os.path.realpath(__file__))

    def set_name(self):
        content = load(self, path=os.path.join(self.cwd, 'CMakeLists.txt'))
        name = re.search('set\(THIS (.*)\)', content).group(1)
        self.name = name.strip().lower()

    def requirements(self):
        self.requires("cassandra-cpp-driver/2.15.3", headers=True,  libs=True,
                      transitive_headers=True, transitive_libs=True)
        self.requires("oatpp/1.3.0", headers=True,  libs=True,
                      transitive_headers=True, transitive_libs=True)

    def layout(self):
        cmake_layout(self)

    def config_options(self):
        if self.settings.os == 'Windows':
            del self.options.fPIC

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables['CMAKE_CONAN'] = False
        tc.cache_variables["CMAKE_POLICY_DEFAULT_CMP0077"] = "NEW"
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        copy(self, pattern='LICENSE', dst='licenses', src=self.cwd)
        copy(self, pattern='AUTHORS', dst='licenses', src=self.cwd)

    def package_info(self):
        self.cpp_info.libs = collect_libs(self)
        self.cpp_info.set_property("cmake_find_mode", "both")
        project_name = self.name.upper()
        self.cpp_info.set_property("cmake_file_name", project_name)
        cmake_target_name = project_name + "::" + project_name
        self.cpp_info.set_property("cmake_target_name", cmake_target_name)
